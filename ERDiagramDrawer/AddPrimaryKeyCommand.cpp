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
	//�H�U�����o�¦�PrymaryKEY��ƥH�K�٭�
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

void AddPrimaryKeyCommand::execute() //���� �]�w�s��PrimaryKEY
{
	_Model->setprimarykey(_EntityID,_NewPrimaryKeyID);
}

void AddPrimaryKeyCommand::unexecute()//����]�w�ª�PrimaryKEY
{
	_Model->setprimarykey(_EntityID,_OldPrimaryKeys);
}
