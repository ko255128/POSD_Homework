/*
 * CompomentFactory.cpp
 *
 *  Created on: 2013/9/17
 *      Author: User
 */

#include "CompomentFactory.h"
#include "Attribute.h"
#include "Entity.h"
#include "Relationship.h"
#include "Connection.h"

CompomentFactory::CompomentFactory()
{

}

CompomentFactory::~CompomentFactory()
{
}

//�ھګ��A�s�طs��Component
Component* CompomentFactory::createComponent(Component::COMPOMENT_TYPE componentType)
{
	if (componentType == Component::ATTRIBUTE)
		return new Attribute();
	else if (componentType == Component::ENTITY)
		return new Entity();
	else if (componentType == Component::RELATIONSHIP)
		return new Relationship();
	else if (componentType == Component::CONNECTION)
		return new Connection();
	else
		return NULL;
}

