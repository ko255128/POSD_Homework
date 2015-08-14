/*
 / t * Entity.cpp
 *
 *  Created on: 2013/9/17
 *      Author: User
 */

#include "Entity.h"
#include "Connection.h"
#include "Compoment.h"
#include "stringResource.h"
#include "Attribute.h"
#include <QtGui/qwidget.h>
#include <QtGui/QPen.h>
#include <QPainter>
#include "ComponentVisitor.h"

Entity::Entity()
{
	const int ENTITY_HEIGHT = 40;
	const int ENTITY_WIDTH = 100;
	const int ENTITY_LOCATE_X = 310;
	_posX = ENTITY_LOCATE_X;
	_height = ENTITY_HEIGHT;
	_width = ENTITY_WIDTH;
	_type = ENTITY;
}

Entity::Entity( Component* entity )
{
	this->_name = entity->getName();
	this->_type = Component::ENTITY;
	this->_height = entity->getHeight();
	this->_width = entity->getWidth();
	this->_posX = entity->getPosX() + pastPosOffset;
	this->_posY = entity->getPosY() + pastPosOffset;
	this->_id = 0;
	this->_isPreview = false;
	this->_selected = false;
	_connectinons.clear();
	_primaryKey.clear();
}

Entity::~Entity()
{
	_primaryKey.clear();
}

//取得該型態所代表字元
string Entity::getTypeSymbol()
{
	return ENTITY_TYPE_SYMBOL;
}

//回船是否能連接到目標
bool Entity::canConnectTo(Component* target)
{
	if (target->getType() == ENTITY || target->getType() == CONNECTION)
		return false;
	else
		return true;
}

//取得所有連接的Attribute的ID
vector<unsigned int> Entity::getAllAttributeID()
{
	vector<unsigned int> attributeIDs;
	for (vector<Component*>::iterator vectorIterator = _connectinons.begin(); vectorIterator != _connectinons.end(); vectorIterator++)
	{
		Connection* connection = (Connection*) *vectorIterator;
		Component* conntectedNode = connection->getAnthorConnectedNode(this);
		if (conntectedNode->getType() == ATTRIBUTE)
			attributeIDs.push_back(conntectedNode->getId());
	}
	return attributeIDs;
}
//加入PrimaryKey
void Entity::addPrimaryKey(Component* attribute)
{
	Attribute* primaryKeyAttribute = (Attribute*) attribute;
	primaryKeyAttribute->setIsPrimaryKey(true);
	_primaryKey.push_back(attribute);
}

//清空PrimaryKey
void Entity::clearPrimaryKey()
{
	while(!_primaryKey.empty())
	{
		Attribute* attribute = (Attribute*) _primaryKey.back();
		_primaryKey.pop_back();
		attribute->setIsPrimaryKey(false);
	}
}
//解除連接至target點
void Entity::disConnectTo(Component* target)
{
	//若連接的另外一點剛好是Primary Keys 會將他從PrimaryKEY中移除
	if (target->getType() == CONNECTION)
	{
		Connection* connection = (Connection*) target;
		Component* connectedPrimary = connection->getAnthorConnectedNode(this);
		for (vector<Component*>::iterator primaryIterator = _primaryKey.begin(); primaryIterator != _primaryKey.end(); primaryIterator++)
		{
			if (*primaryIterator == connectedPrimary)
			{
				Attribute* attribute = (Attribute*) connectedPrimary;
				attribute->setIsPrimaryKey(false);
				_primaryKey.erase(primaryIterator);
				break;
			}
		}
	}
	hookdisConnectTo(target); //主要解除連接動作
}
//回傳是否為PrimaryKey
bool Entity::isPrimaryKey(Component* target)
{
	for (vector<Component*>::iterator primaryIterator = _primaryKey.begin(); primaryIterator != _primaryKey.end(); primaryIterator++)
	{
		if (*primaryIterator == target)
		{
			return true;
		}
	}
	return false;

}

//取得該Entity所連接的點型態為Realthionship的Component
vector<Component*> Entity::getAllConnectRelation()
{
	vector<Component*> relations;
	for (vector<Component*>::iterator vectorIterator = _connectinons.begin(); vectorIterator != _connectinons.end(); vectorIterator++)
	{
		Connection* connection = (Connection*) *vectorIterator;
		Component* conntectedNode = connection->getAnthorConnectedNode(this);
		if (conntectedNode->getType() == RELATIONSHIP)
			relations.push_back(conntectedNode);
	}
	return relations;
}

void Entity::draw(QWidget* widget)
{
	QPainter painter(widget);
	const float HELF = 0.5;
	painter.setPen(Qt::black);
	if(_isPreview)
			painter.setPen(Qt::DashLine);
	painter.drawRoundRect(_posX, _posY, _width, _height);
	painter.drawText(_posX, _posY + _height * HELF, QString::fromStdString(getName()));
	if (_selected)
	{
		painter.setPen(Qt::green);
		painter.drawRoundRect(_posX - (outBoundOffset * HELF), _posY - (outBoundOffset * HELF), _width + outBoundOffset, _height + outBoundOffset);
	}
}

bool Entity::isCollision(int x, int y)
{
	return x >= _posX && x <= _posX + _width && y >= _posY && y <= _posY + _height;
}

list<unsigned int> Entity::getallPrimaryKeyID()
{
	list<unsigned int> allPrimaryKeyIDs;
	for (vector<Component*>::iterator vectorIterator = _primaryKey.begin(); vectorIterator != _primaryKey.end(); vectorIterator++)
	{
		allPrimaryKeyIDs.push_back((*vectorIterator)->getId());
	}
	return allPrimaryKeyIDs;
}

void Entity::accept( ComponentVisitor* visitor )
{
	visitor->visit(this);
}

Component* Entity::clone()
{
	return new Entity(this);
}
