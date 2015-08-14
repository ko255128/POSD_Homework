/*
 * EntityTest.cpp
 *
 *  Created on: 2013/10/21
 *      Author: User
 */

#include "EntityTest.h"
#include <vector>
#include <algorithm>
#include "Relationship.h"
#include "Connection.h"
#include "Attribute.h"

using namespace std;

void EntityTest::SetUp()
{
	_entity = new Entity();
}

void EntityTest::TearDown()
{
	delete _entity;
}

TEST_F(EntityTest,testRelationshipConstructor)
{
	unsigned int zero = 0;
	ASSERT_EQ(zero, _entity->getId());
	ASSERT_STREQ("", _entity->getName().c_str());
	ASSERT_STREQ("E", _entity->getTypeSymbol().c_str());
	ASSERT_EQ(Component::ENTITY, _entity->getType());
}

TEST_F(EntityTest,testCanConnectToATTRIBUTE)
{
	Attribute* connectTarget = new Attribute();
	bool actualy = _entity->canConnectTo(connectTarget);
	ASSERT_TRUE(actualy);
	delete connectTarget;
}

TEST_F(EntityTest,testCanConnectToConnection)
{
	Connection* connectTarget = new Connection();
	bool actualy = _entity->canConnectTo(connectTarget);
	ASSERT_FALSE(actualy);
	delete connectTarget;
}

TEST_F(EntityTest,testCanConnectToEntity)
{
	Entity* connectTarget = new Entity();
	bool actualy = _entity->canConnectTo(connectTarget);
	ASSERT_FALSE(actualy);
	delete connectTarget;
}

TEST_F(EntityTest,testCanConnectToRelationship)
{
	Relationship* connectTarget = new Relationship();
	bool actualy = _entity->canConnectTo(connectTarget);
	ASSERT_TRUE(actualy);
	delete connectTarget;
}

TEST_F(EntityTest,testDisConnectToConnectedNodeWithPrimaryKey)
{
	Connection* connectTarget = new Connection();
	Attribute* connectAttribute = new Attribute();
	_entity->connectTo(connectTarget);
	connectAttribute->connectTo(connectTarget);
	connectTarget->connectTo(connectAttribute);
	connectTarget->connectTo(_entity);
	_entity->addPrimaryKey(connectAttribute);
	vector<Component*> connections = _entity->getConnectinons();
	vector<Component*> primaryKeys = _entity->getPrimaryKey();
	ASSERT_TRUE(find(connections.begin(), connections.end(), connectTarget) != connections.end());
	ASSERT_TRUE(find(primaryKeys.begin(), primaryKeys.end(), connectAttribute) != primaryKeys.end());
	_entity->disConnectTo(connectTarget);
	connections = _entity->getConnectinons();
	primaryKeys = _entity->getPrimaryKey();
	ASSERT_TRUE(find(connections.begin(), connections.end(), connectTarget) == connections.end());
	ASSERT_TRUE(find(primaryKeys.begin(), primaryKeys.end(), connectAttribute) == primaryKeys.end());
	delete connectTarget;
	delete connectAttribute;
}

TEST_F(EntityTest,testDisConnectToConnectedNodeWithNotPrimaryKey)
{
	Connection* connectTarget = new Connection();
	Attribute* connectAttribute = new Attribute();
	_entity->connectTo(connectTarget);
	connectAttribute->connectTo(connectTarget);
	connectTarget->connectTo(connectAttribute);
	connectTarget->connectTo(_entity);
	vector<Component*> connections = _entity->getConnectinons();
	vector<Component*> primaryKeys = _entity->getPrimaryKey();
	ASSERT_TRUE(find(connections.begin(), connections.end(), connectTarget) != connections.end());
	ASSERT_TRUE(find(primaryKeys.begin(), primaryKeys.end(), connectAttribute) == primaryKeys.end());
	_entity->disConnectTo(connectTarget);
	connections = _entity->getConnectinons();
	primaryKeys = _entity->getPrimaryKey();
	ASSERT_TRUE(find(connections.begin(), connections.end(), connectTarget) == connections.end());
	ASSERT_TRUE(find(primaryKeys.begin(), primaryKeys.end(), connectAttribute) == primaryKeys.end());
	delete connectTarget;
	delete connectAttribute;
}

TEST_F(EntityTest,testDisConnectToNotConnectedNode)
{
	Connection* connectTarget = new Connection();
	vector<Component*> connections = _entity->getConnectinons();
	ASSERT_TRUE(find(connections.begin(), connections.end(), connectTarget) == connections.end());
	_entity->disConnectTo(connectTarget);
	connections = _entity->getConnectinons();
	ASSERT_TRUE(find(connections.begin(), connections.end(), connectTarget) == connections.end());
	delete connectTarget;
}

TEST_F(EntityTest,testClearPrimaryKey)
{
	Attribute* primaryKey_1 = new Attribute();
	Attribute* primaryKey_2 = new Attribute();
	_entity->addPrimaryKey(primaryKey_1);
	_entity->addPrimaryKey(primaryKey_2);
	vector<Component*> primaryKeys = _entity->getPrimaryKey();
	ASSERT_FALSE(primaryKeys.empty());
	ASSERT_TRUE(find(primaryKeys.begin(), primaryKeys.end(), primaryKey_1) != primaryKeys.end());
	ASSERT_TRUE(find(primaryKeys.begin(), primaryKeys.end(), primaryKey_2) != primaryKeys.end());
	_entity->clearPrimaryKey();
	primaryKeys = _entity->getPrimaryKey();
	ASSERT_TRUE(primaryKeys.empty());
	ASSERT_TRUE(find(primaryKeys.begin(), primaryKeys.end(), primaryKey_1) == primaryKeys.end());
	ASSERT_TRUE(find(primaryKeys.begin(), primaryKeys.end(), primaryKey_2) == primaryKeys.end());
	delete primaryKey_1;
	delete primaryKey_2;
}

TEST_F(EntityTest,testIsPrimaryKey)
{
	Attribute* primaryKey = new Attribute();
	Attribute* notPrimaryKey = new Attribute();
	_entity->addPrimaryKey(primaryKey);
	vector<Component*> primaryKeys = _entity->getPrimaryKey();
	ASSERT_FALSE(primaryKeys.empty());
	ASSERT_TRUE(find(primaryKeys.begin(), primaryKeys.end(), primaryKey) != primaryKeys.end());
	ASSERT_TRUE(find(primaryKeys.begin(), primaryKeys.end(), notPrimaryKey) == primaryKeys.end());
	ASSERT_TRUE(_entity->isPrimaryKey(primaryKey));
	ASSERT_FALSE(_entity->isPrimaryKey(notPrimaryKey));
	delete primaryKey;
	delete notPrimaryKey;
}

TEST_F(EntityTest,testGetAllConnectRelation)
{
	Connection connections[5];
	Attribute attribute[3];
	Relationship relationship[2];
	const unsigned int TWO = 2;
	for (int i = 0; i < 5; i++)
	{
		_entity->connectTo(&connections[i]);
		connections[i].connectTo(_entity);
		if (i < 3)
		{
			connections[i].connectTo(&attribute[i]);
			attribute[i].connectTo(&connections[i]);
		}
		else
		{
			connections[i].connectTo(&relationship[i - 3]);
			relationship[i - 3].connectTo(&connections[i]);
		}
	}
	vector<Component*> entityRelationships = _entity->getAllConnectRelation();
	ASSERT_EQ(TWO, entityRelationships.size());
	for (int i = 0; i < 5; i++)
	{
		ASSERT_TRUE(find(entityRelationships.begin(), entityRelationships.end(), &connections[i]) == entityRelationships.end());
		if (i < 3)
			ASSERT_TRUE(find(entityRelationships.begin(), entityRelationships.end(), &attribute[i]) == entityRelationships.end());
		else
			ASSERT_TRUE(find(entityRelationships.begin(), entityRelationships.end(), &relationship[i - 3]) != entityRelationships.end());
	}
}

TEST_F(EntityTest,testGetAttributeID)
{
	Connection connections[5];
	Attribute attribute[3];
	Relationship relationship[2];
	const unsigned int THREE = 3;
	for (int i = 0; i < 5; i++)
	{
		_entity->connectTo(&connections[i]);
		connections[i].connectTo(_entity);
		connections[i].setId(5 + i);
		if (i < 3)
		{
			connections[i].connectTo(&attribute[i]);
			attribute[i].connectTo(&connections[i]);
			attribute[i].setId(i);
		}
		else
		{
			connections[i].connectTo(&relationship[i - 3]);
			relationship[i - 3].connectTo(&connections[i]);
			relationship[i - 3].setId(i);
		}
	}
	vector<unsigned int> attributeIDs = _entity->getAllAttributeID();
	ASSERT_EQ(THREE, attributeIDs.size());
	for (unsigned int i = 0; i < 5; i++)
	{
		ASSERT_TRUE(find(attributeIDs.begin(), attributeIDs.end(), connections[i].getId()) == attributeIDs.end());
		if (i < 3)
			ASSERT_TRUE(find(attributeIDs.begin(), attributeIDs.end(), attribute[i].getId()) != attributeIDs.end());
		else
			ASSERT_TRUE(find(attributeIDs.begin(), attributeIDs.end(), relationship[i - 3].getId()) == attributeIDs.end());
	}
}
