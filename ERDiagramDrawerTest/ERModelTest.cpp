/*
 * ERModelTest.cpp
 *
 *  Created on: 2013/10/22
 *      Author: User
 */

#include "ERModelTest.h"
#include <sys/stat.h>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "Compoment.h"

using namespace std;
void ERModelTest::creatTestFile()
{
	const string NEWLINE = "\n";
	struct stat stFileInfo;
	int intStat = stat("testdata", &stFileInfo);
	if (!(intStat == 0 && (stFileInfo.st_mode & S_IFDIR)))
		_mkdir("testdata");
	ofstream outputFilsStream("testdata/test_file1.erd", ios::out | ios::trunc);
	if (outputFilsStream.is_open())
	{
		outputFilsStream << "E, Engineer" << NEWLINE;
		outputFilsStream << "A, Emp_ID" << NEWLINE;
		outputFilsStream << "R, Has" << NEWLINE;
		outputFilsStream << "A, Name" << NEWLINE;
		outputFilsStream << "E, PC" << NEWLINE;
		outputFilsStream << "A, PC_ID" << NEWLINE;
		outputFilsStream << "A, Purchase_Date" << NEWLINE;
		outputFilsStream << "C" << NEWLINE;
		outputFilsStream << "C" << NEWLINE;
		outputFilsStream << "C" << NEWLINE;
		outputFilsStream << "C" << NEWLINE;
		outputFilsStream << "C, 1" << NEWLINE;
		outputFilsStream << "C, 1" << NEWLINE;
		outputFilsStream << "A, Department" << NEWLINE;
		outputFilsStream << "C" << NEWLINE;
		outputFilsStream << NEWLINE;
		outputFilsStream << "7 0,1" << NEWLINE;
		outputFilsStream << "8 0,3" << NEWLINE;
		outputFilsStream << "9 4,5" << NEWLINE;
		outputFilsStream << "10 4,6" << NEWLINE;
		outputFilsStream << "11 0,2" << NEWLINE;
		outputFilsStream << "12 2,4" << NEWLINE;
		outputFilsStream << "14 0,13" << NEWLINE;
		outputFilsStream << NEWLINE;
		outputFilsStream << "0 1,3" << NEWLINE;
		outputFilsStream << "4 5" << NEWLINE;
	}
	outputFilsStream.close();
}

void ERModelTest::SetUp()
{
	_model = new ERModel();
	creatTestFile();
}

void ERModelTest::TearDown()
{
	delete _model;
	remove("testdata/test_file1.erd");
	_rmdir("testdata");

}

TEST_F(ERModelTest,testAddNode)
{
	unsigned int ZERO = 0;
	unsigned int ONE = 1;
	unsigned int TWO = 2;
	unsigned int THREE = 3;
	unsigned int FOURE = 4;

	ASSERT_EQ(0, _model->addNode(Component::ATTRIBUTE, "attribute"));
	ASSERT_EQ(ONE, _model->getNextNodeId());
	ASSERT_EQ(1, _model->addNode(Component::ENTITY, "entity"));
	ASSERT_EQ(TWO, _model->getNextNodeId());
	ASSERT_EQ(2, _model->addNode(Component::RELATIONSHIP, "relationship"));
	ASSERT_EQ(THREE, _model->getNextNodeId());
	ASSERT_EQ(3, _model->addNode(Component::CONNECTION, ""));
	ASSERT_EQ(FOURE, _model->getNextNodeId());
	vector<Component*> components = _model->getComponents();

	ASSERT_EQ(FOURE, components.size());
	ASSERT_EQ(ZERO, components.at(0)->getId());
	ASSERT_EQ(Component::ATTRIBUTE, components.at(0)->getType());
	ASSERT_STREQ("attribute", components.at(0)->getName().c_str());
	ASSERT_STREQ("A", components.at(0)->getTypeSymbol().c_str());

	ASSERT_EQ(ONE, components.at(1)->getId());
	ASSERT_EQ(Component::ENTITY, components.at(1)->getType());
	ASSERT_STREQ("entity", components.at(1)->getName().c_str());
	ASSERT_STREQ("E", components.at(1)->getTypeSymbol().c_str());

	ASSERT_EQ(TWO, components.at(2)->getId());
	ASSERT_EQ(Component::RELATIONSHIP, components.at(2)->getType());
	ASSERT_STREQ("relationship", components.at(2)->getName().c_str());
	ASSERT_STREQ("R", components.at(2)->getTypeSymbol().c_str());

	ASSERT_EQ(THREE, components.at(3)->getId());
	ASSERT_EQ(Component::CONNECTION, components.at(3)->getType());
	ASSERT_STREQ("", components.at(3)->getName().c_str());
	ASSERT_STREQ("C", components.at(3)->getTypeSymbol().c_str());
}

TEST_F(ERModelTest,testCheckHasEntity)
{
	ASSERT_EQ(0, _model->addNode(Component::ATTRIBUTE, "attribute"));
	ASSERT_FALSE(_model->checkHaveAnyEntity());
	ASSERT_EQ(1, _model->addNode(Component::ENTITY, "entity"));
	ASSERT_TRUE(_model->checkHaveAnyEntity());
}

TEST_F(ERModelTest,testAddNodeByUI)
{
	unsigned int ZERO = 0;
	unsigned int ONE = 1;
	unsigned int TWO = 2;
	unsigned int THREE = 3;

	_model->addNodeByUI("Attribute", "attribute");
	ASSERT_EQ(ONE, _model->getNextNodeId());
	_model->addNodeByUI("Entity", "entity");
	ASSERT_EQ(TWO, _model->getNextNodeId());
	_model->addNodeByUI("Relation", "relationship");
	ASSERT_EQ(THREE, _model->getNextNodeId());
	vector<Component*> components = _model->getComponents();

	ASSERT_EQ(THREE, components.size());
	ASSERT_EQ(ZERO, components.at(0)->getId());
	ASSERT_EQ(Component::ATTRIBUTE, components.at(0)->getType());
	ASSERT_STREQ("attribute", components.at(0)->getName().c_str());
	ASSERT_STREQ("A", components.at(0)->getTypeSymbol().c_str());

	ASSERT_EQ(ONE, components.at(1)->getId());
	ASSERT_EQ(Component::ENTITY, components.at(1)->getType());
	ASSERT_STREQ("entity", components.at(1)->getName().c_str());
	ASSERT_STREQ("E", components.at(1)->getTypeSymbol().c_str());

	ASSERT_EQ(TWO, components.at(2)->getId());
	ASSERT_EQ(Component::RELATIONSHIP, components.at(2)->getType());
	ASSERT_STREQ("relationship", components.at(2)->getName().c_str());
	ASSERT_STREQ("R", components.at(2)->getTypeSymbol().c_str());
}

TEST_F(ERModelTest,testGetComponentByID)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	vector<Component*> components = _model->getComponents();
	Component* component = components.at(1);
	ASSERT_EQ(component, _model->getComponentByID(1));
	component->setId(53);
	ASSERT_EQ(component, _model->getComponentByID(53));
	ASSERT_EQ(NULL, _model->getComponentByID(1));
}

TEST_F(ERModelTest,testDeleteLastComonent)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	vector<Component*> components = _model->getComponents();
	Component* component = components.back();
	ASSERT_TRUE(find(components.begin(), components.end(), component) != components.end());
	_model->deleteLastComponent();
	components = _model->getComponents();
	ASSERT_FALSE(find(components.begin(), components.end(), component) != components.end());
}

TEST_F(ERModelTest,testSetNodeName)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	vector<Component*> components = _model->getComponents();
	Component* component = components.at(1);
	_model->setNodeName(1, "SetName");
	ASSERT_STREQ("SetName", component->getName().c_str());
	ASSERT_NO_FATAL_FAILURE(_model->setNodeName(1024, "SetName"));
}

TEST_F(ERModelTest,testAddConnectSuccedWithCarditionNone)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	_model->addConnection(0, 1, Connection::NONE);
	vector<Component*> components = _model->getComponents();
	Component* component = components.back();
	Component* connectedFirstNode = components.at(0);
	Component* connectedSecondNode = components.at(1);
	const unsigned int THREE = 3;
	const unsigned int FOURE = 4;
	ASSERT_EQ(FOURE, components.size());
	ASSERT_EQ(THREE, component->getId());
	ASSERT_EQ(Component::CONNECTION, component->getType());
	ASSERT_STREQ("", component->getName().c_str());
	ASSERT_STREQ("C", component->getTypeSymbol().c_str());
	ASSERT_EQ(Connection::NONE, ((Connection* )component)->getRealtionCardinality());
	ASSERT_TRUE(find(component->getConnectinons().begin(), component->getConnectinons().end(), connectedFirstNode) != component->getConnectinons().end());
	ASSERT_TRUE(find(component->getConnectinons().begin(), component->getConnectinons().end(), connectedSecondNode) != component->getConnectinons().end());
	ASSERT_TRUE(find(connectedFirstNode->getConnectinons().begin(), connectedFirstNode->getConnectinons().end(), component) != connectedFirstNode->getConnectinons().end());
	ASSERT_TRUE(find(connectedSecondNode->getConnectinons().begin(), connectedSecondNode->getConnectinons().end(), component) != connectedSecondNode->getConnectinons().end());
}

TEST_F(ERModelTest,testAddConnectSuccedWithCarditionOne)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	_model->addConnection(1, 2, Connection::ONE);
	vector<Component*> components = _model->getComponents();
	Component* component = components.back();
	Component* connectedFirstNode = components.at(1);
	Component* connectedSecondNode = components.at(2);
	const unsigned int THREE = 3;
	const unsigned int FOURE = 4;
	ASSERT_EQ(FOURE, components.size());
	ASSERT_EQ(THREE, component->getId());
	ASSERT_EQ(Component::CONNECTION, component->getType());
	ASSERT_STREQ("1", component->getName().c_str());
	ASSERT_STREQ("C", component->getTypeSymbol().c_str());
	ASSERT_EQ(Connection::ONE, ((Connection* )component)->getRealtionCardinality());
	ASSERT_TRUE(find(component->getConnectinons().begin(), component->getConnectinons().end(), connectedFirstNode) != component->getConnectinons().end());
	ASSERT_TRUE(find(component->getConnectinons().begin(), component->getConnectinons().end(), connectedSecondNode) != component->getConnectinons().end());
	ASSERT_TRUE(find(connectedFirstNode->getConnectinons().begin(), connectedFirstNode->getConnectinons().end(), component) != connectedFirstNode->getConnectinons().end());
	ASSERT_TRUE(find(connectedSecondNode->getConnectinons().begin(), connectedSecondNode->getConnectinons().end(), component) != connectedSecondNode->getConnectinons().end());
}

TEST_F(ERModelTest,testAddConnectSuccedWithCarditionN)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	_model->addConnection(1, 2, Connection::N);
	vector<Component*> components = _model->getComponents();
	Component* component = components.back();
	Component* connectedFirstNode = components.at(1);
	Component* connectedSecondNode = components.at(2);
	const unsigned int THREE = 3;
	const unsigned int FOURE = 4;
	ASSERT_EQ(FOURE, components.size());
	ASSERT_EQ(THREE, component->getId());
	ASSERT_EQ(Component::CONNECTION, component->getType());
	ASSERT_STREQ("N", component->getName().c_str());
	ASSERT_STREQ("C", component->getTypeSymbol().c_str());
	ASSERT_EQ(Connection::N, ((Connection* )component)->getRealtionCardinality());
	ASSERT_TRUE(find(component->getConnectinons().begin(), component->getConnectinons().end(), connectedFirstNode) != component->getConnectinons().end());
	ASSERT_TRUE(find(component->getConnectinons().begin(), component->getConnectinons().end(), connectedSecondNode) != component->getConnectinons().end());
	ASSERT_TRUE(find(connectedFirstNode->getConnectinons().begin(), connectedFirstNode->getConnectinons().end(), component) != connectedFirstNode->getConnectinons().end());
	ASSERT_TRUE(find(connectedSecondNode->getConnectinons().begin(), connectedSecondNode->getConnectinons().end(), component) != connectedSecondNode->getConnectinons().end());
}

TEST_F(ERModelTest,testAddConnectUISuccedWithCarditionNone)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	_model->addConnectByUI(0, 1, Connection::NONE);
	vector<Component*> components = _model->getComponents();
	Component* component = components.back();
	Component* connectedFirstNode = components.at(0);
	Component* connectedSecondNode = components.at(1);
	const unsigned int THREE = 3;
	const unsigned int FOURE = 4;
	ASSERT_EQ(FOURE, components.size());
	ASSERT_EQ(THREE, component->getId());
	ASSERT_EQ(Component::CONNECTION, component->getType());
	ASSERT_STREQ("", component->getName().c_str());
	ASSERT_STREQ("C", component->getTypeSymbol().c_str());
	ASSERT_EQ(Connection::NONE, ((Connection* )component)->getRealtionCardinality());
	ASSERT_TRUE(find(component->getConnectinons().begin(), component->getConnectinons().end(), connectedFirstNode) != component->getConnectinons().end());
	ASSERT_TRUE(find(component->getConnectinons().begin(), component->getConnectinons().end(), connectedSecondNode) != component->getConnectinons().end());
	ASSERT_TRUE(find(connectedFirstNode->getConnectinons().begin(), connectedFirstNode->getConnectinons().end(), component) != connectedFirstNode->getConnectinons().end());
	ASSERT_TRUE(find(connectedSecondNode->getConnectinons().begin(), connectedSecondNode->getConnectinons().end(), component) != connectedSecondNode->getConnectinons().end());
}

TEST_F(ERModelTest,testAddConnectUISuccedWithCarditionOne)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	_model->addConnectByUI(1, 2, Connection::ONE);
	vector<Component*> components = _model->getComponents();
	Component* component = components.back();
	Component* connectedFirstNode = components.at(1);
	Component* connectedSecondNode = components.at(2);
	const unsigned int THREE = 3;
	const unsigned int FOURE = 4;
	ASSERT_EQ(FOURE, components.size());
	ASSERT_EQ(THREE, component->getId());
	ASSERT_EQ(Component::CONNECTION, component->getType());
	ASSERT_STREQ("1", component->getName().c_str());
	ASSERT_STREQ("C", component->getTypeSymbol().c_str());
	ASSERT_EQ(Connection::ONE, ((Connection* )component)->getRealtionCardinality());
	ASSERT_TRUE(find(component->getConnectinons().begin(), component->getConnectinons().end(), connectedFirstNode) != component->getConnectinons().end());
	ASSERT_TRUE(find(component->getConnectinons().begin(), component->getConnectinons().end(), connectedSecondNode) != component->getConnectinons().end());
	ASSERT_TRUE(find(connectedFirstNode->getConnectinons().begin(), connectedFirstNode->getConnectinons().end(), component) != connectedFirstNode->getConnectinons().end());
	ASSERT_TRUE(find(connectedSecondNode->getConnectinons().begin(), connectedSecondNode->getConnectinons().end(), component) != connectedSecondNode->getConnectinons().end());
}

TEST_F(ERModelTest,testAddConnectUISuccedWithCarditionN)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	_model->addConnectByUI(1, 2, Connection::N);
	vector<Component*> components = _model->getComponents();
	Component* component = components.back();
	Component* connectedFirstNode = components.at(1);
	Component* connectedSecondNode = components.at(2);
	const unsigned int THREE = 3;
	const unsigned int FOURE = 4;
	ASSERT_EQ(FOURE, components.size());
	ASSERT_EQ(THREE, component->getId());
	ASSERT_EQ(Component::CONNECTION, component->getType());
	ASSERT_STREQ("N", component->getName().c_str());
	ASSERT_STREQ("C", component->getTypeSymbol().c_str());
	ASSERT_EQ(Connection::N, ((Connection* )component)->getRealtionCardinality());
	ASSERT_TRUE(find(component->getConnectinons().begin(), component->getConnectinons().end(), connectedFirstNode) != component->getConnectinons().end());
	ASSERT_TRUE(find(component->getConnectinons().begin(), component->getConnectinons().end(), connectedSecondNode) != component->getConnectinons().end());
	ASSERT_TRUE(find(connectedFirstNode->getConnectinons().begin(), connectedFirstNode->getConnectinons().end(), component) != connectedFirstNode->getConnectinons().end());
	ASSERT_TRUE(find(connectedSecondNode->getConnectinons().begin(), connectedSecondNode->getConnectinons().end(), component) != connectedSecondNode->getConnectinons().end());
}

TEST_F(ERModelTest,testAddConnectFailure)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	ASSERT_NO_FATAL_FAILURE(_model->addConnection(1, 3, Connection::ONE));
	const unsigned int THREE = 3;
	vector<Component*> components = _model->getComponents();
	ASSERT_EQ(THREE, components.size());
}

TEST_F(ERModelTest,testAddConnectUIFailure)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	ASSERT_NO_FATAL_FAILURE(_model->addConnectByUI(1, 3, Connection::ONE));
	const unsigned int THREE = 3;
	vector<Component*> components = _model->getComponents();
	ASSERT_EQ(THREE, components.size());
}

TEST_F(ERModelTest,testSetPrimaryKeyByIDList)
{
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Attribute", "attribute2");
	_model->addNodeByUI("Attribute", "attribute3");
	_model->addNodeByUI("Entity", "entity");
	list<unsigned int> primaryKeyids;
	primaryKeyids.push_back(1);
	primaryKeyids.push_back(2);
	Component* notPrimaryKey = _model->getComponentByID(0);
	Component* primaryKey1 = _model->getComponentByID(1);
	Component* primaryKey2 = _model->getComponentByID(2);
	Component* entityComponet = _model->getComponentByID(3);
	Entity* entity = (Entity*) entityComponet;
	_model->setprimarykey(3, primaryKeyids);
	const unsigned int TWO = 2;
	ASSERT_EQ(TWO, entity->getPrimaryKey().size());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), primaryKey1) != entity->getPrimaryKey().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), primaryKey2) != entity->getPrimaryKey().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), notPrimaryKey) == entity->getPrimaryKey().end());
}

TEST_F(ERModelTest,testSetPrimaryKeyByList)
{
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Attribute", "attribute2");
	_model->addNodeByUI("Attribute", "attribute3");
	_model->addNodeByUI("Entity", "entity");
	Component* notPrimaryKey = _model->getComponentByID(0);
	Component* primaryKey1 = _model->getComponentByID(1);
	Component* primaryKey2 = _model->getComponentByID(2);
	Component* entityComponet = _model->getComponentByID(3);
	vector<Component*> primaryKeys;
	primaryKeys.push_back(primaryKey1);
	primaryKeys.push_back(primaryKey2);
	Entity* entity = (Entity*) entityComponet;
	_model->setprimarykey(3, primaryKeys);
	const unsigned int TWO = 2;
	ASSERT_EQ(TWO, entity->getPrimaryKey().size());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), primaryKey1) != entity->getPrimaryKey().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), primaryKey2) != entity->getPrimaryKey().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), notPrimaryKey) == entity->getPrimaryKey().end());
}

TEST_F(ERModelTest,testSetPrimaryKeyByUI)
{
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Attribute", "attribute2");
	_model->addNodeByUI("Attribute", "attribute3");
	_model->addNodeByUI("Entity", "entity");
	list<unsigned int> primaryKeyids;
	primaryKeyids.push_back(1);
	primaryKeyids.push_back(2);
	Component* notPrimaryKey = _model->getComponentByID(0);
	Component* primaryKey1 = _model->getComponentByID(1);
	Component* primaryKey2 = _model->getComponentByID(2);
	Component* entityComponet = _model->getComponentByID(3);
	Entity* entity = (Entity*) entityComponet;
	_model->setPrimaryKeyByUI(3, primaryKeyids);
	const unsigned int TWO = 2;
	ASSERT_EQ(TWO, entity->getPrimaryKey().size());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), primaryKey1) != entity->getPrimaryKey().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), primaryKey2) != entity->getPrimaryKey().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), notPrimaryKey) == entity->getPrimaryKey().end());
}

TEST_F(ERModelTest,testDeleteNodeByID_ConnectionWithPrimaryKey)
{
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Entity", "entity");
	_model->addConnection(0, 1, Connection::NONE);
	Component* attribute = _model->getComponentByID(0);
	Component* entityComponent = _model->getComponentByID(1);
	Component* connection = _model->getComponentByID(2);
	Entity* entity = (Entity*) entityComponent;
	list<unsigned int> primaryKeyids;
	primaryKeyids.push_back(0);
	_model->setprimarykey(1, primaryKeyids);
	vector<Component*> components = _model->getComponents();
	const unsigned int THREE = 3;
	const unsigned int TWO = 2;
	ASSERT_EQ(THREE, components.size());
	_model->deleteNodeByID(2);
	components = _model->getComponents();
	ASSERT_EQ(TWO, components.size());
	ASSERT_TRUE(find(components.begin(), components.end(), connection) == components.end());
	ASSERT_TRUE(find(attribute->getConnectinons().begin(), attribute->getConnectinons().end(), connection) == attribute->getConnectinons().end());
	ASSERT_TRUE(find(entityComponent->getConnectinons().begin(), entityComponent->getConnectinons().end(), connection) == entityComponent->getConnectinons().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute) == entity->getPrimaryKey().end());
}

TEST_F(ERModelTest,testDeleteNodeByID_ConnectionWithPrimaryKey2)
{
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Entity", "entity");
	_model->addConnection(1, 0, Connection::NONE);
	Component* attribute = _model->getComponentByID(0);
	Component* entityComponent = _model->getComponentByID(1);
	Component* connection = _model->getComponentByID(2);
	Entity* entity = (Entity*) entityComponent;
	list<unsigned int> primaryKeyids;
	primaryKeyids.push_back(0);
	_model->setprimarykey(1, primaryKeyids);
	vector<Component*> components = _model->getComponents();
	const unsigned int THREE = 3;
	const unsigned int TWO = 2;
	ASSERT_EQ(THREE, components.size());
	_model->deleteNodeByID(2);
	components = _model->getComponents();
	ASSERT_EQ(TWO, components.size());
	ASSERT_TRUE(find(components.begin(), components.end(), connection) == components.end());
	ASSERT_TRUE(find(attribute->getConnectinons().begin(), attribute->getConnectinons().end(), connection) == attribute->getConnectinons().end());
	ASSERT_TRUE(find(entityComponent->getConnectinons().begin(), entityComponent->getConnectinons().end(), connection) == entityComponent->getConnectinons().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute) == entity->getPrimaryKey().end());
}

TEST_F(ERModelTest,testDeleteNodeByID_ENTITY)
{
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Entity", "entity");
	_model->addConnection(0, 1, Connection::NONE);
	Component* attribute = _model->getComponentByID(0);
	Component* entityComponent = _model->getComponentByID(1);
	Component* connection = _model->getComponentByID(2);
	vector<Component*> components = _model->getComponents();
	const unsigned int THREE = 3;
	const unsigned int ONE = 1;
	ASSERT_EQ(THREE, components.size());
	_model->deleteNodeByID(1);
	components = _model->getComponents();
	ASSERT_EQ(ONE, components.size());
	ASSERT_TRUE(find(components.begin(), components.end(), connection) == components.end());
	ASSERT_TRUE(find(components.begin(), components.end(), entityComponent) == components.end());
	ASSERT_TRUE(find(attribute->getConnectinons().begin(), attribute->getConnectinons().end(), connection) == attribute->getConnectinons().end());
}

TEST_F(ERModelTest,testDeleteNodeByID_ATTRIBUTE)
{
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Entity", "entity");
	_model->addConnection(0, 1, Connection::NONE);
	Component* attribute = _model->getComponentByID(0);
	Component* entityComponent = _model->getComponentByID(1);
	Component* connection = _model->getComponentByID(2);
	Entity* entity = (Entity*) entityComponent;
	list<unsigned int> primaryKeyids;
	primaryKeyids.push_back(0);
	_model->setprimarykey(1, primaryKeyids);
	vector<Component*> components = _model->getComponents();
	const unsigned int THREE = 3;
	const unsigned int ONE = 1;
	ASSERT_EQ(THREE, components.size());
	_model->deleteNodeByID(0);
	components = _model->getComponents();
	ASSERT_EQ(ONE, components.size());
	ASSERT_TRUE(find(components.begin(), components.end(), connection) == components.end());
	ASSERT_TRUE(find(components.begin(), components.end(), attribute) == components.end());
	ASSERT_TRUE(find(entityComponent->getConnectinons().begin(), entityComponent->getConnectinons().end(), connection) == entityComponent->getConnectinons().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute) == entity->getPrimaryKey().end());
}

TEST_F(ERModelTest,testDeleteNode_ConnectionWithPrimaryKey)
{
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Entity", "entity");
	_model->addConnection(0, 1, Connection::NONE);
	Component* attribute = _model->getComponentByID(0);
	Component* entityComponent = _model->getComponentByID(1);
	Component* connection = _model->getComponentByID(2);
	Entity* entity = (Entity*) entityComponent;
	list<unsigned int> primaryKeyids;
	primaryKeyids.push_back(0);
	_model->setprimarykey(1, primaryKeyids);
	vector<Component*> components = _model->getComponents();
	const unsigned int THREE = 3;
	const unsigned int TWO = 2;
	ASSERT_EQ(THREE, components.size());
	_model->deleteNode(connection);
	components = _model->getComponents();
	ASSERT_EQ(TWO, components.size());
	ASSERT_TRUE(find(components.begin(), components.end(), connection) == components.end());
	ASSERT_TRUE(find(attribute->getConnectinons().begin(), attribute->getConnectinons().end(), connection) == attribute->getConnectinons().end());
	ASSERT_TRUE(find(entityComponent->getConnectinons().begin(), entityComponent->getConnectinons().end(), connection) == entityComponent->getConnectinons().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute) == entity->getPrimaryKey().end());
}

TEST_F(ERModelTest,testDeleteNode_NODE)
{
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Entity", "entity");
	_model->addConnection(0, 1, Connection::NONE);
	Component* attribute = _model->getComponentByID(0);
	Component* entityComponent = _model->getComponentByID(1);
	Component* connection = _model->getComponentByID(2);
	vector<Component*> components = _model->getComponents();
	const unsigned int THREE = 3;
	const unsigned int ONE = 1;
	ASSERT_EQ(THREE, components.size());
	_model->deleteNode(entityComponent);
	components = _model->getComponents();
	ASSERT_EQ(ONE, components.size());
	ASSERT_TRUE(find(components.begin(), components.end(), connection) == components.end());
	ASSERT_TRUE(find(components.begin(), components.end(), entityComponent) == components.end());
	ASSERT_TRUE(find(attribute->getConnectinons().begin(), attribute->getConnectinons().end(), connection) == attribute->getConnectinons().end());
}

TEST_F(ERModelTest,testDeleteNode_NotExits)
{
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Entity", "entity");
	_model->addConnection(0, 1, Connection::NONE);
	Component* attribute = _model->getComponentByID(0);
	Component* entityComponent = _model->getComponentByID(1);
	Component* connection = _model->getComponentByID(2);
	vector<Component*> components = _model->getComponents();
	const unsigned int THREE = 3;
	ASSERT_EQ(THREE, components.size());
	ASSERT_NO_FATAL_FAILURE(_model->deleteNode(NULL));
	components = _model->getComponents();
	ASSERT_EQ(THREE, components.size());
	ASSERT_TRUE(find(components.begin(), components.end(), connection) != components.end());
	ASSERT_TRUE(find(components.begin(), components.end(), entityComponent) != components.end());
	ASSERT_TRUE(find(components.begin(), components.end(), attribute) != components.end());
	ASSERT_TRUE(find(attribute->getConnectinons().begin(), attribute->getConnectinons().end(), connection) != attribute->getConnectinons().end());
	ASSERT_TRUE(find(entityComponent->getConnectinons().begin(), entityComponent->getConnectinons().end(), connection) != entityComponent->getConnectinons().end());
}

TEST_F(ERModelTest,testDeleteNodeByID_NotExits)
{
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Entity", "entity");
	_model->addConnection(0, 1, Connection::NONE);
	Component* attribute = _model->getComponentByID(0);
	Component* entityComponent = _model->getComponentByID(1);
	Component* connection = _model->getComponentByID(2);
	vector<Component*> components = _model->getComponents();
	const unsigned int THREE = 3;
	ASSERT_EQ(THREE, components.size());
	ASSERT_NO_FATAL_FAILURE(_model->deleteNodeByID(1024));
	components = _model->getComponents();
	ASSERT_EQ(THREE, components.size());
	ASSERT_TRUE(find(components.begin(), components.end(), connection) != components.end());
	ASSERT_TRUE(find(components.begin(), components.end(), entityComponent) != components.end());
	ASSERT_TRUE(find(components.begin(), components.end(), attribute) != components.end());
	ASSERT_TRUE(find(attribute->getConnectinons().begin(), attribute->getConnectinons().end(), connection) != attribute->getConnectinons().end());
	ASSERT_TRUE(find(entityComponent->getConnectinons().begin(), entityComponent->getConnectinons().end(), connection) != entityComponent->getConnectinons().end());
}

TEST_F(ERModelTest,testSortComponent)
{
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Attribute", "attribute2");
	_model->addNodeByUI("Attribute", "attribute3");
	_model->addNodeByUI("Entity", "entity");
	vector<Component*> components = _model->getComponents();
	Component* attribute1 = components.at(0);
	Component* attribute2 = components.at(1);
	Component* attribute3 = components.at(2);
	Component* entity = components.at(3);
	attribute1->setId(90);
	attribute2->setId(53);
	attribute3->setId(60);
	entity->setId(2);
	_model->sortComponentsByID();
	components = _model->getComponents();
	ASSERT_EQ(entity, components.at(0));
	ASSERT_EQ(attribute2, components.at(1));
	ASSERT_EQ(attribute3, components.at(2));
	ASSERT_EQ(attribute1, components.at(3));
}

TEST_F(ERModelTest,UndeleteNode_connection)
{
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Entity", "entity");
	_model->addConnection(0, 1, Connection::NONE);
	Component* attribute = _model->getComponentByID(0);
	Component* entityComponent = _model->getComponentByID(1);
	Component* connection = _model->getComponentByID(2);
	Entity* entity = (Entity*) entityComponent;
	vector<Component*> primaryKeyids;
	vector<Component*> needRescurePrimaryKeyEntity;
	primaryKeyids.push_back(attribute);
	needRescurePrimaryKeyEntity.push_back(entity);
	_model->setprimarykey(1, primaryKeyids);
	vector<Component*> components = _model->getComponents();
	const unsigned int THREE = 3;
	const unsigned int TWO = 2;
	ASSERT_EQ(THREE, components.size());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute) != entity->getPrimaryKey().end());
	_model->deleteNode(connection);
	components = _model->getComponents();
	ASSERT_EQ(TWO, components.size());
	ASSERT_TRUE(find(components.begin(), components.end(), connection) == components.end());
	ASSERT_TRUE(find(attribute->getConnectinons().begin(), attribute->getConnectinons().end(), connection) == attribute->getConnectinons().end());
	ASSERT_TRUE(find(entityComponent->getConnectinons().begin(), entityComponent->getConnectinons().end(), connection) == entityComponent->getConnectinons().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute) == entity->getPrimaryKey().end());
	_model->undeleteNode(connection, needRescurePrimaryKeyEntity);
	components = _model->getComponents();
	ASSERT_EQ(THREE, components.size());
	ASSERT_TRUE(find(components.begin(), components.end(), connection) != components.end());
	ASSERT_TRUE(find(attribute->getConnectinons().begin(), attribute->getConnectinons().end(), connection) != attribute->getConnectinons().end());
	ASSERT_TRUE(find(entityComponent->getConnectinons().begin(), entityComponent->getConnectinons().end(), connection) != entityComponent->getConnectinons().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute) != entity->getPrimaryKey().end());
}

TEST_F(ERModelTest,UndeleteNode_NODE)
{
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Entity", "entity");
	_model->addConnection(0, 1, Connection::NONE);
	Component* attribute = _model->getComponentByID(0);
	Component* entityComponent = _model->getComponentByID(1);
	Component* connection = _model->getComponentByID(2);
	Entity* entity = (Entity*) entityComponent;
	vector<Component*> primaryKeyids;
	vector<Component*> needRescurePrimaryKeyEntity;
	primaryKeyids.push_back(attribute);
	needRescurePrimaryKeyEntity.push_back(entity);
	_model->setprimarykey(1, primaryKeyids);
	vector<Component*> components = _model->getComponents();
	const unsigned int THREE = 3;
	const unsigned int ONE = 1;
	ASSERT_EQ(THREE, components.size());
	_model->deleteNode(attribute);
	components = _model->getComponents();
	ASSERT_EQ(ONE, components.size());
	ASSERT_TRUE(find(components.begin(), components.end(), connection) == components.end());
	ASSERT_TRUE(find(components.begin(), components.end(), attribute) == components.end());
	ASSERT_TRUE(find(entityComponent->getConnectinons().begin(), entityComponent->getConnectinons().end(), connection) == entityComponent->getConnectinons().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute) == entity->getPrimaryKey().end());
	_model->undeleteNode(attribute, needRescurePrimaryKeyEntity);
	components = _model->getComponents();
	ASSERT_EQ(THREE, components.size());
	ASSERT_TRUE(find(components.begin(), components.end(), connection) != components.end());
	ASSERT_TRUE(find(components.begin(), components.end(), attribute) != components.end());
	ASSERT_TRUE(find(entityComponent->getConnectinons().begin(), entityComponent->getConnectinons().end(), connection) != entityComponent->getConnectinons().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute) != entity->getPrimaryKey().end());
}

TEST_F(ERModelTest,testUnDoAddNode)
{
	unsigned int ZERO = 0;
	unsigned int ONE = 1;
	unsigned int TWO = 2;

	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	ASSERT_TRUE(_model->undoCommand());
	ASSERT_EQ(TWO, _model->getNextNodeId());
	ASSERT_EQ(TWO, _model->getComponents().size());
	ASSERT_EQ(NULL, _model->getComponentByID(2));
	ASSERT_TRUE(_model->undoCommand());

	ASSERT_EQ(ONE, _model->getNextNodeId());
	ASSERT_EQ(ONE, _model->getComponents().size());
	ASSERT_EQ(NULL, _model->getComponentByID(1));
	ASSERT_TRUE(_model->undoCommand());

	ASSERT_EQ(ZERO, _model->getNextNodeId());
	ASSERT_EQ(ZERO, _model->getComponents().size());
	ASSERT_EQ(NULL, _model->getComponentByID(0));

	ASSERT_FALSE(_model->undoCommand());
}

TEST_F(ERModelTest,testReDoAddNode)
{
	unsigned int ZERO = 0;
	unsigned int ONE = 1;
	unsigned int TWO = 2;
	unsigned int THREE = 3;

	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	ASSERT_EQ(3, _model->getNextNodeId());
	ASSERT_TRUE(_model->undoCommand());
	ASSERT_TRUE(_model->undoCommand());
	ASSERT_TRUE(_model->undoCommand());

	ASSERT_FALSE(_model->undoCommand());

	ASSERT_TRUE(_model->redoCommand());
	ASSERT_EQ(ONE, _model->getNextNodeId());
	ASSERT_EQ(ONE, _model->getComponents().size());
	ASSERT_EQ(ZERO, _model->getComponentByID(0)->getId());
	ASSERT_EQ(Component::ATTRIBUTE, _model->getComponentByID(0)->getType());
	ASSERT_STREQ("attribute", _model->getComponentByID(0)->getName().c_str());
	ASSERT_STREQ("A", _model->getComponentByID(0)->getTypeSymbol().c_str());

	ASSERT_TRUE(_model->redoCommand());
	ASSERT_EQ(TWO, _model->getNextNodeId());
	ASSERT_EQ(TWO, _model->getComponents().size());
	ASSERT_EQ(ONE, _model->getComponentByID(1)->getId());
	ASSERT_EQ(Component::ENTITY, _model->getComponentByID(1)->getType());
	ASSERT_STREQ("entity", _model->getComponentByID(1)->getName().c_str());
	ASSERT_STREQ("E", _model->getComponentByID(1)->getTypeSymbol().c_str());

	ASSERT_TRUE(_model->redoCommand());
	ASSERT_EQ(THREE, _model->getNextNodeId());
	ASSERT_EQ(THREE, _model->getComponents().size());
	ASSERT_EQ(TWO, _model->getComponentByID(2)->getId());
	ASSERT_EQ(Component::RELATIONSHIP, _model->getComponentByID(2)->getType());
	ASSERT_STREQ("relationship", _model->getComponentByID(2)->getName().c_str());
	ASSERT_STREQ("R", _model->getComponentByID(2)->getTypeSymbol().c_str());

	ASSERT_FALSE(_model->redoCommand());
}

TEST_F(ERModelTest,testUnDoConnect)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	_model->addConnectByUI(0, 1, Connection::NONE);

	ASSERT_TRUE(_model->undoCommand());
	ASSERT_EQ(NULL, _model->getComponentByID(3));
}

TEST_F(ERModelTest,testReDoConnect)
{
	unsigned int THREE = 3;
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	_model->addConnectByUI(0, 1, Connection::NONE);

	ASSERT_TRUE(_model->undoCommand());

	ASSERT_TRUE(_model->redoCommand());
	Component* attribute = _model->getComponentByID(0);
	Component* entity = _model->getComponentByID(1);
	Connection* connection = (Connection*) _model->getComponentByID(3);
	ASSERT_EQ(THREE, connection->getId());
	ASSERT_EQ(Component::CONNECTION, connection->getType());
	ASSERT_STREQ("", connection->getName().c_str());
	ASSERT_STREQ("C", connection->getTypeSymbol().c_str());
	ASSERT_EQ(attribute, connection->getAnthorConnectedNode(entity));
	ASSERT_EQ(entity, connection->getAnthorConnectedNode(attribute));
	ASSERT_FALSE(_model->redoCommand());
}

TEST_F(ERModelTest,testUndoSetPrimaryKey)
{
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Attribute", "attribute2");
	_model->addNodeByUI("Attribute", "attribute3");
	_model->addNodeByUI("Entity", "entity");
	list<unsigned int> primaryKeyids;
	primaryKeyids.push_back(1);
	primaryKeyids.push_back(2);
	Component* attribute1 = _model->getComponentByID(0);
	Component* attribute2 = _model->getComponentByID(1);
	Component* attribute3 = _model->getComponentByID(2);
	Component* entityComponet = _model->getComponentByID(3);
	Entity* entity = (Entity*) entityComponet;
	_model->setPrimaryKeyByUI(3, primaryKeyids);
	primaryKeyids.clear();
	primaryKeyids.push_back(0);
	_model->setPrimaryKeyByUI(3, primaryKeyids);
	const unsigned int TWO = 2;
	const unsigned int ONE = 1;
	ASSERT_EQ(ONE, entity->getPrimaryKey().size());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute1) != entity->getPrimaryKey().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute2) == entity->getPrimaryKey().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute3) == entity->getPrimaryKey().end());
	ASSERT_TRUE(_model->undoCommand());
	ASSERT_EQ(TWO, entity->getPrimaryKey().size());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute1) == entity->getPrimaryKey().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute2) != entity->getPrimaryKey().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute3) != entity->getPrimaryKey().end());
	ASSERT_TRUE(_model->undoCommand());
	ASSERT_TRUE(entity->getPrimaryKey().empty());
}

TEST_F(ERModelTest,testRedoSetPrimaryKey)
{
	const unsigned int TWO = 2;
	const unsigned int ONE = 1;
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Attribute", "attribute2");
	_model->addNodeByUI("Attribute", "attribute3");
	_model->addNodeByUI("Entity", "entity");
	list<unsigned int> primaryKeyids;
	primaryKeyids.push_back(1);
	primaryKeyids.push_back(2);
	Component* attribute1 = _model->getComponentByID(0);
	Component* attribute2 = _model->getComponentByID(1);
	Component* attribute3 = _model->getComponentByID(2);
	Component* entityComponet = _model->getComponentByID(3);
	Entity* entity = (Entity*) entityComponet;
	_model->setPrimaryKeyByUI(3, primaryKeyids);
	primaryKeyids.clear();
	primaryKeyids.push_back(0);
	_model->setPrimaryKeyByUI(3, primaryKeyids);

	ASSERT_TRUE(_model->undoCommand());
	ASSERT_TRUE(_model->undoCommand());

	ASSERT_TRUE(_model->redoCommand());
	ASSERT_EQ(TWO, entity->getPrimaryKey().size());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute1) == entity->getPrimaryKey().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute2) != entity->getPrimaryKey().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute3) != entity->getPrimaryKey().end());

	ASSERT_TRUE(_model->redoCommand());
	ASSERT_EQ(ONE, entity->getPrimaryKey().size());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute1) != entity->getPrimaryKey().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute2) == entity->getPrimaryKey().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute3) == entity->getPrimaryKey().end());

	ASSERT_FALSE(_model->redoCommand());
}

TEST_F(ERModelTest,testUndoDelete)
{
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Entity", "entity");
	_model->addConnection(0, 1, Connection::NONE);
	Component* attribute = _model->getComponentByID(0);
	Component* entityComponent = _model->getComponentByID(1);
	Component* connection = _model->getComponentByID(2);
	Entity* entity = (Entity*) entityComponent;
	list<unsigned int> primaryKeyids;
	primaryKeyids.push_back(0);
	_model->setprimarykey(1, primaryKeyids);
	vector<Component*> components = _model->getComponents();
	const unsigned int THREE = 3;
	const unsigned int TWO = 2;
	ASSERT_EQ(THREE, components.size());
	_model->deleteNodeByID(2);
	components = _model->getComponents();
	ASSERT_EQ(TWO, components.size());
	ASSERT_TRUE(find(components.begin(), components.end(), connection) == components.end());
	ASSERT_TRUE(find(attribute->getConnectinons().begin(), attribute->getConnectinons().end(), connection) == attribute->getConnectinons().end());
	ASSERT_TRUE(find(entityComponent->getConnectinons().begin(), entityComponent->getConnectinons().end(), connection) == entityComponent->getConnectinons().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute) == entity->getPrimaryKey().end());
	ASSERT_TRUE(_model->undoCommand());
	components = _model->getComponents();
	ASSERT_EQ(THREE, components.size());
	ASSERT_TRUE(find(components.begin(), components.end(), connection) != components.end());
	ASSERT_TRUE(find(attribute->getConnectinons().begin(), attribute->getConnectinons().end(), connection) != attribute->getConnectinons().end());
	ASSERT_TRUE(find(entityComponent->getConnectinons().begin(), entityComponent->getConnectinons().end(), connection) != entityComponent->getConnectinons().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute) != entity->getPrimaryKey().end());
}

TEST_F(ERModelTest,testRedoDelete)
{
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Entity", "entity");
	_model->addConnection(0, 1, Connection::NONE);
	Component* attribute = _model->getComponentByID(0);
	Component* entityComponent = _model->getComponentByID(1);
	Component* connection = _model->getComponentByID(2);
	Entity* entity = (Entity*) entityComponent;
	list<unsigned int> primaryKeyids;
	primaryKeyids.push_back(0);
	_model->setprimarykey(1, primaryKeyids);
	vector<Component*> components = _model->getComponents();
	const unsigned int THREE = 3;
	const unsigned int TWO = 2;
	ASSERT_EQ(THREE, components.size());
	_model->deleteNodeByID(2);
	components = _model->getComponents();
	ASSERT_TRUE(_model->undoCommand());

	ASSERT_TRUE(_model->redoCommand());

	ASSERT_EQ(TWO, components.size());
	ASSERT_TRUE(find(components.begin(), components.end(), connection) == components.end());
	ASSERT_TRUE(find(attribute->getConnectinons().begin(), attribute->getConnectinons().end(), connection) == attribute->getConnectinons().end());
	ASSERT_TRUE(find(entityComponent->getConnectinons().begin(), entityComponent->getConnectinons().end(), connection) == entityComponent->getConnectinons().end());
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), attribute) == entity->getPrimaryKey().end());
	ASSERT_FALSE(_model->redoCommand());
}
TEST_F(ERModelTest,testGetAllComponentDetial)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	_model->addNodeByUI("Relation", "relationship2");
	_model->addConnectByUI(0, 1, Connection::NONE);
	_model->addConnectByUI(1, 2, Connection::ONE);
	_model->addConnectByUI(1, 3, Connection::N);
	const string TWO_BLANK_STRING = "  ";
	const string COMPONENT_SEPARATED = "  ¢x";
	const int TYPE_AND_ID_WIDTH = 4;
	stringstream stringStream;
	stringStream << setw(TYPE_AND_ID_WIDTH) << "A" << COMPONENT_SEPARATED << setw(TYPE_AND_ID_WIDTH) << 0 << COMPONENT_SEPARATED << TWO_BLANK_STRING << "attribute" << endl;
	stringStream << setw(TYPE_AND_ID_WIDTH) << "E" << COMPONENT_SEPARATED << setw(TYPE_AND_ID_WIDTH) << 1 << COMPONENT_SEPARATED << TWO_BLANK_STRING << "entity" << endl;
	stringStream << setw(TYPE_AND_ID_WIDTH) << "R" << COMPONENT_SEPARATED << setw(TYPE_AND_ID_WIDTH) << 2 << COMPONENT_SEPARATED << TWO_BLANK_STRING << "relationship" << endl;
	stringStream << setw(TYPE_AND_ID_WIDTH) << "R" << COMPONENT_SEPARATED << setw(TYPE_AND_ID_WIDTH) << 3 << COMPONENT_SEPARATED << TWO_BLANK_STRING << "relationship2" << endl;
	stringStream << setw(TYPE_AND_ID_WIDTH) << "C" << COMPONENT_SEPARATED << setw(TYPE_AND_ID_WIDTH) << 4 << COMPONENT_SEPARATED << TWO_BLANK_STRING << "" << endl;
	stringStream << setw(TYPE_AND_ID_WIDTH) << "C" << COMPONENT_SEPARATED << setw(TYPE_AND_ID_WIDTH) << 5 << COMPONENT_SEPARATED << TWO_BLANK_STRING << "1" << endl;
	stringStream << setw(TYPE_AND_ID_WIDTH) << "C" << COMPONENT_SEPARATED << setw(TYPE_AND_ID_WIDTH) << 6 << COMPONENT_SEPARATED << TWO_BLANK_STRING << "N" << endl;
	ASSERT_STREQ(stringStream.str().c_str(), _model->getAllComponentDetial().c_str());
}

TEST_F(ERModelTest,testGetAllEntityDetial)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	_model->addNodeByUI("Relation", "relationship2");
	_model->addConnectByUI(0, 1, Connection::NONE);
	_model->addConnectByUI(1, 2, Connection::ONE);
	_model->addConnectByUI(1, 3, Connection::N);
	const string TWO_BLANK_STRING = "  ";
	const string COMPONENT_SEPARATED = "  ¢x";
	const int TYPE_AND_ID_WIDTH = 4;
	stringstream stringStream;
	stringStream << setw(TYPE_AND_ID_WIDTH) << "E" << COMPONENT_SEPARATED << setw(TYPE_AND_ID_WIDTH) << 1 << COMPONENT_SEPARATED << TWO_BLANK_STRING << "entity" << endl;
	ASSERT_STREQ(stringStream.str().c_str(), _model->getAllEntityDetial().c_str());
}

TEST_F(ERModelTest,testGetAllConnectionsDetial)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	_model->addNodeByUI("Relation", "relationship2");
	_model->addConnectByUI(0, 1, Connection::NONE);
	_model->addConnectByUI(1, 2, Connection::ONE);
	_model->addConnectByUI(1, 3, Connection::N);
	const string CONNECTION_SEPARATED = "     ¢x";
	const int CONNECTION_WIDTH = 7;
	const string SEPARATED = "   ¢x";
	const int NODE_WIDTH = 5;
	stringstream stringStream;
	stringStream << setw(CONNECTION_WIDTH) << 4 << CONNECTION_SEPARATED << setw(NODE_WIDTH) << 0 << SEPARATED << setw(NODE_WIDTH) << 1 << SEPARATED << endl;
	stringStream << setw(CONNECTION_WIDTH) << 5 << CONNECTION_SEPARATED << setw(NODE_WIDTH) << 1 << SEPARATED << setw(NODE_WIDTH) << 2 << SEPARATED << endl;
	stringStream << setw(CONNECTION_WIDTH) << 6 << CONNECTION_SEPARATED << setw(NODE_WIDTH) << 1 << SEPARATED << setw(NODE_WIDTH) << 3 << SEPARATED << endl;
	ASSERT_STREQ(stringStream.str().c_str(), _model->getAllConnectionDetial().c_str());
}

TEST_F(ERModelTest_UndeleteNode_NODE_Test,testGetAllEntity)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	_model->addNodeByUI("Entity", "Entity2");
	_model->addConnectByUI(0, 1, Connection::NONE);
	_model->addConnectByUI(1, 2, Connection::ONE);
	vector<Component*> entitys = _model->getAllEntity();
	ASSERT_EQ((unsigned int )2, entitys.size());
	ASSERT_TRUE(find(entitys.begin(), entitys.end(), _model->getComponentByID(1)) != entitys.end());
	ASSERT_TRUE(find(entitys.begin(), entitys.end(), _model->getComponentByID(3)) != entitys.end());

	ASSERT_TRUE(find(entitys.begin(), entitys.end(), _model->getComponentByID(0)) == entitys.end());
	ASSERT_TRUE(find(entitys.begin(), entitys.end(), _model->getComponentByID(2)) == entitys.end());
	ASSERT_TRUE(find(entitys.begin(), entitys.end(), _model->getComponentByID(4)) == entitys.end());
	ASSERT_TRUE(find(entitys.begin(), entitys.end(), _model->getComponentByID(5)) == entitys.end());

	ASSERT_EQ(Component::ENTITY, entitys.at(0)->getType());
	ASSERT_EQ(Component::ENTITY, entitys.at(1)->getType());
	ASSERT_STREQ("entity", entitys.at(0)->getName().c_str());
	ASSERT_STREQ("Entity2", entitys.at(1)->getName().c_str());
}

TEST_F(ERModelTest,testgetAllAttributeDetialOfEntity)
{
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Attribute", "attribute2");
	_model->addNodeByUI("Attribute", "attribute3");
	_model->addNodeByUI("Entity", "entity");
	_model->addConnectByUI(1, 3, Connection::N);
	_model->addConnectByUI(2, 3, Connection::N);
	const string TWO_BLANK_STRING = "  ";
	const string COMPONENT_SEPARATED = "  ¢x";
	const int TYPE_AND_ID_WIDTH = 4;
	stringstream stringStream;
	stringStream << setw(TYPE_AND_ID_WIDTH) << "A" << COMPONENT_SEPARATED << setw(TYPE_AND_ID_WIDTH) << 1 << COMPONENT_SEPARATED << TWO_BLANK_STRING << "attribute2" << endl;
	stringStream << setw(TYPE_AND_ID_WIDTH) << "A" << COMPONENT_SEPARATED << setw(TYPE_AND_ID_WIDTH) << 2 << COMPONENT_SEPARATED << TWO_BLANK_STRING << "attribute3" << endl;
	ASSERT_STREQ(stringStream.str().c_str(), _model->getAllAttributeDetialOfEntity(3).c_str());
}

TEST_F(ERModelTest,testCheckEnoughNodeToConnect)
{
	ASSERT_FALSE(_model->checkEnoughNodeToConnect());
	_model->addNodeByUI("Attribute", "attribute1");
	ASSERT_FALSE(_model->checkEnoughNodeToConnect());
	_model->addNodeByUI("Attribute", "attribute2");
	ASSERT_TRUE(_model->checkEnoughNodeToConnect());
	_model->deleteNodeByID(0);
	ASSERT_FALSE(_model->checkEnoughNodeToConnect());
	_model->undoCommand();
	ASSERT_TRUE(_model->checkEnoughNodeToConnect());
	_model->redoCommand();
	ASSERT_FALSE(_model->checkEnoughNodeToConnect());
	_model->undoCommand();
	_model->undoCommand();
	_model->undoCommand();
	ASSERT_FALSE(_model->checkEnoughNodeToConnect());
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Entity", "attribute2");
	ASSERT_TRUE(_model->checkEnoughNodeToConnect());
}

TEST_F(ERModelTest,testCheckHaveAnyEntity)
{
	ASSERT_FALSE(_model->checkHaveAnyEntity());
	_model->addNodeByUI("Attribute", "attribute1");
	ASSERT_FALSE(_model->checkHaveAnyEntity());
	_model->addNodeByUI("Attribute", "attribute2");
	_model->addNodeByUI("Attribute", "attribute3");
	_model->addNodeByUI("Entity", "entity");
	ASSERT_TRUE(_model->checkHaveAnyEntity());
	_model->addNodeByUI("Entity", "entity2");
	ASSERT_TRUE(_model->checkHaveAnyEntity());
	_model->deleteNodeByID(4);
	ASSERT_TRUE(_model->checkHaveAnyEntity());
	_model->deleteNodeByID(3);
	ASSERT_FALSE(_model->checkHaveAnyEntity());
	_model->undoCommand();
	ASSERT_TRUE(_model->checkHaveAnyEntity());
}

TEST_F(ERModelTest,testCheckHaveAnyAttributeOfEntity)
{
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Attribute", "attribute2");
	_model->addNodeByUI("Attribute", "attribute3");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Entity", "entity2");
	ASSERT_FALSE(_model->checkHaveAnyAttributeOfEntity(3));
	ASSERT_FALSE(_model->checkHaveAnyAttributeOfEntity(4));
	_model->addConnectByUI(0, 3, Connection::NONE);
	_model->addConnectByUI(1, 4, Connection::NONE);
	_model->addConnectByUI(2, 4, Connection::NONE);
	ASSERT_TRUE(_model->checkHaveAnyAttributeOfEntity(3));
	ASSERT_TRUE(_model->checkHaveAnyAttributeOfEntity(4));
	_model->deleteNodeByID(5);
	ASSERT_FALSE(_model->checkHaveAnyAttributeOfEntity(3));
	_model->undoCommand();
	ASSERT_TRUE(_model->checkHaveAnyAttributeOfEntity(3));
	_model->deleteNodeByID(0);
	ASSERT_FALSE(_model->checkHaveAnyAttributeOfEntity(3));
}

TEST_F(ERModelTest,checkAttributeIsInEntity)
{
	_model->addNodeByUI("Attribute", "attribute1");
	_model->addNodeByUI("Attribute", "attribute2");
	_model->addNodeByUI("Attribute", "attribute3");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Entity", "entity2");
	ASSERT_FALSE(_model->checkAttributeIsInEntity(3, 0));
	ASSERT_FALSE(_model->checkAttributeIsInEntity(3, 1));
	ASSERT_FALSE(_model->checkAttributeIsInEntity(3, 2));
	ASSERT_FALSE(_model->checkAttributeIsInEntity(4, 0));
	ASSERT_FALSE(_model->checkAttributeIsInEntity(4, 1));
	ASSERT_FALSE(_model->checkAttributeIsInEntity(4, 2));
	_model->addConnectByUI(0, 3, Connection::NONE);
	_model->addConnectByUI(1, 3, Connection::NONE);
	_model->addConnectByUI(2, 4, Connection::NONE);
	ASSERT_TRUE(_model->checkAttributeIsInEntity(3, 0));
	ASSERT_TRUE(_model->checkAttributeIsInEntity(3, 1));
	ASSERT_TRUE(_model->checkAttributeIsInEntity(4, 2));
	_model->deleteNodeByID(5);
	ASSERT_FALSE(_model->checkAttributeIsInEntity(3, 0));
}

TEST_F(ERModelTest,testCheckNodeIsExits)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	_model->addNodeByUI("Entity", "Entity2");
	_model->addConnectByUI(0, 1, Connection::NONE);
	_model->addConnectByUI(1, 2, Connection::ONE);
	for (int i = 0; i < 6; i++)
		ASSERT_TRUE(_model->checkNodeIsExist(i));
	ASSERT_FALSE(_model->checkNodeIsExist(25));
	_model->deleteNodeByID(4);
	ASSERT_FALSE(_model->checkNodeIsExist(4));
	_model->undoCommand();
	ASSERT_TRUE(_model->checkNodeIsExist(4));
	_model->deleteNodeByID(1);
	ASSERT_FALSE(_model->checkNodeIsExist(1));
	ASSERT_FALSE(_model->checkNodeIsExist(4));
	ASSERT_FALSE(_model->checkNodeIsExist(5));
}

TEST_F(ERModelTest,testCheckNeedAddRealtionCardinality)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");

	ASSERT_FALSE(_model->checkNeedAddRealtionCardinality(0, 1));
	ASSERT_FALSE(_model->checkNeedAddRealtionCardinality(0, 2));
	ASSERT_FALSE(_model->checkNeedAddRealtionCardinality(1, 0));
	ASSERT_TRUE(_model->checkNeedAddRealtionCardinality(1, 2));
	ASSERT_TRUE(_model->checkNeedAddRealtionCardinality(2, 1));
	ASSERT_FALSE(_model->checkNeedAddRealtionCardinality(2, 0));
	ASSERT_FALSE(_model->checkNeedAddRealtionCardinality(5, 22));
}

TEST_F(ERModelTest,testCheckIsEntity)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	ASSERT_FALSE(_model->checkIsEntity(0));
	ASSERT_TRUE(_model->checkIsEntity(1));
	ASSERT_FALSE(_model->checkIsEntity(2));
	ASSERT_FALSE(_model->checkIsEntity(9999));
}

TEST_F(ERModelTest,testComponentIDCompare)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	Component* attribute = _model->getComponentByID(0);
	Component* entity = _model->getComponentByID(1);
	Component* relationship = _model->getComponentByID(2);
	ASSERT_TRUE(_model->componentIDCompare(attribute, entity));
	ASSERT_TRUE(_model->componentIDCompare(attribute, relationship));
	ASSERT_TRUE(_model->componentIDCompare(entity, relationship));
	ASSERT_FALSE(_model->componentIDCompare(entity, attribute));
	ASSERT_FALSE(_model->componentIDCompare(relationship, attribute));
	ASSERT_FALSE(_model->componentIDCompare(relationship, entity));
}

TEST_F(ERModelTest,testSaveFile)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Attribute", "attribute2");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	_model->addNodeByUI("Relation", "relationship2");
	_model->addConnectByUI(1, 2, Connection::NONE);
	_model->addConnectByUI(2, 3, Connection::ONE);
	_model->addConnectByUI(0, 2, Connection::ONE);
	_model->addConnectByUI(2, 4, Connection::N);

	list<unsigned int> primaryKeyIds;
	primaryKeyIds.push_back(0);
	primaryKeyIds.push_back(1);
	_model->setprimarykey(2, primaryKeyIds);

	ASSERT_FALSE(_model->saveFile("/IGUESSYOUATRENOTHAVETHISPATH/test.erd",false));
	ASSERT_TRUE(_model->saveFile("test.erd",false));

	ifstream inputFileStream("test.erd");
	ASSERT_TRUE(inputFileStream.is_open());
	string loadString;
	getline(inputFileStream, loadString);
	ASSERT_STREQ("A, attribute", loadString.c_str());
	getline(inputFileStream, loadString);
	ASSERT_STREQ("A, attribute2", loadString.c_str());
	getline(inputFileStream, loadString);
	ASSERT_STREQ("E, entity", loadString.c_str());
	getline(inputFileStream, loadString);
	ASSERT_STREQ("R, relationship", loadString.c_str());
	getline(inputFileStream, loadString);
	ASSERT_STREQ("R, relationship2", loadString.c_str());
	getline(inputFileStream, loadString);
	ASSERT_STREQ("C", loadString.c_str());
	getline(inputFileStream, loadString);
	ASSERT_STREQ("C, 1", loadString.c_str());
	getline(inputFileStream, loadString);
	ASSERT_STREQ("C, 1", loadString.c_str());
	getline(inputFileStream, loadString);
	ASSERT_STREQ("C, N", loadString.c_str());
	getline(inputFileStream, loadString);
	ASSERT_STREQ("", loadString.c_str());
	getline(inputFileStream, loadString);
	ASSERT_STREQ("5 1,2", loadString.c_str());
	getline(inputFileStream, loadString);
	ASSERT_STREQ("6 2,3", loadString.c_str());
	getline(inputFileStream, loadString);
	ASSERT_STREQ("7 0,2", loadString.c_str());
	getline(inputFileStream, loadString);
	ASSERT_STREQ("8 2,4", loadString.c_str());
	getline(inputFileStream, loadString);
	ASSERT_STREQ("", loadString.c_str());
	getline(inputFileStream, loadString);
	ASSERT_STREQ("2 0,1", loadString.c_str());
	getline(inputFileStream, loadString);
	ASSERT_STREQ("", loadString.c_str());
	ASSERT_TRUE(inputFileStream.eof());
	inputFileStream.close();
	remove("test.erd");
}

TEST_F(ERModelTest,testLoadFile)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	_model->addNodeByUI("Entity", "Entity2");
	_model->addConnectByUI(0, 1, Connection::NONE);
	_model->addConnectByUI(1, 2, Connection::ONE);
	ASSERT_FALSE(_model->loadFile("/IGUESSYOUATRENOTHAVETHISPATH/test.erd"));
	ASSERT_EQ((unsigned int )6, _model->getComponents().size());
	ASSERT_TRUE(_model->loadFile("testdata/test_file1.erd"));
	string names[] = { "Engineer", "Emp_ID", "Has", "Name", "PC", "PC_ID", "Purchase_Date", "", "", "", "", "1", "1", "Department", "" };
	string typeSymbol[] = { "E", "A", "R", "A", "E", "A", "A", "C", "C", "C", "C", "C", "C", "A", "C" };
	const unsigned int connectionsID[] = { 7, 8, 9, 10, 11, 12, 14 };
	const unsigned int connectionsFirstNodeID[] = { 0, 0, 4, 4, 0, 2, 0 };
	const unsigned int connectionsSecondNodeID[] = { 1, 3, 5, 6, 2, 4, 13 };
	const Connection::REALTION_CARDINALITY relationCardinaltitys[] = { Connection::NONE, Connection::NONE, Connection::NONE, Connection::NONE, Connection::ONE, Connection::ONE, Connection::NONE };
	Component::COMPOMENT_TYPE types[] = { Component::ENTITY, Component::ATTRIBUTE, Component::RELATIONSHIP, Component::ATTRIBUTE, Component::ENTITY, Component::ATTRIBUTE, Component::ATTRIBUTE, Component::CONNECTION, Component::CONNECTION, Component::CONNECTION, Component::CONNECTION, Component::CONNECTION, Component::CONNECTION, Component::ATTRIBUTE, Component::CONNECTION };
	vector<Component*> components = _model->getComponents();
	ASSERT_EQ((unsigned int )15, components.size());
	for (unsigned int i = 0; i < components.size(); i++)
	{
		ASSERT_EQ(i, components.at(i)->getId());
		ASSERT_EQ(types[i], components.at(i)->getType());
		ASSERT_STREQ(names[i].c_str(), components.at(i)->getName().c_str());
		ASSERT_STREQ(typeSymbol[i].c_str(), components.at(i)->getTypeSymbol().c_str());
	}

	for (unsigned int i = 0; i < 5; i++)
	{
		Component* connectionComponent = components.at(connectionsID[i]);
		ASSERT_EQ(Component::CONNECTION, connectionComponent->getType());
		Connection* connection = (Connection*) connectionComponent;
		Component* connectedFirstNode = components.at(connectionsFirstNodeID[i]);
		Component* connectedSecondNode = components.at(connectionsSecondNodeID[i]);
		ASSERT_EQ((unsigned int )2, connection->getConnectinons().size());
		ASSERT_EQ(connectedFirstNode, connection->getAnthorConnectedNode(connectedSecondNode));
		ASSERT_EQ(relationCardinaltitys[i], connection->getRealtionCardinality());
		ASSERT_EQ(connectedSecondNode, connection->getAnthorConnectedNode(connectedFirstNode));
		ASSERT_TRUE(find(connection->getConnectinons().begin(), connection->getConnectinons().end(), connectedFirstNode) != connection->getConnectinons().end());
		ASSERT_TRUE(find(connection->getConnectinons().begin(), connection->getConnectinons().end(), connectedSecondNode) != connection->getConnectinons().end());

		ASSERT_TRUE(find(connectedFirstNode->getConnectinons().begin(), connectedFirstNode->getConnectinons().end(), connection) != connectedFirstNode->getConnectinons().end());
		ASSERT_TRUE(find(connectedSecondNode->getConnectinons().begin(), connectedSecondNode->getConnectinons().end(), connection) != connectedSecondNode->getConnectinons().end());
	}
	Component* entityComponent = components.at(0);
	ASSERT_EQ(Component::ENTITY, entityComponent->getType());
	Entity* entity = (Entity*) entityComponent;
	ASSERT_EQ((unsigned int )2, entity->getPrimaryKey().size());
	Component* primaryKey = components.at(1);
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), primaryKey) != entity->getPrimaryKey().end());
	primaryKey = components.at(3);
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), primaryKey) != entity->getPrimaryKey().end());

	entityComponent = components.at(4);
	ASSERT_EQ(Component::ENTITY, entityComponent->getType());
	entity = (Entity*) entityComponent;
	ASSERT_EQ((unsigned int )1, entity->getPrimaryKey().size());
	primaryKey = components.at(5);
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), primaryKey) != entity->getPrimaryKey().end());
}

TEST_F(ERModelTest,testCheckCanConnect)
{
	_model->addNodeByUI("Attribute", "attribute");
	_model->addNodeByUI("Entity", "entity");
	_model->addNodeByUI("Relation", "relationship");
	_model->addNodeByUI("Attribute", "attribute2");
	_model->addNodeByUI("Entity", "entity2");
	_model->addNodeByUI("Relation", "relationship2");
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (i == j)
				ASSERT_EQ(ERModel::CONNECT_ITSELF, _model->checkCanConnect(i, j));
			else if (j == i + 3 || j == i - 3)
				ASSERT_EQ(ERModel::CAN_NOT_CONNECT, _model->checkCanConnect(i, j));
			else
				ASSERT_EQ(ERModel::CAN_CONNECT, _model->checkCanConnect(i, j));
		}
	}
	_model->addConnection(0, 1, Connection::NONE);
	for (int j = 0; j < 6; j++)
		ASSERT_EQ(ERModel::CAN_NOT_CONNECT, _model->checkCanConnect(j, 6));
	ASSERT_EQ(ERModel::ALREADY_CONNECTED, _model->checkCanConnect(0, 1));
	ASSERT_EQ(ERModel::ALREADY_CONNECTED, _model->checkCanConnect(1, 0));
	ASSERT_EQ(ERModel::CAN_NOT_CONNECT, _model->checkCanConnect(156, 99999));
}

TEST_F(ERModelTest,testCheckTranslateTable)
{
	_model->loadFile("testdata/test_file1.erd");
	stringstream stirngStream;
	stirngStream << "  Engineer¢x  PK(Emp_ID,Name),Department," << endl;
	stirngStream << "        PC¢x  PK(PC_ID),Purchase_Date,FK(Emp_ID,Name)," << endl;
	ASSERT_EQ(stirngStream.str().c_str(), _model->translateDiagramToTable());
}

TEST_F(ERModelTest,testloadComponentsByString_failureWithHadConnectionButNoConnectionPart)
{
	vector<string> componentString;
	vector<string> connectionString;
	vector<string> primaryKeyString;
	componentString.push_back("E, Engineer");
	componentString.push_back("A, Emp_ID");
	componentString.push_back("C");
	ASSERT_FALSE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));
}

TEST_F(ERModelTest,testloadComponentsByString_failureWithCompontPartFormatError)
{
	vector<string> componentString;
	vector<string> connectionString;
	vector<string> primaryKeyString;
	componentString.push_back("E Engineer");
	componentString.push_back("A Emp_ID");
	ASSERT_FALSE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));
	componentString.clear();
	componentString.push_back("H, Engineer");
	ASSERT_FALSE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));
	componentString.clear();
	componentString.push_back("E, Engineer");
	componentString.push_back("A, Emp_ID");
	componentString.push_back("C, A");
	ASSERT_FALSE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));

}

TEST_F(ERModelTest,testloadComponentsByString_failureWithConnectPartFormatError)
{
	vector<string> componentString;
	vector<string> connectionString;
	vector<string> primaryKeyString;
	componentString.push_back("E, Engineer");
	componentString.push_back("A, Emp_ID");
	componentString.push_back("R, Has");
	componentString.push_back("A, Name");
	componentString.push_back("E, PC");
	componentString.push_back("A, PC_ID");
	componentString.push_back("A, Purchase_Date");
	for (int i = 0; i < 4; i++)
		componentString.push_back("C");
	componentString.push_back("C, 1");
	componentString.push_back("C, N");

	connectionString.push_back("1,2,3");
	ASSERT_FALSE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));
	connectionString.pop_back();
	connectionString.push_back("1 2,3");
	ASSERT_FALSE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));
	connectionString.pop_back();
	connectionString.push_back("7 0,1");
	connectionString.push_back("8 0,1");
	ASSERT_FALSE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));
	connectionString.pop_back();
	connectionString.push_back("7 0,3");
	ASSERT_FALSE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));
	connectionString.pop_back();
	connectionString.push_back("8 0,A3");
	ASSERT_FALSE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));
	connectionString.pop_back();
	connectionString.push_back("8 0,999");
	ASSERT_FALSE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));
	connectionString.pop_back();
	connectionString.push_back("8 7,0");
	ASSERT_FALSE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));
	connectionString.pop_back();
	connectionString.push_back("8 0,0");
	ASSERT_FALSE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));
	connectionString.pop_back();
	connectionString.push_back("8 0,4");
	ASSERT_FALSE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));
	connectionString.pop_back();
	connectionString.push_back("8 1,3");
	ASSERT_FALSE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));
}

TEST_F(ERModelTest,testloadComponentsByString_failureWithPrimaryKeyFormatError)
{
	vector<string> componentString;
	vector<string> connectionString;
	vector<string> primaryKeyString;
	componentString.push_back("E, Engineer");
	componentString.push_back("A, Emp_ID");
	componentString.push_back("R, Has");
	componentString.push_back("A, Name");
	componentString.push_back("E, PC");
	componentString.push_back("A, PC_ID");
	componentString.push_back("A, Purchase_Date");
	for (int i = 0; i < 4; i++)
		componentString.push_back("C");
	componentString.push_back("C, 1");
	componentString.push_back("C, N");
	componentString.push_back("A, Department");
	componentString.push_back("C");

	connectionString.push_back("7 0,1");
	connectionString.push_back("8 0,3");
	connectionString.push_back("9 4,5");
	connectionString.push_back("10 4,6");
	connectionString.push_back("11 0,2");
	connectionString.push_back("12 2,4");
	connectionString.push_back("14 0,13");

	primaryKeyString.push_back("1,2,3");
	ASSERT_FALSE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));
	primaryKeyString.clear();
	primaryKeyString.push_back("1 2,3");
	ASSERT_FALSE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));
	primaryKeyString.clear();
	primaryKeyString.push_back("1 1A,3");
	ASSERT_FALSE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));
	primaryKeyString.clear();
	primaryKeyString.push_back("1 1,3,");
	ASSERT_FALSE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));
	primaryKeyString.clear();
	primaryKeyString.push_back("1 99,23");
	ASSERT_FALSE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));
	primaryKeyString.clear();
	primaryKeyString.push_back("99 1,3");
	ASSERT_FALSE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));
}

TEST_F(ERModelTest,testloadComponentByStringSucceed)
{
	vector<string> componentString;
	vector<string> connectionString;
	vector<string> primaryKeyString;
	componentString.push_back("E, Engineer");
	componentString.push_back("A, Emp_ID");
	componentString.push_back("R, Has");
	componentString.push_back("A, Name");
	componentString.push_back("E, PC");
	componentString.push_back("A, PC_ID");
	componentString.push_back("A, Purchase_Date");
	for (int i = 0; i < 4; i++)
		componentString.push_back("C");
	componentString.push_back("C, 1");
	componentString.push_back("C, N");
	componentString.push_back("A, Department");
	componentString.push_back("C");

	connectionString.push_back("7 0,1");
	connectionString.push_back("8 0,3");
	connectionString.push_back("9 4,5");
	connectionString.push_back("10 4,6");
	connectionString.push_back("11 0,2");
	connectionString.push_back("12 2,4");
	connectionString.push_back("14 0,13");

	primaryKeyString.push_back("0 1,3");
	primaryKeyString.push_back("4 5");
	ASSERT_TRUE(_model->loadComponentsByString(componentString, connectionString, primaryKeyString));

	string names[] = { "Engineer", "Emp_ID", "Has", "Name", "PC", "PC_ID", "Purchase_Date", "", "", "", "", "1", "N", "Department", "" };
	string typeSymbol[] = { "E", "A", "R", "A", "E", "A", "A", "C", "C", "C", "C", "C", "C", "A", "C" };
	const unsigned int connectionsID[] = { 7, 8, 9, 10, 11, 12, 14 };
	const unsigned int connectionsFirstNodeID[] = { 0, 0, 4, 4, 0, 2, 0 };
	const unsigned int connectionsSecondNodeID[] = { 1, 3, 5, 6, 2, 4, 13 };
	const Connection::REALTION_CARDINALITY relationCardinaltitys[] = { Connection::NONE, Connection::NONE, Connection::NONE, Connection::NONE, Connection::ONE, Connection::N, Connection::NONE };
	Component::COMPOMENT_TYPE types[] = { Component::ENTITY, Component::ATTRIBUTE, Component::RELATIONSHIP, Component::ATTRIBUTE, Component::ENTITY, Component::ATTRIBUTE, Component::ATTRIBUTE, Component::CONNECTION, Component::CONNECTION, Component::CONNECTION, Component::CONNECTION, Component::CONNECTION, Component::CONNECTION, Component::ATTRIBUTE, Component::CONNECTION };
	vector<Component*> components = _model->getComponents();
	ASSERT_EQ((unsigned int )15, components.size());
	for (unsigned int i = 0; i < components.size(); i++)
	{
		ASSERT_EQ(i, components.at(i)->getId());
		ASSERT_EQ(types[i], components.at(i)->getType());
		ASSERT_STREQ(names[i].c_str(), components.at(i)->getName().c_str());
		ASSERT_STREQ(typeSymbol[i].c_str(), components.at(i)->getTypeSymbol().c_str());
	}

	for (unsigned int i = 0; i < 5; i++)
	{
		Component* connectionComponent = components.at(connectionsID[i]);
		ASSERT_EQ(Component::CONNECTION, connectionComponent->getType());
		Connection* connection = (Connection*) connectionComponent;
		Component* connectedFirstNode = components.at(connectionsFirstNodeID[i]);
		Component* connectedSecondNode = components.at(connectionsSecondNodeID[i]);
		ASSERT_EQ((unsigned int )2, connection->getConnectinons().size());
		ASSERT_EQ(connectedFirstNode, connection->getAnthorConnectedNode(connectedSecondNode));
		ASSERT_EQ(relationCardinaltitys[i], connection->getRealtionCardinality());
		ASSERT_EQ(connectedSecondNode, connection->getAnthorConnectedNode(connectedFirstNode));
		ASSERT_TRUE(find(connection->getConnectinons().begin(), connection->getConnectinons().end(), connectedFirstNode) != connection->getConnectinons().end());
		ASSERT_TRUE(find(connection->getConnectinons().begin(), connection->getConnectinons().end(), connectedSecondNode) != connection->getConnectinons().end());

		ASSERT_TRUE(find(connectedFirstNode->getConnectinons().begin(), connectedFirstNode->getConnectinons().end(), connection) != connectedFirstNode->getConnectinons().end());
		ASSERT_TRUE(find(connectedSecondNode->getConnectinons().begin(), connectedSecondNode->getConnectinons().end(), connection) != connectedSecondNode->getConnectinons().end());
	}
	Component* entityComponent = components.at(0);
	ASSERT_EQ(Component::ENTITY, entityComponent->getType());
	Entity* entity = (Entity*) entityComponent;
	ASSERT_EQ((unsigned int )2, entity->getPrimaryKey().size());
	Component* primaryKey = components.at(1);
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), primaryKey) != entity->getPrimaryKey().end());
	primaryKey = components.at(3);
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), primaryKey) != entity->getPrimaryKey().end());

	entityComponent = components.at(4);
	ASSERT_EQ(Component::ENTITY, entityComponent->getType());
	entity = (Entity*) entityComponent;
	ASSERT_EQ((unsigned int )1, entity->getPrimaryKey().size());
	primaryKey = components.at(5);
	ASSERT_TRUE(find(entity->getPrimaryKey().begin(), entity->getPrimaryKey().end(), primaryKey) != entity->getPrimaryKey().end());
}
