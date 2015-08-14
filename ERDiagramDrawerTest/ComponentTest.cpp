/*
 * ComponentTest.cpp
 *
 *  Created on: 2013/10/22
 *      Author: User
 */

#include "ComponentTest.h"
#include "Entity.h"
#include "Attribute.h"
#include "Connection.h"

void ComponentTest::SetUp()
{
	_component = new Entity();
}

void ComponentTest::TearDown()
{
	delete _component;
}

TEST_F(ComponentTest,checkAlreadyConnect)
{
	Attribute* connectedAttribute = new Attribute();
	Connection* connection = new Connection();
	ASSERT_FALSE(_component->checkAlreadyConnect(connectedAttribute));
	connection->connectTo(_component);
	connection->connectTo(connectedAttribute);
	connectedAttribute->connectTo(connection);
	_component->connectTo(connection);
	ASSERT_TRUE(_component->checkAlreadyConnect(connectedAttribute));
}
