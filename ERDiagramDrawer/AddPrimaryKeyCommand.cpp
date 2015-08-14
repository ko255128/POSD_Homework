/*
 * AddPrimaryKeyCommand.cpp
 *
 *  Created on: 2013/10/5
 *      Author: User
 */

#include "AddPrimaryKeyCommand.h"

AddPrimaryKeyCommand::AddPrimaryKeyCommand(ERModel* model, unsigned int entityID, list<unsigned int> primaryKeyID)
{
	this->_Model = model;
	this->_EntityID = entityID;
	this->_NewPrimaryKeyID = primaryKeyID;
	//以下為取得舊有PrymaryKEY資料以便還原
	Component* entityComponent = model->getComponentByID(_EntityID);
	if(entityComponent->getType() == Component::ENTITY)
	{
		Entity* entity = (Entity*) entityComponent;
		_OldPrimaryKeys = entity->getPrimaryKey();
	}
}

AddPrimaryKeyCommand::~AddPrimaryKeyCommand()
{
}

void AddPrimaryKeyCommand::execute() //執行 設定新的PrimaryKEY
{
	_Model->setprimarykey(_EntityID,_NewPrimaryKeyID);
}

void AddPrimaryKeyCommand::unexecute()//執行設定舊的PrimaryKEY
{
	_Model->setprimarykey(_EntityID,_OldPrimaryKeys);
}
