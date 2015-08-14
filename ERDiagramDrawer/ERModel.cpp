/*
 * ERModel.cpp
 *
 *  Created on: 2013/9/13
 *      Author: User
 */

#include "ERModel.h"
#include "Compoment.h"
#include "Connection.h"
#include "Entity.h"
#include "AddCommand.h"
#include "DeleteCommand.h"
#include "ConnectComponentCommand.h"
#include "EditTextCommand.h"
#include "FileControl.h"
#include "AddPrimaryKeyCommand.h"
#include "MoveCommand.h"
#include "PasteCommand.h"
#include <sstream>
#include <iterator>
#include <iomanip>
#include <algorithm>

#include "stringResource.h"
#include "Attribute.h"

const int CONNECTION_WIDTH = 7;
const int TYPE_AND_ID_WIDTH = 4;
const int ENTITY_NAME_WIDTH = 10;
const unsigned int MIN_REQUIREMENT_NODE_TO_CONNECT = 2;

ERModel::ERModel() :
		_NextNodeID(0), _isPressCtrl(false)
{
	_CompomentFactory = new CompomentFactory();
	_CommandManger = new CommandManager();
	_selectComponent.clear();
	_previewNode = NULL;
	_connectFirstNode = NULL;
	_connectSecondNode = NULL;
}

ERModel::~ERModel()
{
	delete _CompomentFactory;
	delete _CommandManger;
	while (!_Components.empty())
	{
		Component* deleteComponent = _Components.back();
		_Components.pop_back();
		delete deleteComponent;
	}
	if (_previewNode != NULL)
		delete _previewNode;
}

//新增NODE
int ERModel::addNode(const Component::COMPOMENT_TYPE type, const string nodeName)
{
	Component* newNode = _CompomentFactory->createComponent(type);
	if (newNode != NULL)
	{
		newNode->setId(_NextNodeID);
		newNode->setName(nodeName);
		_Components.push_back(newNode);
		_NextNodeID++;
		return newNode->getId();
	}
	return -1;
}
//取得所有Entity詳細資訊
string ERModel::getAllEntityDetial()
{
	stringstream stringStream;
	for (vector<Component*>::iterator vectorIterator = _Components.begin(); vectorIterator != _Components.end(); vectorIterator++)
	{
		Component* component = *vectorIterator;
		if (component->getType() == Component::ENTITY)
			stringStream << getComponentDetial(component);
	}
	return stringStream.str();
}

//取得Entity內所有的Attribute資訊
string ERModel::getAllAttributeDetialOfEntity(unsigned int entityID)
{
	stringstream stringStream;
	Entity* entity = (Entity*) getComponentByID(entityID);
	vector<unsigned int> attributeIDs = entity->getAllAttributeID();
	for (vector<unsigned int>::iterator vectorIterator = attributeIDs.begin(); vectorIterator != attributeIDs.end(); vectorIterator++)
	{
		unsigned int attributeID = *vectorIterator;
		Component* attribute = getComponentByID(attributeID);
		stringStream << getComponentDetial(attribute);
	}
	return stringStream.str();
}

//確認是否有任何的Entity
bool ERModel::checkHaveAnyEntity()
{
	for (vector<Component*>::iterator vectorIterator = _Components.begin(); vectorIterator != _Components.end(); vectorIterator++)
	{
		Component* component = *vectorIterator;
		if (component->getType() == Component::ENTITY)
			return true;
	}
	return false;
}
//確認在Entity內有Attribute;
bool ERModel::checkHaveAnyAttributeOfEntity(unsigned int entityID)
{
	Entity* entity = (Entity*) getComponentByID(entityID);
	vector<unsigned int> attributeIDs = entity->getAllAttributeID();
	return !attributeIDs.empty();
}

//確認該Attrubute是否在Entity
bool ERModel::checkAttributeIsInEntity(unsigned int entityID, unsigned int attributeID)
{
	Entity* entity = (Entity*) getComponentByID(entityID);
	vector<unsigned int> attributeIDInEntity = entity->getAllAttributeID();
	return find(attributeIDInEntity.begin(), attributeIDInEntity.end(), attributeID) != attributeIDInEntity.end();
}

//設定PrimaryKey
void ERModel::setprimarykey(unsigned int entityID, list<unsigned> attributeIDs)
{
	Entity* entity = (Entity*) getComponentByID(entityID);
	entity->clearPrimaryKey();
	for (list<unsigned>::iterator vectorIterator = attributeIDs.begin(); vectorIterator != attributeIDs.end(); vectorIterator++)
	{
		Component* attribute = getComponentByID(*vectorIterator);
		if (attribute != NULL)
			entity->addPrimaryKey(attribute);
	}
	notify();
}

//將現有資料轉匯成Table
string ERModel::translateDiagramToTable()
{
	stringstream stringStream;
	vector<Component*> relationshipsForForigenKey;
	for (vector<Component*>::iterator vectorIterator = _Components.begin(); vectorIterator != _Components.end(); vectorIterator++)
	{
		Component* component = *vectorIterator;
		if (component->getType() == Component::ENTITY)
		{
			stringStream << setw(ENTITY_NAME_WIDTH) << component->getName() << ENTITY_SPEARATE;
			stringStream << PRIMARY_KEY_TIP << getEntityPrimaryKey((Entity*) component) << COMMA;
			stringStream << getEntityAttributeWithoutPrimaryKey((Entity*) component);
			stringStream << getEntityForeginKey((Entity*) component, relationshipsForForigenKey) << endl;
		}
	}
	return stringStream.str();
}

//回傳Component的詳細資訊
string ERModel::getComponentDetial(Component* component)
{
	stringstream stringStream;
	stringStream << setw(TYPE_AND_ID_WIDTH) << component->getTypeSymbol() << COMPONENT_SEPARATED;
	stringStream << setw(TYPE_AND_ID_WIDTH) << component->getId() << COMPONENT_SEPARATED;
	stringStream << TWO_BLANK_STRING << component->getName() << endl;
	return stringStream.str();
}

//取得所有Component的詳細資訊
string ERModel::getAllComponentDetial()
{
	stringstream stringStream;
	for (vector<Component*>::iterator vectorIterator = _Components.begin(); vectorIterator != _Components.end(); vectorIterator++)
	{
		Component* component = *vectorIterator;
		stringStream << getComponentDetial(component);
	}
	return stringStream.str();
}

//設定節點名稱
void ERModel::setNodeName(unsigned int nodeID, string name)
{
	for (vector<Component*>::iterator vectorIterator = _Components.begin(); vectorIterator != _Components.end(); vectorIterator++)
	{
		Component* component = *vectorIterator;
		if (component->getId() == nodeID)
		{
			component->setName(name);
			break;
		}
	}
}

//根據ID取得Componet
Component* ERModel::getComponentByID(unsigned int nodeID)
{
	for (vector<Component*>::iterator vectorIterator = _Components.begin(); vectorIterator != _Components.end(); vectorIterator++)
	{
		Component* component = *vectorIterator;
		if (component->getId() == nodeID)
			return component;
	}
	return NULL;
}

//增加Connection
void ERModel::addConnection(unsigned int firstNodeID, unsigned int secondNodeID, Connection::REALTION_CARDINALITY realtionCardinality)
{
	Component* firstNode = getComponentByID(firstNodeID);
	Component* secondNode = getComponentByID(secondNodeID);
	if (firstNode == NULL || secondNode == NULL || firstNode == secondNode || firstNode->checkAlreadyConnect(secondNode))
		return;
	Component* newConnection = _CompomentFactory->createComponent(Component::CONNECTION);
	newConnection->setId(_NextNodeID);
	_NextNodeID++;
	((Connection*) newConnection)->setRealtionCardinality(realtionCardinality);
	_Components.push_back(newConnection);
	firstNode->connectTo(newConnection);
	secondNode->connectTo(newConnection);
	newConnection->connectTo(firstNode);
	newConnection->connectTo(secondNode);
}

//取得所有Connection的詳細資料
string ERModel::getAllConnectionDetial()
{
	stringstream stringStream;
	for (vector<Component*>::iterator vectorIterator = _Components.begin(); vectorIterator != _Components.end(); vectorIterator++)
	{
		Component* component = *vectorIterator;
		if (component->getType() == Component::CONNECTION)
		{
			Connection* connection = (Connection*) component;
			stringStream << setw(CONNECTION_WIDTH) << component->getId() << CONNECTION_SEPARATED;
			stringStream << connection->getDetial() << endl;
		}
	}
	return stringStream.str();
}

//檢查是否有足夠的點供聯結
bool ERModel::checkEnoughNodeToConnect()
{
	if (_Components.size() < MIN_REQUIREMENT_NODE_TO_CONNECT)
		return false;
	else
		return true;
}

//取得Entity的PrimaryKey
string ERModel::getEntityPrimaryKey(Entity* entity)
{

	vector<Component*> primaryKeys = entity->getPrimaryKey();
	stringstream stringStream;
	stringStream << LEFT_BRACKETS;
	bool noPrimaryKey = true;
	for (vector<Component*>::iterator vectorIterator = primaryKeys.begin(); vectorIterator != primaryKeys.end(); vectorIterator++)
	{
		noPrimaryKey = false;
		stringStream << (*vectorIterator)->getName() << COMMA;
	}
	if (!noPrimaryKey)
		stringStream.seekp((int) stringStream.tellp() - 1);
	stringStream << RIGHT_BRACKETS;
	return stringStream.str();
}

//取得Entity的Attribute扣除掉Primary的部分
string ERModel::getEntityAttributeWithoutPrimaryKey(Entity* entity)
{
	vector<Component*> primaryKeys = entity->getPrimaryKey();
	vector<unsigned int> attributeIDs = entity->getAllAttributeID();
	stringstream stringStream;
	bool noAttribueWithoutPrimaryKey = true;
	for (vector<unsigned int>::iterator attributeIDIterator = attributeIDs.begin(); attributeIDIterator != attributeIDs.end(); attributeIDIterator++)
	{
		Component* component = getComponentByID(*attributeIDIterator);
		if (find(primaryKeys.begin(), primaryKeys.end(), component) == primaryKeys.end())
		{
			stringStream << component->getName() << COMMA;
			noAttribueWithoutPrimaryKey = false;
		}
	}
	if (!noAttribueWithoutPrimaryKey) //如果無任何Primary則不刪除最後一個字元
		stringStream.seekp((int) stringStream.tellp() - 1);
	stringStream << EMPTY_STRING;
	return stringStream.str();
}

//取得Entity的ForeginKey
string ERModel::getEntityForeginKey(Entity* entity, vector<Component*>& relathionshipFoeginKey)
{
	stringstream stringStream;
	vector<Component*> referenceEntitys = findReferenceEntity(entity,relathionshipFoeginKey);
	for(unsigned int i=0;i<referenceEntitys.size();i++)
	{
		Entity* referenceEntity = (Entity*) referenceEntitys.at(i);
		stringStream << FORIGEN_KEY_TIP << getEntityPrimaryKey(referenceEntity) << COMMA;
	}
	return stringStream.str();
}

//從UI新增點產生Command呼叫
void ERModel::addNodeByUI(const string typeName, const string nodeName)
{
	if (typeName.compare(ATTRITUBE_TYPE_NAME) == 0)
		_CommandManger->excute(new AddCommand(this, Component::ATTRIBUTE, nodeName));
	else if (typeName.compare(ENTITY_TYPE_NAME) == 0)
		_CommandManger->excute(new AddCommand(this, Component::ENTITY, nodeName));
	else if (typeName.compare(RELATIONSHIP_TYPE_NAME) == 0)
		_CommandManger->excute(new AddCommand(this, Component::RELATIONSHIP, nodeName));
}
//刪除Component陣列中最後一個;
Component* ERModel::deleteLastComponent()
{
	if (_Components.empty())
		return NULL;
	Component* deleteComponent = _Components.back();
	_Components.pop_back();
	if (deleteComponent->getType() == Component::CONNECTION)
	{
		vector<Component*> connections = deleteComponent->getConnectinons();
		for (unsigned int i = 0; i < connections.size(); i++)
		{
			Component* disConnectNode = connections.at(i);
			disConnectNode->disConnectTo(deleteComponent);
		}
	}
	_NextNodeID--;
	return deleteComponent;
}
//確認是否能連接
ERModel::CHECK_CONNECTION_STATS_CODE ERModel::checkCanConnect(unsigned int firstNodeID, unsigned int secondNodeID)
{
	Component* firstNode = getComponentByID(firstNodeID);
	Component* secondNode = getComponentByID(secondNodeID);
	if (firstNode == NULL || secondNode == NULL)
		return CAN_NOT_CONNECT;
	if (firstNodeID == secondNodeID)
		return CONNECT_ITSELF;
	else if (firstNode->checkAlreadyConnect(secondNode))
		return ALREADY_CONNECTED;
	else if (!firstNode->canConnectTo(secondNode))
		return CAN_NOT_CONNECT;
	else
		return CAN_CONNECT;
}
//刪除deleteComponent
void ERModel::deleteNode(Component* deleteComponent)
{
	if (deleteComponent == NULL)
		return;
	vector<Component*> connections = deleteComponent->getConnectinons(); //取得該Component的所有連線
	for (unsigned int i = 0; i < connections.size(); i++)
	{
		Component* connectedNode = connections.at(i);
		if (deleteComponent->getType() == Component::CONNECTION) //若刪除點是Contection，呼叫該點與連接線斷連
			connectedNode->disConnectTo(deleteComponent);
		else //若山除點不是Connection 則是找到連接的另一點與Connection斷連後 再將Connection移除
		{
			if(deleteComponent->getType() == Component::ENTITY)
				unSetPrimaryKeyForDeleteEntity(deleteComponent);
			Component* connectedAnthorNode = ((Connection*) connectedNode)->getAnthorConnectedNode(deleteComponent);
			connectedAnthorNode->disConnectTo(connectedNode);
			vector<Component*>::iterator deleteIterator = find(_Components.begin(), _Components.end(), connectedNode);
			if (deleteIterator != _Components.end())
				_Components.erase(deleteIterator);
		}
	}
	vector<Component*>::iterator deleteIterator = find(_Components.begin(), _Components.end(), deleteComponent);
	if (deleteIterator != _Components.end())
		_Components.erase(deleteIterator);
}

//比較兩Compoment
bool ERModel::componentIDCompare(Component* fistNode, Component* secondNode)
{
	return fistNode->getId() < secondNode->getId();
}

//刪除ID為DeleteNodeID(UI 用)
void ERModel::deleteNodeByID(unsigned int deleteNodeID)
{
	Component* deleteNode = getComponentByID(deleteNodeID);
	if (deleteNode != NULL)
		_CommandManger->excute(new DeleteCommand(this, deleteNode));
}

//將Component依照ID排序
void ERModel::sortComponentsByID()
{
	for (vector<Component*>::iterator firstIterator = _Components.begin(); firstIterator != _Components.end(); firstIterator++)
	{
		for (vector<Component*>::iterator secondIterator = _Components.begin(); secondIterator != _Components.end(); secondIterator++)
		{
			if (componentIDCompare(*firstIterator, *secondIterator))
			{
				Component* temp = *firstIterator;
				*firstIterator = *secondIterator;
				*secondIterator = temp;
			}
		}
	}
}
//取得所有ENTITY
vector<Component*> ERModel::getAllEntity()
{
	vector<Component*> allEntitys;
	for (vector<Component*>::iterator vectorIterator = _Components.begin(); vectorIterator != _Components.end(); vectorIterator++)
	{
		if ((*vectorIterator)->getType() == Component::ENTITY)
			allEntitys.push_back(*vectorIterator);
	}
	return allEntitys;
}
//取得所有Connection
vector<Component*> ERModel::getAllConnection()
{
	vector<Component*> allConnections;
	for (vector<Component*>::iterator vectorIterator = _Components.begin(); vectorIterator != _Components.end(); vectorIterator++)
	{
		if ((*vectorIterator)->getType() == Component::CONNECTION)
			allConnections.push_back(*vectorIterator);
	}
	return allConnections;
}

//存檔
bool ERModel::saveFile(string filePath,bool withPos)
{
	FileControl* fileControler = new FileControl(this);
	bool saveFileResult = fileControler->writeFile(filePath,withPos);
	delete fileControler;
	return saveFileResult;
}
//將讀檔讀近來的三段字串作辨別  接辨別成功 表示讀檔成功 則刪除原本的所有Component 並且REDO、UNDO清空
bool ERModel::loadComponentsByString(vector<string> loadComponents, vector<string> loadConnections, vector<string> loadPrimary)
{
	bool loadReslut = false;
	if (extractComponentFormat(loadComponents, loadConnections.empty()) && extractConnectFormat(loadConnections) && extractEntityPrimary(loadPrimary))
	{
		_Components.swap(_loadingComponents);
		loadReslut = true;
		delete _CommandManger;
		_CommandManger = new CommandManager();
		_NextNodeID = _Components.size();
		_selectComponent.clear();
		_clipboard.clear();
		notify();
	}
	while (!_loadingComponents.empty())
	{
		Component* deleteComponent = _loadingComponents.back();
		_loadingComponents.pop_back();
		delete deleteComponent;
		deleteComponent = NULL;
	}
	return loadReslut;
}
//還原所有rescueNode所影響的連接點
void ERModel::rescueEffectConnection(Component* rescueNode)
{
	vector<Component*> connectedNodes = rescueNode->getConnectinons();
	for (vector<Component*>::iterator vectorIterator = connectedNodes.begin(); vectorIterator != connectedNodes.end(); vectorIterator++)
	{
		if (rescueNode->getType() == Component::CONNECTION)
			(*vectorIterator)->connectTo(rescueNode);
		else
		{
			Connection* connection = (Connection*) (*vectorIterator);
			Component* anotherConnetedNode = connection->getAnthorConnectedNode(rescueNode);
			anotherConnetedNode->connectTo(connection);
			_Components.push_back(connection);
		}
	}
}
//反執行刪除
void ERModel::undeleteNode(Component* rescueNode, vector<Component*> needRescuePrimaryKeyEntity)
{
	_Components.push_back(rescueNode);
	rescueEffectConnection(rescueNode);
	if(rescueNode->getType() == Component::ENTITY)
		reSetPrimaryForRescueEntity(rescueNode);
	for (vector<Component*>::iterator vectorIterator = needRescuePrimaryKeyEntity.begin(); vectorIterator != needRescuePrimaryKeyEntity.end(); vectorIterator++)
	{
		Entity* entity = (Entity*) *vectorIterator;
		if (rescueNode->getType() == Component::ATTRIBUTE)
			entity->addPrimaryKey(rescueNode);
		else if (rescueNode->getType() == Component::CONNECTION)
		{
			Connection* connection = (Connection*) rescueNode;
			entity->addPrimaryKey(connection->getAnthorConnectedNode(entity));
		}
	}
	sortComponentsByID();
	notify();
}
//執行重做
bool ERModel::redoCommand()
{
	bool returnFlag = _CommandManger->redo();
	clearSelect();
	notify();
	return returnFlag;
}
//程行復原
bool ERModel::undoCommand()
{
	bool returnFlag = _CommandManger->undo();
	clearSelect();
	notify();
	return returnFlag;
}

//執行連接兩點
void ERModel::addConnectByUI(unsigned int firstNodeID, unsigned int secondID, Connection::REALTION_CARDINALITY realtionCardinality)
{
	_CommandManger->excute(new ConnectComponentCommand(this, firstNodeID, secondID, realtionCardinality));
}

//確認Component是否在RelathionshipForeginKey的政列中
bool ERModel::checkRelationInTheRelationForeginKey(Component* component, vector<Component*>& relathionshipFoeginKy)
{
	for (vector<Component*>::iterator relathionshipForeginKeyIterator = relathionshipFoeginKy.begin(); relathionshipForeginKeyIterator != relathionshipFoeginKy.end(); relathionshipForeginKeyIterator++)
	{
		if (component == *relathionshipForeginKeyIterator)
		{
			relathionshipForeginKeyIterator = relathionshipFoeginKy.erase(relathionshipForeginKeyIterator); //將此筆資料刪除
			return true;
		}
	}
	return false;
}
//確認NodeID的點是否存在
bool ERModel::checkNodeIsExist(unsigned int nodeID)
{
	for (vector<Component*>::iterator vectorIterator = _Components.begin(); vectorIterator != _Components.end(); vectorIterator++)
	{
		Component* component = *vectorIterator;
		if (component->getId() == nodeID)
			return true;
	}
	return false;
}
//確認是否需要輸入RealtionCardinality
bool ERModel::checkNeedAddRealtionCardinality(unsigned int firstNodeID, unsigned int secondeNodeID)
{
	Component* firstNode = getComponentByID(firstNodeID);
	Component* secondeNode = getComponentByID(secondeNodeID);
	if (firstNode == NULL || secondeNode == NULL)
		return false;
	if ((firstNode->getType() == Connection::ENTITY && secondeNode->getType() == Connection::RELATIONSHIP) || (firstNode->getType() == Connection::RELATIONSHIP && secondeNode->getType() == Connection::ENTITY))
		return true;
	else
		return false;
}
//確認輸入的ID是否為Entity
bool ERModel::checkIsEntity(unsigned int nodeID)
{
	Component* component = getComponentByID(nodeID);
	if (component == NULL)
		return false;
	if (component->getType() == Connection::ENTITY)
		return true;
	return false;
}
//新增點至讀檔Component陣列
bool ERModel::addNodeToLoadingComponents(string componentTypeSymbol, string componentName)
{
	if (componentTypeSymbol.compare(CONNECTION_TYPE_SYMBOL) == 0)
	{
		if ((componentName.compare(REALTION_CARDINALITY_ONE) != 0 && componentName.compare(REALTION_CARDINALITY_N) != 0) && !componentName.empty())
			return false;
		_loadingComponents.push_back(_CompomentFactory->createComponent(Component::CONNECTION));
	}
	else if (componentTypeSymbol.compare(ENTITY_TYPE_SYMBOL) == 0)
		_loadingComponents.push_back(_CompomentFactory->createComponent(Component::ENTITY));
	else if (componentTypeSymbol.compare(ATTRIBUTE_TYYPE_SYMBOL) == 0)
		_loadingComponents.push_back(_CompomentFactory->createComponent(Component::ATTRIBUTE));
	else if (componentTypeSymbol.compare(RELATIONSHIP_TYPE_SYMBPL) == 0)
		_loadingComponents.push_back(_CompomentFactory->createComponent(Component::RELATIONSHIP));
	else
		return false;
	_loadingComponents.back()->setId(_loadingComponents.size() - 1);
	_loadingComponents.back()->setName(componentName);
	return true;
}
//讀入Component格式是否正確(若有讀到Connection 但無設定連接 也是為失敗)
bool ERModel::extractComponentFormat(vector<string> loadComponentString, bool connectionIsEmpty)
{
	for (unsigned int i = 0; i < loadComponentString.size(); i++)
	{
		string componetString = loadComponentString.at(i);
		string componentType = componetString.substr(0, 1);
		string name = "";
		unsigned int position = componetString.find(COMMA);
		if (position != string::npos)
		{
			componentType = componetString.substr(0, position);
			const int NAMEOFFSET = 2;
			name = componetString.substr(position + NAMEOFFSET, componetString.size());
		}
		else if (name.compare("") == 0 && componentType.compare(CONNECTION_TYPE_SYMBOL) != 0)
			return false;
		if ((componentType.compare(CONNECTION_TYPE_SYMBOL) == 0 && connectionIsEmpty) || !addNodeToLoadingComponents(componentType, name))
			return false;
	}
	return true;
}
//辨別讀入 連線格式是否正確
bool ERModel::extractConnectFormat(vector<string> loadConnectString)
{
	for (unsigned int i = 0; i < loadConnectString.size(); i++)
	{
		string connectString = loadConnectString.at(i);
		unsigned int connectionID, firstConnectNodeID, secondConnectNodeID;
		unsigned int position = connectString.find(BLANK);
		if (position != string::npos && !transStringToInt(connectString.substr(0, position), connectionID))
			return false;
		connectString = connectString.substr(position + 1, connectString.size());
		position = connectString.find(",");
		if (position != string::npos && !transStringToInt(connectString.substr(0, position), firstConnectNodeID))
			return false;
		if (!transStringToInt(connectString.substr(position + 1, connectString.size()), secondConnectNodeID))
			return false;
		if (!setConnectForLoadCompoment(connectionID, firstConnectNodeID, secondConnectNodeID))
			return false;
	}
	return true;
}
//字串轉數字
bool ERModel::transStringToInt(string inputString, unsigned int& outputInt)
{
	stringstream stringStream;
	stringStream << inputString << endl;
	if ((stringStream >> outputInt) && stringStream.get() == ENDLINE)
		return true;
	return false;
}
//辨別讀入EntityPrimary設定
bool ERModel::extractEntityPrimary(vector<string> loadPrimaryString)
{
	for (unsigned int i = 0; i < loadPrimaryString.size(); i++)
	{
		unsigned int entityID;
		vector<unsigned int> primaryKeysIDs;
		string primaryString = loadPrimaryString.at(i);
		unsigned int position = primaryString.find(BLANK);
		if (position != string::npos && !transStringToInt(primaryString.substr(0, position), entityID))
			return false;
		primaryString = primaryString.substr(position + 1, primaryString.size());
		if (!extracePrimaryKeys(primaryString, primaryKeysIDs) || !setEntityPrimaryKeyForLoadCompoment(entityID, primaryKeysIDs))
			return false;
	}
	return true;
}
//設定讀入Componet的連結
bool ERModel::setConnectForLoadCompoment(unsigned int connectionID, unsigned firstConnectNodeID, unsigned secondConnectNodeID)
{
	Component* connectionComponet = getNodeByIDForLoadComponent(connectionID);
	Component* firstConnectNode = getNodeByIDForLoadComponent(firstConnectNodeID);
	Component* secondConnectNode = getNodeByIDForLoadComponent(secondConnectNodeID);
	if (connectionComponet == NULL || firstConnectNode == NULL || secondConnectNode == NULL || firstConnectNode == secondConnectNode)
		return false;
	if (connectionComponet->getType() != Component::CONNECTION || !firstConnectNode->canConnectTo(secondConnectNode) || firstConnectNode->checkAlreadyConnect(secondConnectNode) || connectionComponet->getConnectinons().size() >= 2)
		return false;
	connectionComponet->connectTo(firstConnectNode);
	connectionComponet->connectTo(secondConnectNode);
	firstConnectNode->connectTo(connectionComponet);
	secondConnectNode->connectTo(connectionComponet);
	Connection* connection = (Connection*) connectionComponet;
	if (connectionComponet->getName().compare(REALTION_CARDINALITY_ONE) == 0)
		connection->setRealtionCardinality(Connection::ONE);
	else if (connectionComponet->getName().compare(REALTION_CARDINALITY_N) == 0)
		connection->setRealtionCardinality(Connection::N);
	else
		connection->setRealtionCardinality(Connection::NONE);
	return true;
}
//辨別讀入PrimaryKEY格式
bool ERModel::extracePrimaryKeys(string primaryString, vector<unsigned int>& primaryKeyIDs)
{
	unsigned int position = primaryString.find(COMMA);
	unsigned int primaryKeyID;
	if (position != string::npos)
	{
		if (!transStringToInt(primaryString.substr(0, position), primaryKeyID))
			return false;
		primaryKeyIDs.push_back(primaryKeyID);
		return extracePrimaryKeys(primaryString.substr(position + 1, primaryString.size()), primaryKeyIDs);
	}
	else
	{
		if (!transStringToInt(primaryString, primaryKeyID))
			return false;
		primaryKeyIDs.push_back(primaryKeyID);
	}
	return true;
}
//讀檔
bool ERModel::loadFile(string filePath)
{
	FileControl* filecontrol = new FileControl(this);
	bool loadresult = filecontrol->loadFile(filePath);
	if(loadresult)
	{
		if(!filecontrol->loadPosFile(filePath))
			setAllComponentLoacation();
	}
	delete filecontrol;
	return loadresult;
}

//設定PrimaryKEY
void ERModel::setprimarykey(unsigned int entityID, vector<Component*> attributes)
{
	Component* entityComponent = getComponentByID(entityID);
	if (entityComponent->getType() != Component::ENTITY)
		return;
	Entity* entity = (Entity*) entityComponent;
	entity->clearPrimaryKey();
	for (vector<Component*>::iterator vectorIterator = attributes.begin(); vectorIterator != attributes.end(); vectorIterator++)
	{
		entity->addPrimaryKey(*vectorIterator);
		Attribute* attributes = (Attribute*) *vectorIterator;
		attributes->setIsPrimaryKey(true);
	}
}

//設定讀入的PrimaryKEY
bool ERModel::setEntityPrimaryKeyForLoadCompoment(unsigned int entityID, vector<unsigned int> primaryKeys)
{
	Component* entityComponet = getNodeByIDForLoadComponent(entityID);
	if (entityComponet == NULL || entityComponet->getType() != Component::ENTITY)
		return false;
	Entity* entity = (Entity*) entityComponet;
	for (unsigned int i = 0; i < primaryKeys.size(); i++)
	{
		Component* primaryKey = getNodeByIDForLoadComponent(primaryKeys.at(i));
		if (primaryKey == NULL || primaryKey->getType() != Component::ATTRIBUTE || !primaryKey->checkAlreadyConnect(entityComponet))
			return false;
		entity->addPrimaryKey(primaryKey);
		Attribute* attribute = (Attribute*) primaryKey;
		attribute->setIsPrimaryKey(true);
	}
	return true;
}

//執行設定PrimaryKEY
void ERModel::setPrimaryKeyByUI(unsigned int entityID, list<unsigned int> attributeIDs)
{
	_CommandManger->excute(new AddPrimaryKeyCommand(this, entityID, attributeIDs));
}

//根據ComponetID取得在讀入Componet的Component
Component* ERModel::getNodeByIDForLoadComponent(unsigned int componentID)
{
	for (vector<Component*>::iterator vectorIterator = _loadingComponents.begin(); vectorIterator != _loadingComponents.end(); vectorIterator++)
	{
		if ((*vectorIterator)->getId() == componentID)
			return *vectorIterator;
	}
	return NULL;
}
//劃出元件
void ERModel::drawComponent(QWidget* widget)
{
	if (_previewNode != NULL)
		_previewNode->draw(widget);
	for (vector<Component*>::iterator vectorIterator = _Components.begin(); vectorIterator != _Components.end(); vectorIterator++)
	{
		(*vectorIterator)->draw(widget);
	}
}
//選取Component
void ERModel::selectComponent(int mouseX, int mouseY)
{
	bool isFound = false;
	for (vector<Component*>::iterator vectorIterator = _Components.begin(); vectorIterator != _Components.end(); vectorIterator++)
	{
		Component* component = *vectorIterator;
		if (component->isCollision(mouseX, mouseY) && !isFound)
		{
			if (component->isSelected())
				unSelectNode(component);
			else
			{
				selectNode(component);
			}
			isFound = true;
		}
		else if (!_isPressCtrl)
			unSelectNode(component);
	}
	notify();
}
//初始化所有物件位置
void ERModel::setAllComponentLoacation()
{
	const int startY = 50;
	const int divide_height = 80;
	int Attributecounter = 0;
	int EntityCounter = 0;
	int RelationshipCounter = 0;
	for (vector<Component*>::iterator vectorIterator = _Components.begin(); vectorIterator != _Components.end(); vectorIterator++)
	{
		Component* component = *vectorIterator;
		if (component->getType() == Component::ENTITY)
		{
			component->setPosY(startY + EntityCounter * divide_height);
			EntityCounter++;
		}
		else if (component->getType() == Component::ATTRIBUTE)
		{
			component->setPosY(startY + Attributecounter * divide_height);
			Attributecounter++;
		}
		else if (component->getType() == Component::RELATIONSHIP)
		{
			component->setPosY(startY + RelationshipCounter * divide_height);
			RelationshipCounter++;
		}
	}
}

void ERModel::clearSelect()
{
	for (vector<Component*>::iterator vectorIterator = _Components.begin(); vectorIterator != _Components.end(); vectorIterator++)
	{
		Component* component = *vectorIterator;
		component->setSelected(false);
	}
	_selectComponent.clear();
}

void ERModel::setPrivewNodelocate(int posX, int posY)
{
	_previewNode->setPosX(posX);
	_previewNode->setPosY(posY);
	notify();
}

void ERModel::setLastNodeLocate(int posX, int posY)
{
	_Components.back()->setPosX(posX);
	_Components.back()->setPosY(posY);
}

void ERModel::createprivewNode(const Component::COMPOMENT_TYPE type, const string nodeName)
{
	_previewNode = _CompomentFactory->createComponent(type);
	_previewNode->setName(nodeName);
	_previewNode->setIsPreview(true);
}

void ERModel::deletePrivewNode()
{
	if (_previewNode != NULL)
	{
		delete _previewNode;
		_previewNode = NULL;
	}
}

void ERModel::setLastNodeSize(int width, int height)
{
	_Components.back()->setWidth(width);
	_Components.back()->setHeight(height);
}

void ERModel::addNodeByGUI()
{
	_CommandManger->excute(new AddCommand(this, _previewNode));
}

bool ERModel::selectConnectNode(int mouseX, int mouseY)
{
	for (vector<Component*>::iterator vectorIterator = _Components.begin(); vectorIterator != _Components.end(); vectorIterator++)
	{
		Component* component = *vectorIterator;
		if (component->isCollision(mouseX, mouseY))
		{
			if (_connectFirstNode == NULL)
			{
				_connectFirstNode = component;
				_connectFirstNode->setSelected(true);
				notify();
				return true;
			}
			else if (_connectSecondNode == NULL)
			{
				_connectSecondNode = component;
				_connectSecondNode->setSelected(true);
				notify();
				return true;
			}
		}
	}
	return false;
}

void ERModel::resetSelectConnectNode()
{
	_connectFirstNode = NULL;
	_connectSecondNode = NULL;
}

ERModel::CHECK_CONNECTION_STATS_CODE ERModel::checkCanConnectForGUI()
{
	if (_connectFirstNode != NULL && _connectSecondNode != NULL)
		return checkCanConnect(_connectFirstNode->getId(), _connectSecondNode->getId());
	return CAN_NOT_CONNECT;
}

bool ERModel::checkNeedInputRealtionCardinalityForGUI()
{
	if (_connectFirstNode != NULL && _connectSecondNode != NULL)
		return checkNeedAddRealtionCardinality(_connectFirstNode->getId(), _connectSecondNode->getId());
	return false;
}

void ERModel::addConnectByGUI(Connection::REALTION_CARDINALITY realtionCardinality)
{
	if (_connectFirstNode != NULL && _connectSecondNode != NULL)
		addConnectByUI(_connectFirstNode->getId(), _connectSecondNode->getId(), realtionCardinality);
	resetSelectConnectNode();
}

void ERModel::moveSelectedComponent(int moveX, int moveY)
{
	for (vector<Component*>::iterator vectorIterator = _Components.begin(); vectorIterator != _Components.end(); vectorIterator++)
	{
		Component* component = *vectorIterator;
		if (component->isSelected())
		{
			component->setPosX(component->getPosX() + moveX);
			component->setPosY(component->getPosY() + moveY);
		}
	}
	notify();
}

vector<string> ERModel::getAllComponetNames()
{
	vector<string> componentNames;
	for (vector<Component*>::iterator vectorIterator = _Components.begin(); vectorIterator != _Components.end(); vectorIterator++)
	{
		Component* component = *vectorIterator;
		componentNames.push_back(component->getName());
	}
	return componentNames;
}

vector<string> ERModel::getAllComponetType()
{
	vector<string> componentTypeNames;
	for (vector<Component*>::iterator vectorIterator = _Components.begin(); vectorIterator != _Components.end(); vectorIterator++)
	{
		Component* component = *vectorIterator;
		if (component->getType() == Component::ATTRIBUTE)
			componentTypeNames.push_back(ATTRITUBE_TYPE_NAME);
		else if (component->getType() == Component::ENTITY)
			componentTypeNames.push_back(ENTITY_TYPE_NAME);
		else if (component->getType() == Component::RELATIONSHIP)
			componentTypeNames.push_back(RELATIONSHIP_TYPE_NAME);
		else
			componentTypeNames.push_back(CONNECTION_TYPE_NAME);
	}
	return componentTypeNames;
}

vector<unsigned int> ERModel::getAllComponentID()
{
	vector<unsigned int> componentNames;
	for (vector<Component*>::iterator vectorIterator = _Components.begin(); vectorIterator != _Components.end(); vectorIterator++)
	{
		Component* component = *vectorIterator;
		componentNames.push_back(component->getId());
	}
	return componentNames;
}

bool ERModel::canInputComponentName(unsigned int index)
{
	Component* component = _Components.at(index);
	if (component == NULL)
		return false;
	if (component->getType() == Component::CONNECTION)
		return checkNeedAddRealtionCardinality(component->getConnectinons().front()->getId(), component->getConnectinons().back()->getId());
	else
		return true;
}

bool ERModel::editTextCmd(int index, string text)
{
	Component* component = _Components.at(index);
	if (component == NULL)
		return false;
	if (component->getType() == Component::CONNECTION)
	{
		if (text.compare(REALTION_CARDINALITY_ONE) != 0 && text.compare(REALTION_CARDINALITY_N) != 0)
			return false;
	}
	_CommandManger->excute(new EditTextCommand(this, component->getId(), component->getName(), text));
	notify();
	return true;
}

bool ERModel::canRedo()
{
	return _CommandManger->canReDo();
}

bool ERModel::canUndo()
{
	return _CommandManger->canUnDo();
}

void ERModel::unSelectNode(Component* component)
{
	component->setSelected(false);
	vector<Component*>::iterator selectNodeIterator = find(_selectComponent.begin(), _selectComponent.end(), component);
	if (selectNodeIterator != _selectComponent.end())
		_selectComponent.erase(selectNodeIterator);
}

void ERModel::selectNode(Component* component)
{
	component->setSelected(true);
	_selectComponent.push_back(component);
}

bool ERModel::isSelectNode()
{
	return !_selectComponent.empty();
}

void ERModel::deleteFromGui()
{
	_CommandManger->excute(new DeleteCommand(this, _selectComponent));
	for(unsigned int i=0;i<_selectComponent.size();i++)
	{
		_selectComponent.at(i)->setSelected(false);
	}
	_selectComponent.clear();
	notify();
}

void ERModel::addComponent(Component* component)
{
	_Components.push_back(component);
	_NextNodeID++;
}

void ERModel::unSetPrimaryKeyForDeleteEntity( Component* deleteComponent )
{
	Entity* entity = (Entity*) deleteComponent;
	vector<Component*> primarykeys = entity->getPrimaryKey();
	for(unsigned int index = 0 ; index < primarykeys.size();index++)
	{
		Attribute* attribute = (Attribute*) primarykeys.at(index);
		attribute->setIsPrimaryKey(false);
	}
}

void ERModel::reSetPrimaryForRescueEntity( Component* rescueNode )
{
	Entity* entity = (Entity*) rescueNode;
	vector<Component*> primaryKeys = entity->getPrimaryKey();
	for(unsigned int index = 0 ; index < primaryKeys.size();index++)
	{
		Attribute* attribute = (Attribute*)primaryKeys.at(index);
		attribute->setIsPrimaryKey(true);
	}
}

void ERModel::setPrimaryKey( int pos_x,int pos_y )
{
	for (vector<Component*>::iterator vectorIterator = _Components.begin(); vectorIterator != _Components.end(); vectorIterator++)
	{
		Component* component = *vectorIterator;
		if (component->isCollision(pos_x, pos_y))
		{
			if(component->getType()==Component::ATTRIBUTE)
			{
				setUpPrimaryKeyByAttribute(component);
			}
			return;
		}
	}
}

void ERModel::setUpPrimaryKeyByAttribute( Component* component )
{
	vector<Component*> connections = component->getConnectinons();
	for (unsigned int index = 0 ; index < connections.size();index++)
	{
		Connection* connection = (Connection*) connections.at(index);
		Component* connectedNode = connection->getAnthorConnectedNode(component);
		if(connectedNode->getType() == Component::ENTITY)
		{
			list<unsigned int> primary_key_id = ((Entity*)connectedNode)->getallPrimaryKeyID();
			list<unsigned int>::iterator primaryKeyIterator = find(primary_key_id.begin(),primary_key_id.end(),component->getId());
			if(primaryKeyIterator == primary_key_id.end())
			{
				primary_key_id.push_back(component->getId());
				_CommandManger->excute(new AddPrimaryKeyCommand(this,connectedNode->getId(),primary_key_id));
			}
		}
	}
}

bool ERModel::saveXmlfile(string filePath)
{
	FileControl* fileControl = new FileControl(this);
	return fileControl->writeXmlFile(filePath);
}

void ERModel::copySelect()
{
	_clipboard.clear();
	_clipboard = _selectComponent;
	notify();
}

unsigned int ERModel::paste( vector<Component*> clipboard )
{
	unsigned int oldNextNodeId = _NextNodeID;
	hash_map<Component*,unsigned int> pastNodeIdMap;
	for(int i=0 ;i<clipboard.size();i++)
	{
		if(clipboard.at(i)->getType() == Component::CONNECTION)
			continue;
		Component* pastNode = clipboard.at(i)->clone();
		pastNode->setId(_NextNodeID);
		_Components.push_back(pastNode);
		pastNodeIdMap.insert(hash_map<Component*,unsigned int>::value_type(clipboard.at(i),_NextNodeID));
		_NextNodeID++;
	}
	buildPastConnection(clipboard, pastNodeIdMap);




	notify();
	return _NextNodeID - oldNextNodeId;
}



void ERModel::rebuildPastNodePrimaryKey( Component* source,Component* target )
{
	if(source->getType() == Component::ATTRIBUTE && target->getType() == Component::ENTITY && ((Attribute*)source)->getIsPrimaryKey())
		((Entity*)target)->addPrimaryKey(source);
	else if(source->getType() == Component::ENTITY && target->getType() == Component::ATTRIBUTE && ((Attribute*)target)->getIsPrimaryKey())
		((Entity*)source)->addPrimaryKey(target);
}

void ERModel::moveComponent( vector<unsigned int> componentIDs,int moveX,int moveY )
{
	for(unsigned int index =0 ; index< componentIDs.size();index++)
	{
		Component* component = getComponentByID(componentIDs.at(index));
		if(component!=NULL)
		{
			component->setPosX(component->getPosX()+moveX);
			component->setPosY(component->getPosY()+moveY);
		}
	}
	notify();
}

void ERModel::commandMove(int deltaX,int deltaY)
{
	vector<unsigned int> moveComponentIDs;
	for(unsigned int index =0 ; index< _selectComponent.size();index++)
	{
		Component* component = _selectComponent.at(index);
		if(component->getType() == Component::CONNECTION)
			continue;
		moveComponentIDs.push_back(component->getId());
	}
	_CommandManger->excute(new MoveCommand(this,moveComponentIDs,deltaX,deltaY));
	notify();
}

void ERModel::commandPaste()
{
	_CommandManger->excute(new PasteCommand(this));
	notify();
}

void ERModel::buildPastConnection( vector<Component*> &clipboard, hash_map<Component*,unsigned int> &pastNodeIdMap )
{
	for(int i=0 ;i<clipboard.size();i++)
	{
		if(clipboard.at(i)->getType() == Component::CONNECTION)
			continue;
		Component* targetNode = clipboard.at(i);
		hash_map<Component*,unsigned int>::iterator idIterator = pastNodeIdMap.find(targetNode);
		unsigned int pastNodeId = idIterator->second;
		vector<Component*> targetNodeConnections = targetNode->getConnectinons();
		for(int j=0;j<targetNodeConnections.size();j++)
		{
			Connection* connection = (Connection*) targetNodeConnections.at(j);
			Component* tartNodeConnectedNode = connection->getAnthorConnectedNode(targetNode);
			idIterator = pastNodeIdMap.find(tartNodeConnectedNode);
			if(idIterator!= pastNodeIdMap.end())
			{
				addConnection(pastNodeId,idIterator->second,connection->getRealtionCardinality());
				rebuildPastNodePrimaryKey(getComponentByID(pastNodeId),getComponentByID(idIterator->second));
			}
			else
			{
				addConnection(pastNodeId,tartNodeConnectedNode->getId(),connection->getRealtionCardinality());
				rebuildPastNodePrimaryKey(getComponentByID(pastNodeId),tartNodeConnectedNode);
			}
		}
	}
}

std::string ERModel::getComponentName( unsigned int id )
{
	if(this->getComponentByID(id)!=NULL)
	{
		return this->getComponentByID(id)->getName();
	}
	else
		throw CAN_NOT_GET_NAME; 
	return "";
}

vector<unsigned int> ERModel::getAllEntityID()
{
	vector<unsigned int> entityIDs;
	entityIDs.clear();
	for(int index =0 ;index<_Components.size();index++)
	{
		if(_Components.at(index)->getType() == Component::ENTITY)
		{
			entityIDs.push_back(_Components.at(index)->getId());
		}
	}
	return entityIDs;
}

vector<unsigned int> ERModel::getAllEntityOfAttributeID( unsigned int entityID )
{
	vector<unsigned int>  attributesId;
	attributesId.clear();
	if(this->getComponentByID(entityID)!=NULL && this->getComponentByID(entityID)->getType()==Component::ENTITY)
	{
		Entity* entity = (Entity*) this->getComponentByID(entityID);
		return entity->getAllAttributeID();
	}
	return attributesId;
}

bool ERModel::isPrimaryKey( unsigned int entityID,unsigned int attributeID )
{
	if(getComponentByID(entityID)==NULL || getComponentByID(attributeID)==NULL)
		return false;
	if(getComponentByID(entityID)->getType() == Component::ENTITY && getComponentByID(attributeID)->getType()==Component::ATTRIBUTE)
	{
		Entity* entity = (Entity*) this->getComponentByID(entityID);
		list<unsigned int> primaryKeyIDs = entity->getallPrimaryKeyID();
		return find(primaryKeyIDs.begin(),primaryKeyIDs.end(),attributeID) != primaryKeyIDs.end();
	}
	return false;
}

vector<unsigned int> ERModel::getForeginKeyIDs( unsigned int entityID )
{
	vector<unsigned int> foreginKeyIDs;
	vector<unsigned int > entityIDs = getAllEntityID();
	vector<Component*> relathionshipFoeginKey;
	for(unsigned int i=0 ;i<entityIDs.size();i++)
	{
		Entity* entity = (Entity*)getComponentByID(entityIDs.at(i));
		vector<Component*> referenceEntitys = findReferenceEntity(entity,relathionshipFoeginKey);
		if(entity->getId() == entityID)
		{
			for(unsigned int referenceEntityIndex = 0 ; referenceEntityIndex < referenceEntitys.size();referenceEntityIndex++)
			{
				Entity* referenctEntity = (Entity*) referenceEntitys.at(referenceEntityIndex);
				list<unsigned int> primaryKeysID = referenctEntity->getallPrimaryKeyID();
				for(list<unsigned int>::iterator listIterator = primaryKeysID.begin();listIterator!=primaryKeysID.end();listIterator++)
					foreginKeyIDs.push_back(*listIterator);
			}
			break;
		}
	}
	return foreginKeyIDs;
}

vector<Component*> ERModel::findReferenceEntity( Entity* entity, vector<Component*> &relathionshipFoeginKey )
{
	vector<Component*> referenceEntity;
	referenceEntity.clear();
	vector<Component*> entityConnectRelation = entity->getAllConnectRelation();
	for (vector<Component*>::iterator vectorIterator = entityConnectRelation.begin(); vectorIterator != entityConnectRelation.end(); vectorIterator++)
	{
		if (checkRelationInTheRelationForeginKey(*vectorIterator, relathionshipFoeginKey))
		{
			vector<Component*> foreginKeyRelationshipConnections = (*vectorIterator)->getConnectinons();
			for (vector<Component*>::iterator relationConnectionIterator = foreginKeyRelationshipConnections.begin(); relationConnectionIterator != foreginKeyRelationshipConnections.end(); relationConnectionIterator++)
			{
				Connection* relationConnection = (Connection*) *relationConnectionIterator;
				if (relationConnection->getRealtionCardinality() != Connection::ONE)
					continue;
				Component* connectedAnotherComponent = relationConnection->getAnthorConnectedNode(*vectorIterator);
				if (connectedAnotherComponent != entity && connectedAnotherComponent->getType() == Component::ENTITY)
					referenceEntity.push_back(connectedAnotherComponent);
			}
		}
		else
			relathionshipFoeginKey.push_back(*vectorIterator);
	}
	return referenceEntity;
}

