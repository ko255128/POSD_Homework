/*
 * AddCommand.cpp
 *
 *  Created on: 2013/10/5
 *      Author: User
 */

#include "AddCommand.h"

AddCommand::~AddCommand()
{
	if (_UndoAddNode != NULL)
	{
		if (_Model->getComponentByID(_UndoAddNode->getId()) == NULL)
		{
			delete _UndoAddNode;
		}
	}
}

void AddCommand::execute() //執行新增
{
	if (_UndoAddNode == NULL)
	{
		_Model->addNode(_Type, _Name);
		_Model->setLastNodeLocate(_posX, _posY);
		_Model->setLastNodeSize(_width, _height);
	}
	else
	{
		_UndoAddNode->setSelected(false);
		_Model->addComponent(_UndoAddNode);
	}

}

AddCommand::AddCommand(ERModel* model, Component::COMPOMENT_TYPE type, const string name)
{
	this->_Type = type;
	this->_Name = name;
	this->_Model = model;
	this->_posX = 0;
	this->_posY = 0;
	this->_height = 0;
	this->_width = 0;
	this->_UndoAddNode = NULL;

}

void AddCommand::unexecute() //反執行等同刪除最後一個Component
{
	_UndoAddNode = _Model->deleteLastComponent();
}

AddCommand::AddCommand(ERModel* model, Component* component)
{
	this->_Type = component->getType();
	this->_Name = component->getName();
	this->_Model = model;
	this->_posX = component->getPosX();
	this->_posY = component->getPosY();
	this->_height = component->getHeight();
	this->_width = component->getWidth();
	this->_UndoAddNode = NULL;
}
