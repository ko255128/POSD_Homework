/*
 * EditTextCommand.cpp
 *
 *  Created on: 2013/11/29
 *      Author: User
 */

#include "EditTextCommand.h"

EditTextCommand::EditTextCommand(ERModel* model,unsigned int id,string oldText,string newText)
{
	this->_ERModel = model;
	this->_Id = id;
	this->_OldText = oldText;
	this->_NewText = newText;

}

EditTextCommand::~EditTextCommand()
{
}

void EditTextCommand::execute()
{
	_ERModel->setNodeName(_Id,_NewText);
}

void EditTextCommand::unexecute()
{
	_ERModel->setNodeName(_Id,_OldText);
}
