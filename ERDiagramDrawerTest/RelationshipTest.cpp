/*
 * RelationshipTest.cpp
 *
 *  Created on: 2013/10/21
 *      Author: User
 */

#include "RelationshipTest.h"
#include <vector>
#include <algorithm>
#include "Entity.h"
#include "Connection.h"
#include "Attribute.h"

using namespace std;

void RelationshipTest::SetUp()
{
    _relationship = new Relationship();
}

void RelationshipTest::TearDown()
{
	delete _relationship;
}

TEST_F(RelationshipTest,testRelationshipConstructor)
{
	unsigned int zero = 0;
	ASSERT_EQ(zero,_relationship->getId());
	ASSERT_STREQ("",_relationship->getName().c_str());
	ASSERT_STREQ("R",_relationship->getTypeSymbol().c_str());
	ASSERT_EQ(Component::RELATIONSHIP,_relationship->getType());
}

TEST_F(RelationshipTest,testCanConnectToATTRIBUTE)
{
	Attribute* connectTarget = new Attribute();
	bool actualy = _relationship->canConnectTo(connectTarget);
	ASSERT_TRUE(actualy);
	delete connectTarget;
}

TEST_F(RelationshipTest,testCanConnectToConnection)
{
	Connection* connectTarget = new Connection();
	bool actualy = _relationship->canConnectTo(connectTarget);
	ASSERT_FALSE(actualy);
	delete connectTarget;
}

TEST_F(RelationshipTest,testCanConnectToEntity)
{
	Entity* connectTarget = new Entity();
	bool actualy = _relationship->canConnectTo(connectTarget);
	ASSERT_TRUE(actualy);
	delete connectTarget;
}

TEST_F(RelationshipTest,testCanConnectToRelationship)
{
	Relationship* connectTarget = new Relationship();
	bool actualy = _relationship->canConnectTo(connectTarget);
	ASSERT_FALSE(actualy);
	delete connectTarget;
}

TEST_F(RelationshipTest,testDisConnectToConnectedNode)
{
	Connection* connectTarget = new Connection();
	_relationship->connectTo(connectTarget);
	vector<Component*> connections = _relationship->getConnectinons();
	ASSERT_TRUE(find(connections.begin(),connections.end(),connectTarget)!=connections.end());
	_relationship->disConnectTo(connectTarget);
	connections = _relationship->getConnectinons();
	ASSERT_TRUE(find(connections.begin(),connections.end(),connectTarget)== connections.end());
	delete connectTarget;
}

TEST_F(RelationshipTest,testDisConnectToNotConnectedNode)
{
	Connection* connectTarget = new Connection();
	vector<Component*> connections = _relationship->getConnectinons();
	ASSERT_TRUE(find(connections.begin(),connections.end(),connectTarget)==connections.end());
	_relationship->disConnectTo(connectTarget);
	connections = _relationship->getConnectinons();
	ASSERT_TRUE(find(connections.begin(),connections.end(),connectTarget)== connections.end());
	delete connectTarget;
}
