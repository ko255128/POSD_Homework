/*
 * PresentationModel.cpp
 *
 *  Created on: 2013/10/31
 *      Author: ChunHsiang
 */

#include "PresentationModel.h"
#include "stringResource.h"
#include "ERModel.h"
#include "PointerState.h"
#include "AddNodeState.h"
#include "ConnectState.h"
#include "setPrimaryKeyState.h"
#include "State.h"
#include <string>
#include <qpainter.h>
#include <qpoint.h>
using namespace std;
PresentationModel::~PresentationModel()
{
	if(_state!=NULL)
		delete _state;
}

void PresentationModel::showConnectErrorMessageAndRest(ERModel::CHECK_CONNECTION_STATS_CODE connectStats, unsigned int secondNodeID)
{
	stringstream stringStream;
	stringStream << MESSAGE_BEFORE_FIRST_NODE_ID;
	if (connectStats == ERModel::CONNECT_ITSELF)
		stringStream << _ParameterCache.front() << CONNECT_MESSAGE_CONNECT_IT_SELF;
	else if (connectStats == ERModel::CAN_NOT_CONNECT)
		stringStream << _ParameterCache.front() << CONNECT_MESSAGE_CANOT_CONNECT_BEFORE_SECOND_NODE_PART << secondNodeID << SIGNLE_QUTOE_AND_NEW_LINE;
	else
		stringStream << _ParameterCache.front() << CONNECT_MESSAGE_ALREADY_CONNECT_BEFORE_SECOND_NODE_PART << secondNodeID << SIGNLE_QUTOE_AND_NEW_LINE;
	stringStream << MAIN_MENU << NOTICE_INPUT_SYMBOL;
	_ShowMessage = stringStream.str();
	functionFinish();
}

void PresentationModel::functionFinish()
{
	_ParameterCache.clear();
	_InputState = SELECT_FUNCTION;
}

void PresentationModel::processInputSecondConnectNode(unsigned int inputInt)
{
	ERModel::CHECK_CONNECTION_STATS_CODE connectStatsCode = _ERModel->checkCanConnect(_ParameterCache.front(), inputInt);
	if (connectStatsCode == ERModel::CAN_CONNECT)
	{
		if (_ERModel->checkNeedAddRealtionCardinality(_ParameterCache.front(), inputInt))
		{
			_ParameterCache.push_back(inputInt);
			_InputState = CONNECT_CARDINALITY;
		}
		else
		{
			stringstream stringStream;
			_ERModel->addConnectByUI(_ParameterCache.front(), inputInt, Connection::NONE);
			stringStream << MESSAGE_BEFORE_FIRST_NODE_ID << _ParameterCache.front() << CONNECT_MESSAGE_CONNECT_SUCCEED_BEFORE_SECOND_NODE_PART << inputInt << SIGNLE_QUTOE_AND_NEW_LINE;
			stringStream << CONNECT_HEADER << _ERModel->getAllConnectionDetial() << CONNECT_ENDLINE << MAIN_MENU << NOTICE_INPUT_SYMBOL;
			_ShowMessage = stringStream.str();
			functionFinish();
		}
	}
	else
		showConnectErrorMessageAndRest(connectStatsCode, inputInt);
}

void PresentationModel::processInputConnectNodeID(unsigned int inputInt)
{
	if (_InputState == CONNECT_FIRST_NODE)
	{
		_ParameterCache.push_back(inputInt);
		_ShowMessage = NOTIC_ENTER_CONNECT_SECOND_NODE_ID;
		_InputState = CONNECT_SECOND_NODE;
	}
	else
		processInputSecondConnectNode(inputInt);
}

void PresentationModel::processInputRelationCardinality(unsigned int inputInt)
{
	stringstream stringStream;
	if (inputInt > 1)
		_ShowMessage = INPUT_INVALID_VALUE + NOTICE_INPUT_SYMBOL;
	else
	{
		stringStream << MESSAGE_BEFORE_FIRST_NODE_ID << _ParameterCache.front() << CONNECT_MESSAGE_CONNECT_SUCCEED_BEFORE_SECOND_NODE_PART << _ParameterCache.back() << SIGNLE_QUTOE_AND_NEW_LINE;
		if (inputInt == 0)
		{
			_ERModel->addConnectByUI(_ParameterCache.front(), _ParameterCache.back(), Connection::ONE);
			stringStream << CONNECT_MESSAGE_CARDINALITY_IS_ONE;
			functionFinish();
		}
		else if (inputInt == 1)
		{
			_ERModel->addConnectByUI(_ParameterCache.front(), _ParameterCache.back(), Connection::N);
			stringStream << CONNECT_MESSAGE_CARDINALITY_IS_N;
			functionFinish();
		}
		stringStream << CONNECT_HEADER << _ERModel->getAllConnectionDetial() << CONNECT_ENDLINE << MAIN_MENU << NOTICE_INPUT_SYMBOL;
		_ShowMessage = stringStream.str();
	}
}

void PresentationModel::processAddConnectInput(string input)
{

	unsigned int inputInt;
	if (_ERModel->transStringToInt(input, inputInt))
	{
		if (_InputState == CONNECT_FIRST_NODE || _InputState == CONNECT_SECOND_NODE)
		{
			if (_ERModel->checkNodeIsExist(inputInt))
				processInputConnectNodeID(inputInt);
			else
				_ShowMessage = NOTIC_INPUT_ID_NOT_EXIST;
		}
		else
			processInputRelationCardinality(inputInt);
	}
	else
		_ShowMessage = NOTIC_INPUT_ID_NOT_EXIST;

}

bool PresentationModel::processSelectFunction(string input)
{
	unsigned int selectedFuntion = 0;
	if (_ERModel->transStringToInt(input, selectedFuntion))
	{
		if (selectedFuntion >= LOADE_FILE && selectedFuntion <= DISPLAY_DIAGRAM)
			processSelection5previousFunction(selectedFuntion);
		else if (selectedFuntion > DISPLAY_DIAGRAM && selectedFuntion < EXIT)
			processSelectionlastFunction(selectedFuntion);
		else if (selectedFuntion == EXIT)
			return false;
		else
			_ShowMessage = INPUT_INVALID_VALUE + NOTICE_INPUT_SYMBOL;
	}
	else
		_ShowMessage = INPUT_INVALID_VALUE + NOTICE_INPUT_SYMBOL;
	return true;
}

void PresentationModel::printCurrentDiagram()
{
	_ShowMessage = getAllComponentDetial();
	_ShowMessage = _ShowMessage + getAllConnectionDetial();
	_ShowMessage = _ShowMessage + MAIN_MENU + NOTICE_INPUT_SYMBOL;
}

void PresentationModel::processLoadFile(string input)
{
	if (_ERModel->loadFile(input))
		_ShowMessage = LOAD_FILE_SUCCEED_NOTICE + getAllComponentDetial() + getAllConnectionDetial() + MAIN_MENU + NOTICE_INPUT_SYMBOL;
	else
		_ShowMessage = LOAD_FILE_ERROR_NOTICE + MAIN_MENU + NOTICE_INPUT_SYMBOL;
	functionFinish();
}

void PresentationModel::processSaveFile(string input)
{
	if (_ERModel->saveFile(input,false))
		_ShowMessage = SVAE_FILE_SUCCEED_NOTICE + MAIN_MENU + NOTICE_INPUT_SYMBOL;
	else
		_ShowMessage = SAVE_FILE_ERROR_NOTICE + MAIN_MENU + NOTICE_INPUT_SYMBOL;
	functionFinish();
}

void PresentationModel::showMessageWhenChooseConnectNode()
{
	if (_ERModel->checkEnoughNodeToConnect())
	{
		_InputState = CONNECT_FIRST_NODE;
		_ShowMessage = NOTIC_ENTER_CONNECT_FIRST_NODE_ID + NOTICE_INPUT_SYMBOL;
	}
	else
		_ShowMessage = NOTIC_DO_NOT_HAVE_ENOUGH_NODE_CONNECT + MAIN_MENU + NOTICE_INPUT_SYMBOL;
}

void PresentationModel::processSelection5previousFunction(unsigned int selectFunction)
{
	if (selectFunction == LOADE_FILE)
	{
		_InputState = LOAD_FILE_PATH;
		_ShowMessage = INPUT_FILE_PATH_NOTICE;
	}
	else if (selectFunction == SAVE_FILE)
	{
		_InputState = SAVE_FILE_PATH;
		_ShowMessage = INPUT_FILE_PATH_NOTICE;
	}
	else if (selectFunction == ADD_NODE)
	{
		_InputState = ADD_NODE_TYPE;
		_ShowMessage = ENTER_ADD_NODE_TYPE_NOTICE + NODE_TYPE_NOTICE + NOTICE_INPUT_SYMBOL;
	}
	else if (selectFunction == CONNECT_NODE)
		showMessageWhenChooseConnectNode();
	else if (selectFunction == DISPLAY_DIAGRAM)
		printCurrentDiagram();
}

void PresentationModel::processUnDo()
{
	if (_ERModel->undoCommand())
		_ShowMessage = UNDO_SUCCEED_NOTICE + getAllComponentDetial() + getAllConnectionDetial() + MAIN_MENU + NOTICE_INPUT_SYMBOL;
	else
		_ShowMessage = CAN_NOT_UNDO_NOTICE + MAIN_MENU + NOTICE_INPUT_SYMBOL;
}

void PresentationModel::processEnterEntityID(unsigned int inputInt)
{
	stringstream stringStream;
	if (_ERModel->checkIsEntity(inputInt))
	{
		if (_ERModel->checkHaveAnyAttributeOfEntity(inputInt))
		{
			_InputState = NEED_PRIMARYKEY_ID;
			_ParameterCache.push_back(inputInt);
			stringStream << PRINT_ATTRIBUTE_NOTICE << inputInt << SIGNLE_QUTOE_AND_NEW_LINE << COMPONENT_HEADER << _ERModel->getAllAttributeDetialOfEntity(inputInt) << COMPONENT_ENDLINE << INPUT_PRIMARY_KEYS_ID_NOTICE << NOTICE_INPUT_SYMBOL;
			_ShowMessage = stringStream.str();
		}
		else
		{
			stringStream << DO_NOT_HAVE_ANY_ATTRIBUTE_OF_ENTITY << inputInt << SIGNLE_QUTOE_AND_NEW_LINE << MAIN_MENU << NOTICE_INPUT_SYMBOL;
			functionFinish();
		}
	}
	else
	{
		stringStream << MESSAGE_BEFORE_FIRST_NODE_ID << inputInt << SELECT_ENTIT_FAILURE_MESSAGE_AFTER_NODE_ID + NOTICE_INPUT_SYMBOL;
		_ShowMessage = stringStream.str();
	}
}

void PresentationModel::processInputDeleteNodeID(string input)
{
	unsigned int inputInt;
	if (_ERModel->transStringToInt(input, inputInt))
	{
		if (_ERModel->checkNodeIsExist(inputInt))
		{
			stringstream stringStream;
			_ERModel->deleteNodeByID(inputInt);
			stringStream << DELETE_SUCCED_NOITCE_BEFORE_NODE_ID_PART << inputInt << DELETE_SUCCED_NOTICE_AFTER_NODE_ID_PART;
			stringStream << getAllComponentDetial() << getAllConnectionDetial() << MAIN_MENU << NOTICE_INPUT_SYMBOL;
			_ShowMessage = stringStream.str();
			functionFinish();
		}
		else
			_ShowMessage = COMPONE_ID_NOT_EXIST_NOTICE + NOTICE_INPUT_SYMBOL;
	}
	else
	{
		_ShowMessage = COMPONE_ID_NOT_EXIST_NOTICE + NOTICE_INPUT_SYMBOL;
	}
}

void PresentationModel::processInputPrimaryKey(const string& input)
{
	list<unsigned int> inputAttributeIDs;
	if (extrectInputValue(input, inputAttributeIDs))
	{
		if (verifyInputAttributeIDOfEntity(_ParameterCache.front(), inputAttributeIDs))
		{
			stringstream stringStream;
			_ERModel->setPrimaryKeyByUI(_ParameterCache.front(), inputAttributeIDs);
			stringStream << INPUT_PRIMARY_KEY_SUCCEED_NOTICE_BEFORE_ENTITY_ID_PART << _ParameterCache.front() << INPUT_PRIMARY_KEY_SUCCEED_NOTICE_BEFORE_PRIMARY_KEYS_PART << input << INPUT_PRIMARY_KEY_SUCCEED_NOTICE_AFTERT_PRIMARY_KEYS_PART;
			stringStream << MAIN_MENU << NOTICE_INPUT_SYMBOL;
			_ShowMessage = stringStream.str();
			functionFinish();
		}
	}
	else
		_ShowMessage = INPUT_PRIMARY_KEY_FORMAT_ERROR_NOTICE + NOTICE_INPUT_SYMBOL;
}

void PresentationModel::processSetPrimaryKey(string input)
{
	unsigned int inputInt;
	if (_InputState == NEED_ENTITY_ID)
	{
		if (_ERModel->transStringToInt(input, inputInt))
			processEnterEntityID(inputInt);
		else
			_ShowMessage = INPUT_INVALID_VALUE + NOTICE_INPUT_SYMBOL;
	}
	else
		processInputPrimaryKey(input);
}

bool PresentationModel::verifyInputAttributeIDOfEntity(unsigned int entityID, list<unsigned int>& inputAttributeIDs)
{
	for (list<unsigned int>::iterator vectorIterator = inputAttributeIDs.begin(); vectorIterator != inputAttributeIDs.end(); vectorIterator++)
	{
		if (!_ERModel->checkAttributeIsInEntity(entityID, *vectorIterator))
		{
			stringstream stringStream;
			stringStream << MESSAGE_BEFORE_FIRST_NODE_ID << *vectorIterator << INPUT_PRIMARY_KEY_NOT_BELONG_ENTITY_MIDDLE_PART << entityID << INPUT_PRIMARY_KEY_NOT_BELONG_ENTITY_END_PART << NOTICE_INPUT_SYMBOL;
			_ShowMessage = stringStream.str();
			return false;
		}
	}
	return true;

}

bool PresentationModel::extrectInputValue(std::string input, list<unsigned int>& inputAttributeIDs)
{
	int temp;
	stringstream stream;
	unsigned int pos = input.find(COMMA);
	if (pos != string::npos)
	{
		stream << input.substr(0, pos) << endl;
		if (!extrectInputValue(input.substr(pos + 1, input.size()), inputAttributeIDs))
			return false;
	}
	else
		stream << input << endl;
	if (stream >> temp && stream.get() == ENDLINE)
		inputAttributeIDs.push_front(temp);
	else
		return false;
	return true;
}

void PresentationModel::processReDo()
{
	if (_ERModel->redoCommand())
		_ShowMessage = REDO_SUCCEED_NOTICE + getAllComponentDetial() + getAllConnectionDetial() + MAIN_MENU + NOTICE_INPUT_SYMBOL;
	else
		_ShowMessage = CAN_NOT_UNDO_NOTICE + MAIN_MENU + NOTICE_INPUT_SYMBOL;
}

void PresentationModel::showMessageWhenChooseSetPrimary()
{
	if (_ERModel->checkHaveAnyEntity())
	{
		_ShowMessage = PRINT_ENTITY_NOTICE + COMPONENT_HEADER + _ERModel->getAllEntityDetial() + COMPONENT_ENDLINE + NOTIC_ENTER_ENTITY_ID + NOTICE_INPUT_SYMBOL;
		_InputState = NEED_ENTITY_ID;
	}
	else
		_ShowMessage = NOTICE_DO_NOT_HAVE_ANY_ENTITY + MAIN_MENU + NOTICE_INPUT_SYMBOL;
}

void PresentationModel::showMessageWhenChooseDeleteComponent()
{
	if (!_ERModel->getComponents().empty())
	{
		_InputState = DELETE_NODE_ID;
		_ShowMessage = NOTIC_ENTER_COMPONENT_ID + NOTICE_INPUT_SYMBOL;
	}
	else
		_ShowMessage = DO_NOT_HAVE_ANY_COMPONENT + MAIN_MENU + NOTICE_INPUT_SYMBOL;
}

void PresentationModel::processSelectionlastFunction(unsigned int selectedFunction)
{
	if (selectedFunction == SET_PRIMRYKEY)
		showMessageWhenChooseSetPrimary();
	else if (selectedFunction == DISPLAY_TABLE)
		_ShowMessage = TABLE_HEADER + _ERModel->translateDiagramToTable() + TABLE_ENDLINE + MAIN_MENU + NOTICE_INPUT_SYMBOL;
	else if (selectedFunction == DELETE_COMPONENT)
		showMessageWhenChooseDeleteComponent();
	else if (selectedFunction == UNDO)
		processUnDo();
	else if (selectedFunction == REDO)
		processReDo();
}

string PresentationModel::getAllConnectionDetial()
{
	return PRING_CONNECT_NOTICE + CONNECT_HEADER + _ERModel->getAllConnectionDetial() + CONNECT_ENDLINE;
}

string PresentationModel::getAllComponentDetial()
{
	return PRINT_COMPONET_NOTICE + COMPONENT_HEADER + _ERModel->getAllComponentDetial() + COMPONENT_ENDLINE;
}

void PresentationModel::processInputNodeType(const string& input)
{
	if (input.compare(ATTRIBUTE_TYYPE_SYMBOL) == 0)
		_AddNodeType = ATTRITUBE_TYPE_NAME;
	else if (input.compare(ENTITY_TYPE_SYMBOL) == 0)
		_AddNodeType = ENTITY_TYPE_NAME;
	else if (input.compare(RELATIONSHIP_TYPE_SYMBPL) == 0)
		_AddNodeType = RELATIONSHIP_TYPE_NAME;
	else
	{
		_ShowMessage = ADD_NODE_TYPE_FAIURE + NOTICE_INPUT_SYMBOL;
		return;
	}
	_ShowMessage = ADD_NODE_NAME_NOTICE + NOTICE_INPUT_SYMBOL;
	_InputState = ADD_NODE_NAME;
}

void PresentationModel::processAddNodeInput(string input)
{
	if (_InputState == ADD_NODE_TYPE)
		processInputNodeType(input);
	else
	{
		stringstream stringStream;
		_ERModel->addNodeByUI(_AddNodeType, input);
		stringStream << ADD_NODE_SUCCEED_FRONT_TYPE_PART << _AddNodeType << ADD_NODE_SUCCEED_FRONT_ID_PART << _ERModel->getComponents().size() - 1 << ADD_NODE_SUCCEED_FRONT_NAME_PART << input << QUOTATION_MARK << ENDLINE;
		stringStream << COMPONENT_HEADER << _ERModel->getAllComponentDetial() << COMPONENT_ENDLINE;
		_ShowMessage = stringStream.str() + MAIN_MENU + NOTICE_INPUT_SYMBOL;
		_AddNodeType = "";
		functionFinish();
	}
}

void PresentationModel::iniButtonCheckState()
{
	_addAttributeButtonChecked = false;
	_addEntityButtonChecked = false;
	_addRelationshipButtonChecked = false;
	_connectButtonChecked = false;
	_pointerButtonChecked = true;
	_trackingMouse = false;
	_needDrawPrivewLine = false;
	_primaryKeyButtonChecked = false;
}

PresentationModel::PresentationModel(ERModel* eRModel)
{
	this->_ERModel = eRModel;
	this->_state = NULL;
	this->_InputState = SELECT_FUNCTION;
	this->_AddNodeType = "";
	_ParameterCache.clear();
	_ShowMessage = MAIN_MENU + NOTICE_INPUT_SYMBOL;
	changeState(POINTER_STATE);
	iniButtonCheckState();

}

bool PresentationModel::processInput(string input)
{
	if (_InputState == SELECT_FUNCTION)
		return processSelectFunction(input);
	else if (_InputState == ADD_NODE_TYPE || _InputState == ADD_NODE_NAME)
		processAddNodeInput(input);
	else if (_InputState == CONNECT_FIRST_NODE || _InputState == CONNECT_SECOND_NODE || _InputState == CONNECT_CARDINALITY)
		processAddConnectInput(input);
	else if (_InputState == NEED_ENTITY_ID || _InputState == NEED_PRIMARYKEY_ID)
		processSetPrimaryKey(input);
	else if (_InputState == DELETE_NODE_ID)
		processInputDeleteNodeID(input);
	else if (_InputState == LOAD_FILE_PATH)
		processLoadFile(input);
	else if (_InputState == SAVE_FILE_PATH)
		processSaveFile(input);
	return true;
}

bool PresentationModel::processLoadFileForGUI(string filePath)
{
	bool loadFileSucceed = _ERModel->loadFile(filePath);
	if (loadFileSucceed)
		changeState(POINTER_STATE);
	return loadFileSucceed;
}

void PresentationModel::drawComponent(QWidget* widget)
{
	if (_needDrawPrivewLine)
	{
		QPainter painter(widget);
		painter.setPen(Qt::blue);
		painter.drawLine(_privewLineStartPoint,_privewLineEndPoint);
	}
	_ERModel->drawComponent(widget);
}

void PresentationModel::selectComponent(int mouseX, int mouseY)
{
	_ERModel->selectComponent(mouseX, mouseY);
}

void PresentationModel::pressCtrl(int key)
{
	if (key == Qt::Key_Control)
		_ERModel->setIsPressCtrl(true);
	else
		_ERModel->setIsPressCtrl(false);
}

void PresentationModel::releaseCtrl()
{
	_ERModel->setIsPressCtrl(false);
}

void PresentationModel::mousePressEvent(QMouseEvent* event, QWidget* widget)
{
	_state->mousePressEvent(event, widget);
}

void PresentationModel::mouseReleaseEvent(QMouseEvent* event, QWidget* widget)
{
	_state->mouseReleaseEvent(event, widget);
}

void PresentationModel::mouseMoveEvent(QMouseEvent* event, QWidget* widget)
{
	_state->mouseMoveEvent(event, widget);
}

void PresentationModel::changeState(STATE state)
{
	if (_state != NULL)
		delete _state;
	_ERModel->clearSelect();
	_ERModel->deletePrivewNode();
	_ERModel->resetSelectConnectNode();
	_trackingMouse = false;
	_needDrawPrivewLine = false;
	if (state == POINTER_STATE)
	{
		iniButtonCheckState();
		_state = new PointerState(this);
	}
	else if (state == ADD_NODE_STATE)
	{
		_state = new AddNodeState(this);
		_trackingMouse = true;
	}
	else if(state == SET_PRIMARY_KEY_STATE)
	{
		_state = new setPrimaryKeyState(this);
	}
	else
		_state = new ConnectState(this);
	_ERModel->notify();
}

void PresentationModel::creatPrivewNode(string typeName, string nodeName)
{
	if (typeName.compare(ATTRITUBE_TYPE_NAME) == 0)
		_ERModel->createprivewNode(Component::ATTRIBUTE, nodeName);
	else if (typeName.compare(ENTITY_TYPE_NAME) == 0)
		_ERModel->createprivewNode(Component::ENTITY, nodeName);
	else if (typeName.compare(RELATIONSHIP_TYPE_NAME) == 0)
		_ERModel->createprivewNode(Component::RELATIONSHIP, nodeName);
	else
		_ERModel->deletePrivewNode();
}

void PresentationModel::setPrivewNodePosistion(int posX, int posY)
{
	_ERModel->setPrivewNodelocate(posX, posY);
}

void PresentationModel::AddNode()
{
	_ERModel->addNodeByGUI();
	_ERModel->deletePrivewNode();
}

Subject* PresentationModel::getSubject()
{
	return _ERModel;
}

void PresentationModel::clearButtonChecked()
{
	_pointerButtonChecked = false;
	_addAttributeButtonChecked = false;
	_addEntityButtonChecked = false;
	_addRelationshipButtonChecked = false;
	_connectButtonChecked = false;
	_primaryKeyButtonChecked = false;
}

bool PresentationModel::selectConnectNode(int mouseX, int mouseY)
{
	return _ERModel->selectConnectNode(mouseX, mouseY);
}

bool PresentationModel::checkCanConnect()
{
	if (_ERModel->checkCanConnectForGUI() == ERModel::CAN_CONNECT)
		return true;
	else
		return false;
}

string PresentationModel::getConnectErrorMessage()
{
	ERModel::CHECK_CONNECTION_STATS_CODE respons = _ERModel->checkCanConnectForGUI();
	if (respons == ERModel::CAN_NOT_CONNECT)
		return ILLEGAL_CONNECT;
	else if (respons == ERModel::CONNECT_ITSELF)
		return CONNECT_ITSELF;
	else if (respons == ERModel::ALREADY_CONNECTED)
		return ALREADY_CONNECT;
	else
		return EMPTY_STRING;
}

bool PresentationModel::checkNeedInputRealtionCardinality()
{
	return _ERModel->checkNeedInputRealtionCardinalityForGUI();
}

void PresentationModel::Addconnection(string relationCardinality)
{
	if (relationCardinality.compare(REALTION_CARDINALITY_N) == 0)
		_ERModel->addConnectByGUI(Connection::N);
	else if (relationCardinality.compare(REALTION_CARDINALITY_ONE) == 0)
		_ERModel->addConnectByGUI(Connection::ONE);
	else
		_ERModel->addConnectByGUI(Connection::NONE);
	changeState(POINTER_STATE);
}

void PresentationModel::moveComponent(int moveX, int moveY)
{
	_ERModel->moveSelectedComponent(moveX,moveY);
}

unsigned int PresentationModel::getNumberOfComponent()
{
	return _ERModel->getComponents().size();
}

vector<string> PresentationModel::getAllComponetNames()
{
	return _ERModel->getAllComponetNames();
}

vector<string> PresentationModel::getAllComponetType()
{
	return _ERModel->getAllComponetType();
}

vector<unsigned int> PresentationModel::getAllComponentID()
{
	return _ERModel->getAllComponentID();
}

bool PresentationModel::canInputComponentName(unsigned int index)
{
	return _ERModel->canInputComponentName(index);
}

bool PresentationModel::editTextCmd(int index, string text)
{
	return _ERModel->editTextCmd(index,text);
}

bool PresentationModel::canRedo()
{
	return _ERModel->canRedo();
}

bool PresentationModel::canUndo()
{
	return _ERModel->canUndo();
}

void PresentationModel::processRedoForGui()
{
	_ERModel->redoCommand();
}

void PresentationModel::processUndoForGui()
{
	_ERModel->undoCommand();
}

bool PresentationModel::isHaveSelect()
{
	return _ERModel->isSelectNode();
}


void PresentationModel::processDeleteComponetFromGui()
{
	_ERModel->deleteFromGui();
}

void PresentationModel::setPrimaryKey( int pos_x,int pos_y )
{
	_ERModel->setPrimaryKey(pos_x,pos_y);
}

bool PresentationModel::saveFileWithGUI( string filePath )
{
	return _ERModel->saveFile(filePath,true);
}

bool PresentationModel::canSave()
{
	return !_ERModel->getComponents().empty();
}

bool PresentationModel::saveXmlFile( string filePath )
{
	return _ERModel->saveXmlfile(filePath);
}

void PresentationModel::cut()
{
	_ERModel->copySelect();
	_ERModel->deleteFromGui();
}

void PresentationModel::commandMove(int deltaX,int deltaY)
{
	_ERModel->commandMove(deltaX,deltaY);
}

void PresentationModel::copy()
{
	_ERModel->copySelect();
}

void PresentationModel::paste()
{
	_ERModel->commandPaste();
}

bool PresentationModel::canPaste()
{
	return !_ERModel->getCilpboard().empty();
}

vector<unsigned int> PresentationModel::getAllEntityID()
{
	return _ERModel->getAllEntityID();
}

vector<unsigned int> PresentationModel::getAllEntityOfAttributeID( unsigned int entityID )
{
	return _ERModel->getAllEntityOfAttributeID(entityID);
}

bool PresentationModel::isPrimaryKey( unsigned int entityID,unsigned int attributeID )
{
	return _ERModel->isPrimaryKey(entityID,attributeID);
}

vector<unsigned int> PresentationModel::getForeginKeyIDs( unsigned int entityID )
{
	return _ERModel->getForeginKeyIDs(entityID);
}

std::string PresentationModel::getComponentName( unsigned int componentID )
{
	return _ERModel->getComponentName(componentID);
}


