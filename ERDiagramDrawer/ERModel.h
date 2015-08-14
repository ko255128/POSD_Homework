/*
 * ERModel.h
 *
 *  Created on: 2013/9/13
 *      Author: User
 */

#ifndef ERMODEL_H_
#define ERMODEL_H_

#include <string>
#include <list>
#include <hash_map>
#include "Subject.h"
#include "Compoment.h"
#include "CompomentFactory.h"
#include "Connection.h"
#include "Entity.h"
#include "CommandManager.h"
using namespace std;

class ERModel:public Subject
{

public:

	ERModel();
	~ERModel();
	enum CHECK_CONNECTION_STATS_CODE
	{
		CAN_NOT_CONNECT, CAN_CONNECT, ALREADY_CONNECTED, CONNECT_ITSELF
	};
	void clearSelect();
	void copySelect();
	unsigned int paste(vector<Component*> clipboard);

	void buildPastConnection( vector<Component*> &clipboard, hash_map<Component*,unsigned int> &pastNodeIdMap );
	string getComponentName(unsigned int id);
	

	int addNode(const Component::COMPOMENT_TYPE type, const string nodeName);
	void addNodeByUI(const string typeName, const string nodeName);
	void addNodeByGUI();
	void setNodeName(unsigned int NodeID, string name);
	void addComponent(Component* component);
	void setprimarykey(unsigned int entityID, list<unsigned int> attributeIDs);
	void setprimarykey(unsigned int entityID, vector<Component*> attributes);
	void setPrimaryKeyByUI(unsigned int entityID, list<unsigned int> attributeIDs);
	void deleteFromGui();
	bool isSelectNode();
	Component* deleteLastComponent();
	void deleteNodeByID(unsigned int deleteNodeID);
	void deleteNode(Component* deleteComponent);
	void setPrimaryKey(int pos_x,int pos_y);
	bool saveXmlfile(string filePath);

	void commandPaste();

	void sortComponentsByID();
	void addConnectByUI(unsigned int firstNodeID, unsigned int secondID, Connection::REALTION_CARDINALITY realtionCardinality);
	void addConnectByGUI(Connection::REALTION_CARDINALITY realtionCardinality);
	void undeleteNode(Component* rescueNode, vector<Component*> rescueEntitysPrimaryKey);

	void moveSelectedComponent(int moveX,int moveY);
	void moveComponent(vector<unsigned int> componentIDs,int moveX,int moveY);
	void commandMove(int deltaX,int deltaY);
	void unSelectNode(Component* component);
	void selectNode(Component* component);
	bool redoCommand();
	bool undoCommand();
	bool canInputComponentName(unsigned int index);
	bool editTextCmd (int index,string text);
	string getAllComponentDetial();
	string getAllConnectionDetial();
	string getAllEntityDetial();
	vector<string> getAllComponetNames();
	vector<string> getAllComponetType();
	vector<unsigned int> getAllComponentID();
	vector<Component*> getAllEntity();
	vector<Component*> getAllConnection();
	Component* getComponentByID(unsigned int nodeID);
	string getAllAttributeDetialOfEntity(unsigned int entityID);
	void addConnection(unsigned int firstNodeID, unsigned int secondNodeID, Connection::REALTION_CARDINALITY realtionCardinality);
	bool checkEnoughNodeToConnect();
	bool checkHaveAnyEntity();
	bool checkHaveAnyAttributeOfEntity(unsigned int entityID);
	bool checkAttributeIsInEntity(unsigned int entityID, unsigned int attributeID);
	bool checkNodeIsExist(unsigned int nodeID);
	bool checkNeedAddRealtionCardinality(unsigned int firstNodeID, unsigned int secondeNodeID);
	bool checkIsEntity(unsigned int nodeID);
	bool saveFile(string filePath,bool withPos);
	bool loadFile(string filePath);
	bool loadComponentsByString(vector<string> loadComponents,vector<string> loadConnections,vector<string> loadPrimary);
	bool canRedo();
	bool canUndo();
	CHECK_CONNECTION_STATS_CODE checkCanConnect(unsigned int firstNodeID, unsigned int secondNodeID);
	string translateDiagramToTable();
	bool componentIDCompare(Component* fistNode, Component* second);
	bool transStringToInt(string inputString,unsigned int&outputInt);
	CHECK_CONNECTION_STATS_CODE checkCanConnectForGUI();
	void drawComponent(QWidget* widget);
	bool selectConnectNode(int mouseX,int mouseY);
	bool checkNeedInputRealtionCardinalityForGUI();
	void resetSelectConnectNode();
	void selectComponent(int mouseX,int mouseY);
	void setPrivewNodelocate(int posX,int posY);
	void setLastNodeLocate(int posX,int posY);
	void setLastNodeSize(int width,int height);
	void createprivewNode(const Component::COMPOMENT_TYPE type, const string nodeName);
	void deletePrivewNode();

	vector<unsigned int> getAllEntityID();
	vector<unsigned int> getAllEntityOfAttributeID(unsigned int entityID);
	bool isPrimaryKey(unsigned int entityID,unsigned int attributeID);
	vector<unsigned int> getForeginKeyIDs(unsigned int entityID);

	unsigned int getNextNodeId() const
	{
		return _NextNodeID;
	}

	const vector<Component*>& getComponents() const
	{
		return _Components;
	}

	void setIsPressCtrl(bool isPressCtrl)
	{
		_isPressCtrl = isPressCtrl;
	}
	vector<Component*> getCilpboard()
	{
		return _clipboard;
	}


private:
	vector<Component*> _Components;
	vector<Component*> _loadingComponents;
	vector<Component*> _selectComponent;
	vector<Component*> _clipboard;
	CompomentFactory* _CompomentFactory;
	CommandManager* _CommandManger;
	Component* _previewNode;
	Component* _connectFirstNode;
	Component* _connectSecondNode;
	unsigned int _NextNodeID;
	bool _isPressCtrl;
	string getComponentDetial(Component* component);
	string getEntityPrimaryKey(Entity* entity);
	string getEntityAttributeWithoutPrimaryKey(Entity* entity);
	string getEntityForeginKey(Entity* entity, vector<Component*> &relathionshipFoeginKy);

	vector<Component*> findReferenceEntity( Entity* entity, vector<Component*> &relathionshipFoeginKey );


	
	void setAllComponentLoacation();
	bool checkRelationInTheRelationForeginKey(Component* component, vector<Component*> &relathionshipFoeginKy);
	void rescueEffectConnection(Component* rescueNode);
	bool addNodeToLoadingComponents(string componentTypeSymbol,string componentName);
	bool extractComponentFormat (vector<string> loadComponentString,bool connectionIsEmpty);
	bool extractConnectFormat(vector<string> loadConnectString );
	bool extractEntityPrimary(vector<string> loadPrimaryString);
	bool extracePrimaryKeys(string primaryString,vector<unsigned int>& primaryKeyID);
	bool setConnectForLoadCompoment(unsigned int connectionID,unsigned int firstConnectNodeID,unsigned int secondConnectNodeID);
	bool setEntityPrimaryKeyForLoadCompoment(unsigned int entityID,vector<unsigned int> primaryKeys);
	void unSetPrimaryKeyForDeleteEntity( Component* deleteComponent );
	void reSetPrimaryForRescueEntity( Component* rescueNode );
	Component* getNodeByIDForLoadComponent(unsigned int componentID);
	void setUpPrimaryKeyByAttribute( Component* component );
	void rebuildPastNodePrimaryKey(Component* source,Component* target);
};

#endif /* ERMODEL_H_ */
