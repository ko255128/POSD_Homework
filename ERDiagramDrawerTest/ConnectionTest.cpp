/*
 * ConnectionTest.cpp
 *
 *  Created on: 2013/10/21
 *      Author: User
 */

#include <sstream>
#include <algorithm>
#include <iostream>
#include "ConnectionTest.h"
#include "Attribute.h"
#include "Relationship.h"
#include "Entity.h"

using namespace std;

void ConnectionTest::SetUp()
{
	_connection = new Connection();
}

void ConnectionTest::TearDown()
{
	delete _connection;
}

TEST_F(ConnectionTest,testContractor)
{
	const unsigned int zero = 0;
	ASSERT_EQ(zero, _connection->getId());
	ASSERT_EQ(Connection::NONE, _connection->getRealtionCardinality());
	ASSERT_EQ(Component::CONNECTION, _connection->getType());
	ASSERT_STREQ("", _connection->getName().c_str());
	ASSERT_STREQ("C", _connection->getTypeSymbol().c_str());
}

TEST_F(ConnectionTest,testCanConnectTo)
{
	Attribute* connectToAttribute = new Attribute();
	Entity* connectToEntity = new Entity();
	Relationship* connectToRelationship = new Relationship();
	Connection* connectToConnection = new Connection();
	ASSERT_FALSE(_connection->canConnectTo(connectToAttribute));
	ASSERT_FALSE(_connection->canConnectTo(connectToEntity));
	ASSERT_FALSE(_connection->canConnectTo(connectToRelationship));
	ASSERT_FALSE(_connection->canConnectTo(connectToConnection));
	delete connectToAttribute;
	delete connectToEntity;
	delete connectToRelationship;
	delete connectToConnection;
}

TEST_F(ConnectionTest,testSetRealtionCardinality)
{
	_connection->setRealtionCardinality(Connection::N);
	ASSERT_EQ(Connection::N, _connection->getRealtionCardinality());
	ASSERT_STREQ("N", _connection->getName().c_str());
	_connection->setRealtionCardinality(Connection::ONE);
	ASSERT_EQ(Connection::ONE, _connection->getRealtionCardinality());
	ASSERT_STREQ("1", _connection->getName().c_str());
	_connection->setRealtionCardinality(Connection::NONE);
	ASSERT_EQ(Connection::NONE, _connection->getRealtionCardinality());
	ASSERT_STREQ("", _connection->getName().c_str());
}

TEST_F(ConnectionTest,testGetAnthorConnectedNode)
{
	Entity* entity = new Entity();
	Attribute* connectAttribute = new Attribute();
	Attribute* notConnectAttribute = new Attribute();
	_connection->connectTo(entity);
	entity->connectTo(_connection);
	ASSERT_EQ(NULL, _connection->getAnthorConnectedNode(entity));
	_connection->connectTo(connectAttribute);
	connectAttribute->connectTo(_connection);
	vector<Component*> connections = _connection->getConnectinons();
	ASSERT_TRUE(find(connections.begin(), connections.end(), entity) != connections.end());
	ASSERT_TRUE(find(connections.begin(), connections.end(), connectAttribute) != connections.end());
	ASSERT_EQ(connectAttribute, _connection->getAnthorConnectedNode(entity));
	ASSERT_EQ(entity, _connection->getAnthorConnectedNode(connectAttribute));
	ASSERT_EQ(NULL, _connection->getAnthorConnectedNode(notConnectAttribute));
	delete notConnectAttribute;
	delete entity;
	delete connectAttribute;
}

TEST_F(ConnectionTest,testGetDetial)
{
	Entity* entity = new Entity();
	Attribute* connectAttribute = new Attribute();
	entity->setId(5);
	connectAttribute->setId(14);
	_connection->connectTo(entity);
	_connection->connectTo(connectAttribute);
	entity->connectTo(_connection);
	connectAttribute->connectTo(_connection);
	vector<Component*> connections = _connection->getConnectinons();
	ASSERT_TRUE(find(connections.begin(), connections.end(), entity) != connections.end());
	ASSERT_TRUE(find(connections.begin(), connections.end(), connectAttribute) != connections.end());
	stringstream stringStream;
	stringStream << setw(5) << 5 << "   ¢x" << setw(5) << 14 << "   ¢x";
	ASSERT_STREQ(stringStream.str().c_str(), _connection->getDetial().c_str());
	delete entity;
	delete connectAttribute;
}

