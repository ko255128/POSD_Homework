/*
 * DeleteCommand.cpp
 *
 *  Created on: 2013/10/5
 *      Author: User
 */

#include "DeleteCommand.h"
#include "Entity.h"
#include "Connection.h"

using namespace std;
//��M���v�T��EntityPrimaryKey(��R��Attribute�ɰ���)
void DeleteCommand::findAttributeEffectEntityPrimayKey(vector<Component*> connections, Component* deleteComponent)
{
	if (deleteComponent->getType() == Component::ATTRIBUTE)
	{
		vector<Component*> effectEntity;
		for (vector<Component*>::iterator vectorIterator = connections.begin(); vectorIterator != connections.end(); vectorIterator++)
		{
			Connection* connection = (Connection*) (*vectorIterator);
			Component* anthorConnectedNode = connection->getAnthorConnectedNode(deleteComponent);
			if (anthorConnectedNode->getType() == Component::ENTITY)
			{
				Entity* entity = (Entity*) (anthorConnectedNode);
				if (entity->isPrimaryKey(deleteComponent))
				{
					effectEntity.push_back(entity);
				}
			}
		}
		_EntityPrimaryKey.push_back(effectEntity);

	}
}

DeleteCommand::DeleteCommand(ERModel* model, vector<Component*> deleteComponents)
{
	this->_DeleteComponent = deleteComponents;
	this->_Model = model;
	for (unsigned int i = 0; i < deleteComponents.size(); i++)
	{
		Component* deleteComponent = deleteComponents.at(i);
		vector<Component*> connections = deleteComponent->getConnectinons();
		if (deleteComponent->getType() == Component::ATTRIBUTE)
			findAttributeEffectEntityPrimayKey(connections, deleteComponent);
		else if (deleteComponent->getType() == Component::CONNECTION)
			findConnectionEffectEntityPrimaryKey(connections, deleteComponent);
		else
		{
			vector<Component*> effectEntity;
			effectEntity.clear();
			_EntityPrimaryKey.push_back(effectEntity);
		}
	}

}

//��M���v�T��EntityPrimaryKey(��R��Connection�ɪ��ʧ@)
void DeleteCommand::findConnectionEffectEntityPrimaryKey(vector<Component*> connections, Component* deleteComponent)
{
	Connection* connection = (Connection*) (deleteComponent);
	vector<Component*> effectEntity;
	if (connections.back()->getType() == Component::ENTITY)
	{
		Entity* entity = (Entity*) (connections.back());
		if (entity->isPrimaryKey(connection->getAnthorConnectedNode(entity)))
			effectEntity.push_back(entity);
	}
	else if (connections.back()->getType() == Component::ATTRIBUTE)
	{
		Component* connectedNode = connection->getAnthorConnectedNode(connections.back());
		if (connectedNode->getType() == Component::ENTITY)
		{
			Entity* entity = (Entity*) (connectedNode);
			if (entity->isPrimaryKey(connections.back()))
				effectEntity.push_back(entity);
		}
	}
	_EntityPrimaryKey.push_back(effectEntity);
}

DeleteCommand::DeleteCommand(ERModel* model, Component* deleteComponent)
{
	this->_Model = model;
	this->_DeleteComponent.push_back(deleteComponent);
	vector<Component*> connections = deleteComponent->getConnectinons();
	if (deleteComponent->getType() == Component::ATTRIBUTE)
		findAttributeEffectEntityPrimayKey(connections, deleteComponent);
	else if (deleteComponent->getType() == Component::CONNECTION)
		findConnectionEffectEntityPrimaryKey(connections, deleteComponent);
	else
	{
		vector<Component*> effectEntity;
		effectEntity.clear();
		_EntityPrimaryKey.push_back(effectEntity);
	}
}

DeleteCommand::~DeleteCommand()
{
	_EntityPrimaryKey.clear();
	_DeleteComponent.clear();
}
//����R��
void DeleteCommand::execute()
{
	for(unsigned int index = 0 ; index < _DeleteComponent.size();index++)
	{
		if(_Model->checkNodeIsExist(_DeleteComponent.at(index)->getId()))
		{
			_Model->deleteNode(_DeleteComponent.at(index));
		}
		else
		{
			_EntityPrimaryKey.erase(_EntityPrimaryKey.begin() + index);
			_DeleteComponent.erase(_DeleteComponent.begin() + index);
			index--;
		}
	}
}
//�ϰ���R��
void DeleteCommand::unexecute()
{
	for (int index = _DeleteComponent.size()-1; index >= 0; index--)
	{
		_Model->undeleteNode(_DeleteComponent.at(index), _EntityPrimaryKey.at(index));
	}
}