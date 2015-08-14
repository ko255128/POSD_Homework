/*
 * PresentationModel.h
 *
 *  Created on: 2013/10/31
 *      Author: ChunHsiang
 */

#ifndef PRESENTATIONMODEL_H_
#define PRESENTATIONMODEL_H_
#include "ERModel.h"
#include "Subject.h"
#include <string>
#include <vector>

using namespace std;

class State;

class PresentationModel
{
public:
	PresentationModel(ERModel* eRModel);

	void cut();
	void copy();
	void paste();
	bool processInput(string input);
	bool processLoadFileForGUI(string filePath);
	bool selectConnectNode(int mouseX,int mouseY);
	void commandMove(int deltaX,int deltaY);
	void drawComponent(QWidget* widget);
	void selectComponent(int mouseX, int mouseY);
	void pressCtrl(int key);
	void releaseCtrl();
	void mousePressEvent(QMouseEvent*, QWidget*);
	void mouseReleaseEvent(QMouseEvent*, QWidget*);
	void mouseMoveEvent(QMouseEvent*, QWidget*);
	void creatPrivewNode(string typeName,string nodeName);
	void setPrivewNodePosistion(int posX,int posY);
	void AddNode();
	void Addconnection(string relationCardinality);
	void clearButtonChecked();
	void moveComponent(int moveX,int moveY);
	void processRedoForGui();
	void processUndoForGui();
	void processDeleteComponetFromGui();
	bool canSave();
	bool canPaste();
	bool saveFileWithGUI(string filePath);
	bool saveXmlFile(string filePath);
	bool checkCanConnect();
	bool checkNeedInputRealtionCardinality();
	bool canInputComponentName(unsigned int index);
	bool canRedo();
	bool canUndo();
	bool isHaveSelect();
	string getConnectErrorMessage();
	Subject* getSubject();
	unsigned int getNumberOfComponent();
	bool editTextCmd (int index,string text);
	vector<string> getAllComponetNames();
	vector<string> getAllComponetType();
	vector<unsigned int> getAllComponentID();
	string getComponentName(unsigned int componentID);

	vector<unsigned int> getAllEntityID();
	vector<unsigned int> getAllEntityOfAttributeID(unsigned int entityID);
	bool isPrimaryKey(unsigned int entityID,unsigned int attributeID);
	vector<unsigned int> getForeginKeyIDs(unsigned int entityID);

	void setPrimaryKey(int pos_x,int pos_y);

	~PresentationModel();
	enum NEED_INPUT_STATE
	{
		SELECT_FUNCTION, LOAD_FILE_PATH, SAVE_FILE_PATH, ADD_NODE_TYPE, CONNECT_FIRST_NODE, NEED_ENTITY_ID, DELETE_NODE_ID, ADD_NODE_NAME, CONNECT_SECOND_NODE, CONNECT_CARDINALITY, NEED_PRIMARYKEY_ID
	};
	enum STATE
	{
		POINTER_STATE,ADD_NODE_STATE,CONNECT_STATE,SET_PRIMARY_KEY_STATE
	};
	const string& getShowMessage() const
	{
		return _ShowMessage;
	}
	void changeState(STATE state);

	bool isSetPrimaryKeyChecked() const
	{
		return _primaryKeyButtonChecked;
	}

	bool isAddAttributeButtonChecked() const
	{
		return _addAttributeButtonChecked;
	}

	bool isAddEntityButtonChecked() const
	{
		return _addEntityButtonChecked;
	}

	bool isAddRelationshipButtonChecked() const
	{
		return _addRelationshipButtonChecked;
	}

	bool isConnectButtonChecked() const
	{
		return _connectButtonChecked;
	}

	bool isPointerButtonChecked() const
	{
		return _pointerButtonChecked;
	}

	void setPrimaryKeyButtonChecked(bool primaryKeyChecked)
	{
		_primaryKeyButtonChecked = primaryKeyChecked;
	}

	void setAddAttributeButtonChecked(bool addAttributeButtonChecked)
	{
		_addAttributeButtonChecked = addAttributeButtonChecked;
	}

	void setAddEntityButtonChecked(bool addEntityButtonChecked)
	{
		_addEntityButtonChecked = addEntityButtonChecked;
	}

	void setAddRelationshipButtonChecked(bool addRelationshipButtonChecked)
	{
		_addRelationshipButtonChecked = addRelationshipButtonChecked;
	}

	void setConnectButtonChecked(bool connectButtonChecked)
	{
		_connectButtonChecked = connectButtonChecked;
	}

	void setPointerButtonChecked(bool pointerButtonChecked)
	{
		_pointerButtonChecked = pointerButtonChecked;
	}

	bool isTrackingMouse() const
	{
		return _trackingMouse;
	}

	void setTrackingMouse(bool trackingMouse)
	{
		_trackingMouse = trackingMouse;
		_ERModel->notify();
	}

	void setPrivewLineEndPoint(const QPoint& privewLineEndPoint)
	{
		_privewLineEndPoint = privewLineEndPoint;
		_ERModel->notify();
	}

	void setPrivewLineStartPoint(const QPoint& privewLineStartPoint)
	{
		_privewLineStartPoint = privewLineStartPoint;
	}

	void setNeedDrawPrivewLine(bool needDrawPrivewLine)
	{
		_needDrawPrivewLine = needDrawPrivewLine;
	}



private:
	NEED_INPUT_STATE _InputState;
	string _ShowMessage;
	ERModel* _ERModel;
	State* _state;
	vector<unsigned int> _ParameterCache;
	string _AddNodeType;
	QPoint _privewLineStartPoint;
	QPoint _privewLineEndPoint;
	bool _needDrawPrivewLine;
	bool _pointerButtonChecked;
	bool _connectButtonChecked;
	bool _addAttributeButtonChecked;
	bool _addEntityButtonChecked;
	bool _addRelationshipButtonChecked;
	bool _primaryKeyButtonChecked;
	bool _trackingMouse;
	void printCurrentDiagram();
	string getAllConnectionDetial();
	string getAllComponentDetial();
	void processSelection5previousFunction(unsigned int selectFunction);
	void processSelectionlastFunction(unsigned int selectedFunction);
	void processAddConnectInput(string input);
	void processAddNodeInput(string input);
	bool processSelectFunction(string input);
	void processInputConnectNodeID(unsigned int inputInt);
	void processSetPrimaryKey(string input);
	void processInputRelationCardinality(unsigned int inputInt);
	void showConnectErrorMessageAndRest(ERModel::CHECK_CONNECTION_STATS_CODE connectStats, unsigned int secondNodeID);
	void functionFinish();
	void processInputSecondConnectNode(unsigned int inputInt);
	void processInputNodeType(const string& input);
	void processUnDo();
	void processReDo();
	void processEnterEntityID(unsigned int inputInt);
	void processLoadFile(string input);
	void processSaveFile(string input);
	void processInputDeleteNodeID(string input);
	bool extrectInputValue(string input, list<unsigned int>& inputAttributeIDs);
	bool verifyInputAttributeIDOfEntity(unsigned int entityID, list<unsigned int>& inputAttributeIDs);
	void processInputPrimaryKey(const string& input);
	void showMessageWhenChooseSetPrimary();
	void showMessageWhenChooseDeleteComponent();
	void showMessageWhenChooseConnectNode();
	void iniButtonCheckState();

	enum FUNCTION_NAMES
	{
		LOADE_FILE = 1, SAVE_FILE, ADD_NODE, CONNECT_NODE, DISPLAY_DIAGRAM, SET_PRIMRYKEY, DISPLAY_TABLE, DELETE_COMPONENT, UNDO, REDO, EXIT
	};

};

#endif /* PRESENTATIONMODEL_H_ */
