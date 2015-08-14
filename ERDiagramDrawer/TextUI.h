/*
 * TextUI.h
 *
 *  Created on: 2013/9/13
 *      Author: User
 */

#ifndef TEXTUI_H_
#define TEXTUI_H_

#include "PresentationModel.h"
#include "Connection.h"

#include <string>
#include <sstream>

using namespace std;
class TextUI
{
public:
	TextUI(PresentationModel* presentationModel);
	virtual ~TextUI();
	void displayMenu();
	bool processCommand();

private:
	PresentationModel* _presentationModel;
	void processAddNode();
	void processConnectNode();
	void processSetPrimaryKey();
	void processPrintTable();
	bool checkInputCurrentTypeSymbol(string& type);
	void printAllComponent();
	unsigned int getInputNodeID();
	void printConnectMessage(ERModel::CHECK_CONNECTION_STATS_CODE response);
	Connection::REALTION_CARDINALITY connectionInputProcess(unsigned int& firstNode, unsigned int& secondNode);
	void getInputRelationshipCardinality(unsigned int& selection);
	void printRealtionCardinalityMessage(Connection::REALTION_CARDINALITY realtionCardinality);
	void printAllConnectins();
	void printCurrentDiagram();
	void printAllEntity();
	void printAllAttributeOfEntity(unsigned int entityID);
	void getInputSelectedFunction(unsigned int& selectedFuntion);
	void printComponentHeader();
	void processDeleteNode();
	void processRedo();
	void processUndo();
	void processSaveFile();
	void processLoadFile();
	unsigned int getEntityByInputID();
	list<unsigned int> getInputAttributesID(unsigned int entityID);
	bool extrectInputValue(string input, list<unsigned int>& inputAttributeIDs);
	bool verifyInputAttributeIDOfEntity(unsigned int entityID, list<unsigned int>& inputAttributeIDs);
};
#endif /* TEXTUI_H_ */
