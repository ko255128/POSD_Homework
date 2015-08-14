/*
 * ConnectComponentCommand.cpp
 *
 *  Created on: 2013/10/5
 *      Author: User
 */

#include "ConnectComponentCommand.h"


ConnectComponentCommand::ConnectComponentCommand(ERModel* model,unsigned int firstNodeID,unsigned int secondNodeID,Connection::REALTION_CARDINALITY relationCardinality)
{
	this->_Model = model;
	this->_FirstNodeID = firstNodeID;
	this->_SecondNodeID = secondNodeID;
	this->_RelationCardinality = relationCardinality;
	this->_UnDoAddConnectNode = NULL;
}

ConnectComponentCommand::~ConnectComponentCommand()
{
	if(_UnDoAddConnectNode!=NULL)
	{
		if(_Model->getComponentByID(_UnDoAddConnectNode->getId())==NULL)
			delete _UnDoAddConnectNode;
	}
}
//執行新增連線
void ConnectComponentCommand::execute()
{
	if(_UnDoAddConnectNode == NULL)
	    _Model->addConnection(_FirstNodeID,_SecondNodeID,_RelationCardinality);
	else
	{
		_Model->addComponent(_UnDoAddConnectNode);
		_Model->getComponentByID(_FirstNodeID)->connectTo(_UnDoAddConnectNode);
		_Model->getComponentByID(_SecondNodeID)->connectTo(_UnDoAddConnectNode);
	}
}
//反執行新增連線
void ConnectComponentCommand::unexecute()
{
	_UnDoAddConnectNode = _Model->deleteLastComponent();
}
