/*
 * stringResource.h
 *
 *  Created on: 2013/10/31
 *      Author: ChunHsiang
 */

#ifndef STRINGRESOURCE_H_
#define STRINGRESOURCE_H_
#include <string>
using namespace std;

static const string PROGRAM_NAME = "Entity Relation Diagram Tool";
static const string ABOUT_PROGRAM = "About Entity Relation Diagram Tool";
static const string ABOUT_CONTEXT = "Entity Relation Diagram Tool\n1.0\nAuthor:t102598014@ntut.edu.tw";
static const string SLASH = "/";
static const string BACK_SLASH = "\\";
static const string POS_FILE_FILE_NAME_EXTENSION = ".pos";
static const string XML_ATTRIBUTE_START_LABEL = "<Attribute>";
static const string XML_ATTRIBUTE_END_LABEL = "</Attribute>";
static const string XML_ENTITY_START_LABEL = "<Entity>";
static const string	XML_ENTITY_END_LABEL = "</Entity>";
static const string XML_RELATION_START_LABEL = "<Relation>";
static const string XML_RELATION_END_LABEL = "</Relation>";
static const string XML_CONNECTOR_START_LABEL = "<Connector>";
static const string XML_CONNECTOR_END_LABEL = "</Connector>";
static const string XML_ERDIAGRAM_START_LABEL = "<ERDiagram>";
static const string XML_ERDIAGRAM_END_LABEL = "</ERDiagram>";
static const string XML_ID_START_LABEL = "<ID>";
static const string XML_ID_END_LABEL = "</ID>";
static const string XML_TEXT_START_LABEL = "<Text>";
static const string XML_TEXT_END_LABEL = "</Text>";
static const string XML_PRIMARYKEY_START_LABEL = "<PrimaryKey>";
static const string XML_PRIMARYKEY_END_LABEL = "</PrimaryKey>";
static const string XML_POSITIONX_START_LABEL = "<PositionX>";
static const string XML_POSITIONX_END_LABEL = "</PositionX>";
static const string XML_POSITIONY_START_LABEL = "<PositionY>";
static const string XML_POSITIONY_END_LABEL = "</PositionY>";
static const string XML_SOURCE_START_LABEL = "<Source>";
static const string XML_SOURCE_END_LABEL = "</Source>";
static const string XML_TARGET_START_LABEL = "<Target>";
static const string XML_TARGET_END_LABEL = "</Target>";
static const string XML_VERSION_LABEL = "<?xml version=\"1.0\"?>";
static const string XML_INDENT_LEVEL1 = "\t";
static const string XML_INDENT_LEVEL2 = "\t\t";


static const string CAN_NOT_GET_NAME = "can not get name";
static const string EMPTY_STRING = "";
static const string REALTION_CARDINALITY_ONE = "1";
static const string REALTION_CARDINALITY_N = "N";
static const string LEFT_BRACKETS = "(";
static const string RIGHT_BRACKETS = ")";
static const string BLANK = " ";
static const string COMMA = ",";
static const string DOT = ".";
static const string CONNECTION_SEPARATED = "     ¢x";
static const string COMPONENT_SEPARATED = "  ¢x";
static const string ENTITY_SPEARATE = "¢x  ";
static const string TWO_BLANK_STRING = "  ";
static const string FORIGEN_KEY_TIP = "FK";
static const string PRIMARY_KEY_TIP = "PK";
static const string ENTITY_TYPE_NAME = "Entity";
static const string ATTRITUBE_TYPE_NAME = "Attribute";
static const string RELATIONSHIP_TYPE_NAME = "Relation";
static const string CONNECTION_TYPE_NAME = "Connector";
static const string CONNECTION_TYPE_SYMBOL = "C";
static const string ENTITY_TYPE_SYMBOL = "E";
static const string ATTRIBUTE_TYYPE_SYMBOL = "A";
static const string RELATIONSHIP_TYPE_SYMBPL = "R";

static const string COMPONENT_LABEL = "Components";

static const string ILLEGAL_CONNECT = "Illegal connect!";
static const string CONNECT_ITSELF = "Can't connect itself!";
static const string ALREADY_CONNECT = "Already connected!";

static const string SELECT_REALTION_CARDINALITY_NOTICE = "Please select relation cardinality:";
static const string SELECT_REALTION_CARDINALITY_DIALOG_TITLE = "Select relation cardinality";

static const string CONNECTION_DETIAL_SEPARATED = "   ¢x";

static const char ENDLINE = '\n';

static const string NOTICE_INPUT_SYMBOL = ">";
static const string MAIN_MENU = "1. Load ER diagram file\n2. Save ER diagram file\n3. Add a node\n4. Connect two nodes\n5. Display the current diagram\n6. Set a primary key\n7. Display the table\n8. Delete a component\n9. Undo\n10.Redo\n11.Exit\n";
static const string INPUT_INVALID_VALUE = "You entered an invalid value. Please enter a valid one again.\n";
static const string TABLE_HEADER = "------------------------------------------------------\n    Entity| Attribute\n----------+---------------------------------------------\n";
static const string TABLE_ENDLINE = "------------------------------------------------------\n";

static const string PRINT_COMPONET_NOTICE = "Component:\n";
static const string COMPONENT_HEADER = "------------------------------------\n TYPE ¢x  ID  ¢x  Name \n------+------+----------------------\n";
static const string COMPONENT_ENDLINE = "------------------------------------\n";
static const string PRING_CONNECT_NOTICE = "Connections:\n";
static const string CONNECT_HEADER = "-------------------------------\n Connection ¢x  node  ¢x  node  ¢x\n------------+--------+---------\n";
static const string CONNECT_ENDLINE = "-------------------------------\n";

static const string PRINT_ENTITY_NOTICE = "Entities:\n";
static const string PRINT_ATTRIBUTE_NOTICE = "Attributes of Entity '";
static const string INPUT_PRIMARY_KEYS_ID_NOTICE = "Enter the IDs of the attributes (use a comma to separate two attributes):\n";
static const string DO_NOT_HAVE_ANY_ATTRIBUTE_OF_ENTITY = "Do not have any Attribute of Entity '";


static const string INPUT_FILE_PATH_NOTICE = "Please input a file path:";
static const string ENTER_ADD_NODE_TYPE_NOTICE = "What kind of node do you want to add?\n";
static const string NODE_TYPE_NOTICE = "[A]Attribute [E]Entity [R]Relation\n";

static const string NOTIC_ENTER_CONNECT_FIRST_NODE_ID = "Please enter the first node ID\n";
static const string NOTIC_ENTER_CONNECT_SECOND_NODE_ID = "Please enter the second node ID\n";

static const string NOTIC_DO_NOT_HAVE_ENOUGH_NODE_CONNECT = "Do not have enough node to connected. Please add Node first.\n";
static const string NOTIC_INPUT_ID_NOT_EXIST = "The node ID you entered does not exist. Please enter a valid one again.\n";

static const string NOTIC_ENTER_COMPONENT_ID = "Please enter the component ID\n";

static const string UNDO_SUCCEED_NOTICE = "Undo succeed!\n";
static const string CAN_NOT_UNDO_NOTICE = "Cannot Undo.\n";
static const string REDO_SUCCEED_NOTICE = "Redo succeed!\n";
static const string CAN_NOT_REDO_NOTICE = "Cannot Redo.\n";

static const string ADD_NODE_TYPE_FAIURE = "You entered an invalid node. Please enter a valid one again.\n";
static const string NOTIC_ENTER_ENTITY_ID = "Enter the ID of the entity:\n";

static const string ADD_NODE_NAME_NOTICE = "Enter the name of this node:\n";

static const string ADD_NODE_SUCCEED_FRONT_TYPE_PART = "A node [";
static const string ADD_NODE_SUCCEED_FRONT_ID_PART = "] has been added. ID: ";
static const string ADD_NODE_SUCCEED_FRONT_NAME_PART =", Name: \"";
static const string QUOTATION_MARK = "\"";
static const string INPUT_PRIMARY_KEY_FORMAT_ERROR_NOTICE = "You entered an invalid format. Please enter a valid one again.\n";
static const string INPUT_PRIMARY_KEY_NOT_BELONG_ENTITY_MIDDLE_PART = "' does not belong to Entity '";
static const string INPUT_PRIMARY_KEY_NOT_BELONG_ENTITY_END_PART = "'. Please enter a valid one again.\n";

static const string MESSAGE_BEFORE_FIRST_NODE_ID = "The node '";
static const string SELECT_ENTIT_FAILURE_MESSAGE_AFTER_NODE_ID = "' is not an entity. Please enter a valid one again.\n";
static const string CONNECT_MESSAGE_CONNECT_IT_SELF = "' cannot be connected to itself.\n";
static const string CONNECT_MESSAGE_CANOT_CONNECT_BEFORE_SECOND_NODE_PART = "' cannot be connected by the node '";
static const string CONNECT_MESSAGE_ALREADY_CONNECT_BEFORE_SECOND_NODE_PART = "' has already been connected to component '";
static const string CONNECT_MESSAGE_CONNECT_SUCCEED_BEFORE_SECOND_NODE_PART = "' has been connected to the node '";
static const string SIGNLE_QUTOE_AND_NEW_LINE = "'\n";
static const string CONNECT_MESSAGE_CARDINALITY_IS_N = "Its cardinality of the relationship is 'N'.\n";
static const string CONNECT_MESSAGE_CARDINALITY_IS_ONE = "Its cardinality of the relationship is '1'.\n";
static const string DO_NOT_HAVE_ANY_COMPONENT = "You don't have any Component.\n";

static const string NOTICE_DO_NOT_HAVE_ANY_ENTITY = "Do not have any Entity.\n";
static const string INPUT_PRIMARY_KEY_SUCCEED_NOTICE_BEFORE_ENTITY_ID_PART = "The entity '";
static const string INPUT_PRIMARY_KEY_SUCCEED_NOTICE_BEFORE_PRIMARY_KEYS_PART = "' has the primary key (";
static const string INPUT_PRIMARY_KEY_SUCCEED_NOTICE_AFTERT_PRIMARY_KEYS_PART = ").\n";
static const string COMPONE_ID_NOT_EXIST_NOTICE = "The component ID you entered does not exist. Please enter a valid one again.\n";

static const string DELETE_SUCCED_NOITCE_BEFORE_NODE_ID_PART = "The component '";
static const string DELETE_SUCCED_NOTICE_AFTER_NODE_ID_PART = "' has been deleted.\n";
static const string LOAD_FILE_SUCCEED_NOTICE = "The ER diagram is displayed as follows: ";
static const string LOAD_FILE_ERROR_NOTICE = "File not exist or format error!\n";
static const string FILE_FORMAT_ERROR_NOTICE = "Format Error!";
static const string SAVE_FILE_ERROR_NOTICE = "File path not exist\n";
static const string SVAE_FILE_SUCCEED_NOTICE = "Save Succeed\n";

static const string TITLE_TYPE = "Type";
static const string TITLE_NAME = "Name";
#endif /* STRINGRESOURCE_H_ */
