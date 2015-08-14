/*
 * Integration.cpp
 *
 *  Created on: 2013/10/23
 *      Author: User
 */

#include "IntegrationTest.h"
#include <sys/stat.h>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "Compoment.h"

using namespace std;
void IntegrationTest::creatTestFile()
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

void IntegrationTest::SetUp()
{
	_model = new ERModel();
	creatTestFile();
}

void IntegrationTest::TearDown()
{
	delete _model;
	remove("testdata/test_file1.erd");
	_rmdir("testdata");
}

TEST_F(IntegrationTest,testLoadFileNotExist)
{
	ASSERT_FALSE(_model->loadFile("testdata/file_not_exist.erd"));
}

TEST_F(IntegrationTest,testIsPrimaryExist)
{
	ASSERT_TRUE(_model->loadFile("testdata/test_file1.erd"));
	stringstream stringStream;
	stringStream << "   E  ¢x   0  ¢x  Engineer" << endl;
	stringStream << "   A  ¢x   1  ¢x  Emp_ID" << endl;
	stringStream << "   R  ¢x   2  ¢x  Has" << endl;
	stringStream << "   A  ¢x   3  ¢x  Name" << endl;
	stringStream << "   E  ¢x   4  ¢x  PC" << endl;
	stringStream << "   A  ¢x   5  ¢x  PC_ID" << endl;
	stringStream << "   A  ¢x   6  ¢x  Purchase_Date" << endl;
	stringStream << "   C  ¢x   7  ¢x  " << endl;
	stringStream << "   C  ¢x   8  ¢x  " << endl;
	stringStream << "   C  ¢x   9  ¢x  " << endl;
	stringStream << "   C  ¢x  10  ¢x  " << endl;
	stringStream << "   C  ¢x  11  ¢x  1" << endl;
	stringStream << "   C  ¢x  12  ¢x  1" << endl;
	stringStream << "   A  ¢x  13  ¢x  Department" << endl;
	stringStream << "   C  ¢x  14  ¢x  " << endl;
	ASSERT_STREQ(stringStream.str().c_str(), _model->getAllComponentDetial().c_str());
	stringStream.clear();
	stringStream.str("");
	stringStream << "      7     ¢x    0   ¢x    1   ¢x" << endl;
	stringStream << "      8     ¢x    0   ¢x    3   ¢x" << endl;
	stringStream << "      9     ¢x    4   ¢x    5   ¢x" << endl;
	stringStream << "     10     ¢x    4   ¢x    6   ¢x" << endl;
	stringStream << "     11     ¢x    0   ¢x    2   ¢x" << endl;
	stringStream << "     12     ¢x    2   ¢x    4   ¢x" << endl;
	stringStream << "     14     ¢x    0   ¢x   13   ¢x" << endl;
	ASSERT_STREQ(stringStream.str().c_str(), _model->getAllConnectionDetial().c_str());
	stringStream.clear();
	stringStream.str("");
	stringStream << "  Engineer¢x  PK(Emp_ID,Name),Department," << endl;
	stringStream << "        PC¢x  PK(PC_ID),Purchase_Date,FK(Emp_ID,Name)," << endl;
	ASSERT_EQ(stringStream.str().c_str(), _model->translateDiagramToTable());
	Component* engineerComponet = _model->getComponentByID(0);
	Component* pcComponent = _model->getComponentByID(4);
	Component* name = _model->getComponentByID(3);
	Component* emp_ID = _model->getComponentByID(1);
	Component* pc_ID = _model->getComponentByID(5);
	Component* purchase_Data = _model->getComponentByID(6);
	ASSERT_EQ(Component::ENTITY, engineerComponet->getType());
	ASSERT_EQ(Component::ENTITY, pcComponent->getType());
	Entity* engineer = (Entity*) engineerComponet;
	Entity* pc = (Entity*) pcComponent;
	ASSERT_TRUE(find(engineer->getPrimaryKey().begin(), engineer->getPrimaryKey().end(), name) != engineer->getPrimaryKey().end());
	ASSERT_TRUE(find(engineer->getPrimaryKey().begin(), engineer->getPrimaryKey().end(), emp_ID) != engineer->getPrimaryKey().end());
	ASSERT_TRUE(find(pc->getPrimaryKey().begin(), pc->getPrimaryKey().end(), pc_ID) != pc->getPrimaryKey().end());
	ASSERT_FALSE(find(pc->getPrimaryKey().begin(), pc->getPrimaryKey().end(), purchase_Data) != pc->getPrimaryKey().end());
}

TEST_F(IntegrationTest,testUndoDeleteComponent)
{
	ASSERT_TRUE(_model->loadFile("testdata/test_file1.erd"));
	stringstream stringStream;
	stringStream << "   E  ¢x   0  ¢x  Engineer" << endl;
	stringStream << "   A  ¢x   1  ¢x  Emp_ID" << endl;
	stringStream << "   R  ¢x   2  ¢x  Has" << endl;
	stringStream << "   A  ¢x   3  ¢x  Name" << endl;
	stringStream << "   E  ¢x   4  ¢x  PC" << endl;
	stringStream << "   A  ¢x   5  ¢x  PC_ID" << endl;
	stringStream << "   A  ¢x   6  ¢x  Purchase_Date" << endl;
	stringStream << "   C  ¢x   7  ¢x  " << endl;
	stringStream << "   C  ¢x   8  ¢x  " << endl;
	stringStream << "   C  ¢x   9  ¢x  " << endl;
	stringStream << "   C  ¢x  10  ¢x  " << endl;
	stringStream << "   C  ¢x  11  ¢x  1" << endl;
	stringStream << "   C  ¢x  12  ¢x  1" << endl;
	stringStream << "   A  ¢x  13  ¢x  Department" << endl;
	stringStream << "   C  ¢x  14  ¢x  " << endl;
	ASSERT_STREQ(stringStream.str().c_str(), _model->getAllComponentDetial().c_str());
	stringStream.clear();
	stringStream.str("");
	stringStream << "      7     ¢x    0   ¢x    1   ¢x" << endl;
	stringStream << "      8     ¢x    0   ¢x    3   ¢x" << endl;
	stringStream << "      9     ¢x    4   ¢x    5   ¢x" << endl;
	stringStream << "     10     ¢x    4   ¢x    6   ¢x" << endl;
	stringStream << "     11     ¢x    0   ¢x    2   ¢x" << endl;
	stringStream << "     12     ¢x    2   ¢x    4   ¢x" << endl;
	stringStream << "     14     ¢x    0   ¢x   13   ¢x" << endl;
	ASSERT_STREQ(stringStream.str().c_str(), _model->getAllConnectionDetial().c_str());
	_model->addNodeByUI("Entity", "Test");
	Component* testEntity = _model->getComponentByID(15);
	ASSERT_TRUE(testEntity!=NULL);
	ASSERT_EQ(Component::ENTITY, testEntity->getType());
	ASSERT_STREQ("Test", testEntity->getName().c_str());
	_model->deleteNodeByID(15);
	ASSERT_EQ(NULL, _model->getComponentByID(15));
	_model->undoCommand();
	ASSERT_TRUE(testEntity!=NULL);
	ASSERT_EQ(Component::ENTITY, testEntity->getType());
	ASSERT_STREQ("Test", testEntity->getName().c_str());
}

TEST_F(IntegrationTest,testRedoConnectComponent)
{
	ASSERT_TRUE(_model->loadFile("testdata/test_file1.erd"));
	stringstream stringStream;
	stringStream << "   E  ¢x   0  ¢x  Engineer" << endl;
	stringStream << "   A  ¢x   1  ¢x  Emp_ID" << endl;
	stringStream << "   R  ¢x   2  ¢x  Has" << endl;
	stringStream << "   A  ¢x   3  ¢x  Name" << endl;
	stringStream << "   E  ¢x   4  ¢x  PC" << endl;
	stringStream << "   A  ¢x   5  ¢x  PC_ID" << endl;
	stringStream << "   A  ¢x   6  ¢x  Purchase_Date" << endl;
	stringStream << "   C  ¢x   7  ¢x  " << endl;
	stringStream << "   C  ¢x   8  ¢x  " << endl;
	stringStream << "   C  ¢x   9  ¢x  " << endl;
	stringStream << "   C  ¢x  10  ¢x  " << endl;
	stringStream << "   C  ¢x  11  ¢x  1" << endl;
	stringStream << "   C  ¢x  12  ¢x  1" << endl;
	stringStream << "   A  ¢x  13  ¢x  Department" << endl;
	stringStream << "   C  ¢x  14  ¢x  " << endl;
	ASSERT_STREQ(stringStream.str().c_str(), _model->getAllComponentDetial().c_str());
	stringStream.clear();
	stringStream.str("");
	stringStream << "      7     ¢x    0   ¢x    1   ¢x" << endl;
	stringStream << "      8     ¢x    0   ¢x    3   ¢x" << endl;
	stringStream << "      9     ¢x    4   ¢x    5   ¢x" << endl;
	stringStream << "     10     ¢x    4   ¢x    6   ¢x" << endl;
	stringStream << "     11     ¢x    0   ¢x    2   ¢x" << endl;
	stringStream << "     12     ¢x    2   ¢x    4   ¢x" << endl;
	stringStream << "     14     ¢x    0   ¢x   13   ¢x" << endl;
	ASSERT_STREQ(stringStream.str().c_str(), _model->getAllConnectionDetial().c_str());
	_model->addNodeByUI("Entity", "Test");
	Component* testEntity = _model->getComponentByID(15);
	ASSERT_TRUE(testEntity!=NULL);
	ASSERT_EQ(Component::ENTITY, testEntity->getType());
	ASSERT_STREQ("Test", testEntity->getName().c_str());
	_model->addNodeByUI("Attribute", "Test Attr");
	Component* testAttribute = _model->getComponentByID(16);
	ASSERT_TRUE(testAttribute!=NULL);
	ASSERT_EQ(Component::ATTRIBUTE, testAttribute->getType());
	ASSERT_STREQ("Test Attr", testAttribute->getName().c_str());
	_model->addConnectByUI(15, 16, Connection::NONE);
	Component* connection = _model->getComponentByID(17);
	ASSERT_EQ(Component::CONNECTION, connection->getType());
	ASSERT_EQ(testEntity, ((Connection* )connection)->getAnthorConnectedNode(testAttribute));
	ASSERT_EQ(testAttribute, ((Connection* )connection)->getAnthorConnectedNode(testEntity));
	ASSERT_TRUE(testEntity->checkAlreadyConnect(testAttribute));
	ASSERT_TRUE(testAttribute->checkAlreadyConnect(testEntity));
	_model->undoCommand();
	ASSERT_EQ(NULL, _model->getComponentByID(17));
	ASSERT_FALSE(testEntity->checkAlreadyConnect(testAttribute));
	ASSERT_FALSE(testAttribute->checkAlreadyConnect(testEntity));
	_model->redoCommand();
	ASSERT_EQ(Component::CONNECTION, connection->getType());
	ASSERT_EQ(testEntity, ((Connection* )connection)->getAnthorConnectedNode(testAttribute));
	ASSERT_EQ(testAttribute, ((Connection* )connection)->getAnthorConnectedNode(testEntity));
	ASSERT_TRUE(testEntity->checkAlreadyConnect(testAttribute));
	ASSERT_TRUE(testAttribute->checkAlreadyConnect(testEntity));
}

TEST_F(IntegrationTest,testCommonUsage)
{
	ASSERT_TRUE(_model->loadFile("testdata/test_file1.erd"));
	stringstream stringStream;
	stringStream << "   E  ¢x   0  ¢x  Engineer" << endl;
	stringStream << "   A  ¢x   1  ¢x  Emp_ID" << endl;
	stringStream << "   R  ¢x   2  ¢x  Has" << endl;
	stringStream << "   A  ¢x   3  ¢x  Name" << endl;
	stringStream << "   E  ¢x   4  ¢x  PC" << endl;
	stringStream << "   A  ¢x   5  ¢x  PC_ID" << endl;
	stringStream << "   A  ¢x   6  ¢x  Purchase_Date" << endl;
	stringStream << "   C  ¢x   7  ¢x  " << endl;
	stringStream << "   C  ¢x   8  ¢x  " << endl;
	stringStream << "   C  ¢x   9  ¢x  " << endl;
	stringStream << "   C  ¢x  10  ¢x  " << endl;
	stringStream << "   C  ¢x  11  ¢x  1" << endl;
	stringStream << "   C  ¢x  12  ¢x  1" << endl;
	stringStream << "   A  ¢x  13  ¢x  Department" << endl;
	stringStream << "   C  ¢x  14  ¢x  " << endl;
	ASSERT_STREQ(stringStream.str().c_str(), _model->getAllComponentDetial().c_str());
	stringStream.clear();
	stringStream.str("");
	stringStream << "      7     ¢x    0   ¢x    1   ¢x" << endl;
	stringStream << "      8     ¢x    0   ¢x    3   ¢x" << endl;
	stringStream << "      9     ¢x    4   ¢x    5   ¢x" << endl;
	stringStream << "     10     ¢x    4   ¢x    6   ¢x" << endl;
	stringStream << "     11     ¢x    0   ¢x    2   ¢x" << endl;
	stringStream << "     12     ¢x    2   ¢x    4   ¢x" << endl;
	stringStream << "     14     ¢x    0   ¢x   13   ¢x" << endl;
	ASSERT_STREQ(stringStream.str().c_str(), _model->getAllConnectionDetial().c_str());
	stringStream.clear();
	stringStream.str("");
	stringStream << "  Engineer¢x  PK(Emp_ID,Name),Department," << endl;
	stringStream << "        PC¢x  PK(PC_ID),Purchase_Date,FK(Emp_ID,Name)," << endl;
	ASSERT_EQ(stringStream.str().c_str(), _model->translateDiagramToTable());
	_model->addNodeByUI("Entity", "Work Diary");
	Component* work_diary = _model->getComponentByID(15);
	ASSERT_TRUE(work_diary!=NULL);
	ASSERT_EQ(Component::ENTITY, work_diary->getType());
	ASSERT_STREQ("Work Diary", work_diary->getName().c_str());
	_model->addNodeByUI("Relation", "Write");
	Component* writeRelation = _model->getComponentByID(16);
	ASSERT_TRUE(work_diary!=NULL);
	ASSERT_EQ(Component::RELATIONSHIP, writeRelation->getType());
	ASSERT_STREQ("Write", writeRelation->getName().c_str());
	_model->addConnectByUI(0, 16, Connection::ONE);
	ASSERT_EQ(Component::CONNECTION, _model->getComponentByID(17)->getType());
	ASSERT_EQ(Connection::ONE, ((Connection* )_model->getComponentByID(17))->getRealtionCardinality());
	ASSERT_TRUE(_model->getComponentByID(0)->checkAlreadyConnect(writeRelation));
	ASSERT_TRUE(writeRelation->checkAlreadyConnect(_model->getComponentByID(0)));
	_model->addConnectByUI(15, 16, Connection::ONE);
	ASSERT_EQ(Component::CONNECTION, _model->getComponentByID(18)->getType());
	ASSERT_EQ(Connection::ONE, ((Connection* )_model->getComponentByID(18))->getRealtionCardinality());
	ASSERT_TRUE(work_diary->checkAlreadyConnect(writeRelation));
	ASSERT_TRUE(writeRelation->checkAlreadyConnect(work_diary));
	_model->addNodeByUI("Attribute", "Content");
	Component* content = _model->getComponentByID(19);
	ASSERT_TRUE(content!=NULL);
	ASSERT_EQ(Component::ATTRIBUTE, content->getType());
	ASSERT_STREQ("Content", content->getName().c_str());
	_model->addNodeByUI("Attribute", "WD_ID");
	Component* wd_ID = _model->getComponentByID(20);
	ASSERT_TRUE(wd_ID!=NULL);
	ASSERT_EQ(Component::ATTRIBUTE, wd_ID->getType());
	ASSERT_STREQ("WD_ID", wd_ID->getName().c_str());
	_model->addNodeByUI("Attribute", "WD_date");
	Component* wd_Date = _model->getComponentByID(21);
	ASSERT_TRUE(wd_Date!=NULL);
	ASSERT_EQ(Component::ATTRIBUTE, wd_Date->getType());
	ASSERT_STREQ("WD_date", wd_Date->getName().c_str());
	_model->addConnectByUI(15, 19, Connection::NONE);
	ASSERT_EQ(Component::CONNECTION, _model->getComponentByID(22)->getType());
	ASSERT_TRUE(work_diary->checkAlreadyConnect(content));
	ASSERT_TRUE(content->checkAlreadyConnect(work_diary));
	_model->addConnectByUI(15, 20, Connection::NONE);
	ASSERT_EQ(Component::CONNECTION, _model->getComponentByID(23)->getType());
	ASSERT_TRUE(work_diary->checkAlreadyConnect(wd_ID));
	ASSERT_TRUE(wd_ID->checkAlreadyConnect(work_diary));
	_model->addConnectByUI(15, 21, Connection::NONE);
	ASSERT_EQ(Component::CONNECTION, _model->getComponentByID(24)->getType());
	ASSERT_TRUE(work_diary->checkAlreadyConnect(wd_Date));
	ASSERT_TRUE(wd_Date->checkAlreadyConnect(work_diary));
	list<unsigned int> primaryKeyIDs;
	primaryKeyIDs.push_back(wd_ID->getId());
	_model->setPrimaryKeyByUI(work_diary->getId(), primaryKeyIDs);
	Entity* work_diaryEntity = (Entity*) work_diary;
	ASSERT_EQ((unsigned int )1, work_diaryEntity->getPrimaryKey().size());
	ASSERT_TRUE(find(work_diaryEntity->getPrimaryKey().begin(), work_diaryEntity->getPrimaryKey().end(), wd_ID) != work_diaryEntity->getPrimaryKey().end());
	stringStream.clear();
	stringStream.str("");
	stringStream << "  Engineer¢x  PK(Emp_ID,Name),Department," << endl;
	stringStream << "        PC¢x  PK(PC_ID),Purchase_Date,FK(Emp_ID,Name)," << endl;
	stringStream << "Work Diary¢x  PK(WD_ID),Content,WD_date,FK(Emp_ID,Name)," << endl;
	ASSERT_EQ(stringStream.str().c_str(), _model->translateDiagramToTable());
	ASSERT_TRUE(find(_model->getComponents().begin(), _model->getComponents().end(), work_diary) != _model->getComponents().end());
	ASSERT_EQ((unsigned int )1, work_diaryEntity->getPrimaryKey().size());
	ASSERT_TRUE(find(work_diaryEntity->getPrimaryKey().begin(), work_diaryEntity->getPrimaryKey().end(), wd_ID) != work_diaryEntity->getPrimaryKey().end());
	_model->deleteNodeByID(15);
	ASSERT_EQ(NULL, _model->getComponentByID(15));
	ASSERT_FALSE(writeRelation->checkAlreadyConnect(work_diary));
	ASSERT_FALSE(content->checkAlreadyConnect(work_diary));
	ASSERT_FALSE(wd_ID->checkAlreadyConnect(work_diary));
	ASSERT_FALSE(wd_Date->checkAlreadyConnect(work_diary));
	stringStream.clear();
	stringStream.str("");
	stringStream << "  Engineer¢x  PK(Emp_ID,Name),Department," << endl;
	stringStream << "        PC¢x  PK(PC_ID),Purchase_Date,FK(Emp_ID,Name)," << endl;
	ASSERT_EQ(stringStream.str().c_str(), _model->translateDiagramToTable());
	Component* engineerComponet = _model->getComponentByID(0);
	Component* name = _model->getComponentByID(3);
	Component* emp_ID = _model->getComponentByID(1);
	ASSERT_EQ(Component::ENTITY, engineerComponet->getType());
	Entity* engineer = (Entity*) engineerComponet;
	ASSERT_EQ((unsigned int )2, engineer->getPrimaryKey().size());
	ASSERT_TRUE(find(engineer->getPrimaryKey().begin(), engineer->getPrimaryKey().end(), name) != engineer->getPrimaryKey().end());
	ASSERT_TRUE(find(engineer->getPrimaryKey().begin(), engineer->getPrimaryKey().end(), emp_ID) != engineer->getPrimaryKey().end());
	_model->undoCommand();
	stringStream.clear();
	stringStream.str("");
	stringStream << "  Engineer¢x  PK(Emp_ID,Name),Department," << endl;
	stringStream << "        PC¢x  PK(PC_ID),Purchase_Date,FK(Emp_ID,Name)," << endl;
	stringStream << "Work Diary¢x  PK(WD_ID),Content,WD_date,FK(Emp_ID,Name)," << endl;
	ASSERT_EQ(stringStream.str().c_str(), _model->translateDiagramToTable());
	ASSERT_TRUE(find(_model->getComponents().begin(), _model->getComponents().end(), work_diary) != _model->getComponents().end());
	ASSERT_EQ((unsigned int )1, work_diaryEntity->getPrimaryKey().size());
	ASSERT_TRUE(find(work_diaryEntity->getPrimaryKey().begin(), work_diaryEntity->getPrimaryKey().end(), wd_ID) != work_diaryEntity->getPrimaryKey().end());
	_model->redoCommand();
	ASSERT_EQ(NULL, _model->getComponentByID(15));
	Component* pcComponent = _model->getComponentByID(4);
	Component* pc_ID = _model->getComponentByID(5);
	ASSERT_EQ(Component::ENTITY, pcComponent->getType());
	Entity* pc = (Entity*) pcComponent;
	ASSERT_EQ((unsigned int )1, pc->getPrimaryKey().size());
	ASSERT_TRUE(find(pc->getPrimaryKey().begin(), pc->getPrimaryKey().end(), pc_ID) != pc->getPrimaryKey().end());
}

