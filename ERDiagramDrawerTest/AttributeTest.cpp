/*
 * AttributeTest.cpp
 *
 *  Created on: 2013/10/21
 *      Author: User
 */

#include <vector>
#include <algorithm>
#include "AttributeTest.h"
#include "Entity.h"
#include "Connection.h"
#include "Relationship.h"

using namespace std;

void AttributeTest::SetUp()
{
    _attribute = new Attribute();
}

void AttributeTest::TearDown()
{
	delete _attribute;
}

TEST_F(AttributeTest,testAttributeConstructor)
{
	unsigned int zero = 0;
	ASSERT_EQ(zero,_attribute->getId());
	ASSERT_STREQ("",_attribute->getName().c_str());
	ASSERT_STREQ("A",_attribute->getTypeSymbol().c_str());
	ASSERT_EQ(Component::ATTRIBUTE,_attribute->getType());
}

TEST_F(AttributeTest,testCanConnectToATTRIBUTE)
{
	Attribute* connectTarget = new Attribute();
	bool actualy = _attribute->canConnectTo(connectTarget);
	ASSERT_FALSE(actualy);
	delete connectTarget;
}

TEST_F(AttributeTest,testCanConnectToConnection)
{
	Connection* connectTarget = new Connection();
	bool actualy = _attribute->canConnectTo(connectTarget);
	ASSERT_FALSE(actualy);
	delete connectTarget;
}

TEST_F(AttributeTest,testCanConnectToEntity)
{
	Entity* connectTarget = new Entity();
	bool actualy = _attribute->canConnectTo(connectTarget);
	ASSERT_TRUE(actualy);
	delete connectTarget;
}

TEST_F(AttributeTest,testCanConnectToRelationship)
{
	Relationship* connectTarget = new Relationship();
	bool actualy = _attribute->canConnectTo(connectTarget);
	ASSERT_TRUE(actualy);
	delete connectTarget;
}

TEST_F(AttributeTest,testDisConnectToSucceed)
{
	Connection* connectTarget = new Connection();
	_attribute->connectTo(connectTarget);
	vector<Component*> connections = _attribute->getConnectinons();
	ASSERT_TRUE(find(connections.begin(),connections.end(),connectTarget)!=connections.end());
	_attribute->disConnectTo(connectTarget);
	connections = _attribute->getConnectinons();
	ASSERT_TRUE(find(connections.begin(),connections.end(),connectTarget)== connections.end());
	delete connectTarget;
}

TEST_F(AttributeTest,testDisConnectToNotConnectedNode)
{
	Connection* connectTarget = new Connection();
	vector<Component*> connections = _attribute->getConnectinons();
	ASSERT_TRUE(find(connections.begin(),connections.end(),connectTarget)==connections.end());
	_attribute->disConnectTo(connectTarget);
	connections = _attribute->getConnectinons();
	ASSERT_TRUE(find(connections.begin(),connections.end(),connectTarget)== connections.end());
	delete connectTarget;
}

