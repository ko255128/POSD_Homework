#include "saveXmlComponentVisitor.h"
#include "stringResource.h"


saveXmlComponentVisitor::saveXmlComponentVisitor(void)
{
	_contant.str(EMPTY_STRING);
	_contant.clear();
	_contant << XML_VERSION_LABEL << endl << XML_ERDIAGRAM_START_LABEL << endl;
}


saveXmlComponentVisitor::~saveXmlComponentVisitor(void)
{
}

void saveXmlComponentVisitor::visit( Attribute* attribute )
{
	_contant <<XML_INDENT_LEVEL1 << XML_ATTRIBUTE_START_LABEL << endl;
	_contant <<XML_INDENT_LEVEL2 << XML_ID_START_LABEL << attribute->getId() << XML_ID_END_LABEL << endl;
	_contant <<XML_INDENT_LEVEL2 << XML_TEXT_START_LABEL << attribute->getName() << XML_TEXT_END_LABEL <<endl;
	_contant <<XML_INDENT_LEVEL2 << XML_POSITIONX_START_LABEL << attribute->getPosX() << XML_POSITIONX_END_LABEL << endl;
	_contant <<XML_INDENT_LEVEL2 << XML_POSITIONY_START_LABEL << attribute->getPosY() << XML_POSITIONY_END_LABEL << endl;
	_contant <<XML_INDENT_LEVEL1 << XML_ATTRIBUTE_END_LABEL << endl;
}

void saveXmlComponentVisitor::visit( Entity* entity )
{
	_contant <<XML_INDENT_LEVEL1 << XML_ENTITY_START_LABEL << endl;
	_contant <<XML_INDENT_LEVEL2 << XML_ID_START_LABEL << entity->getId() << XML_ID_END_LABEL << endl;
	_contant <<XML_INDENT_LEVEL2 << XML_TEXT_START_LABEL << entity->getName() << XML_TEXT_END_LABEL <<endl;
	_contant <<XML_INDENT_LEVEL2 << XML_PRIMARYKEY_START_LABEL;
	if(!entity->getallPrimaryKeyID().empty())
	{
		list<unsigned int> primarykeyIDs = entity->getallPrimaryKeyID();
		for(list<unsigned int>::iterator listIterator = primarykeyIDs.begin();listIterator!=(--primarykeyIDs.end());listIterator++)
			_contant << *listIterator << COMMA;
		_contant << primarykeyIDs.back();
	}
	_contant << XML_PRIMARYKEY_END_LABEL<<endl;
	_contant <<XML_INDENT_LEVEL2 << XML_POSITIONX_START_LABEL << entity->getPosX() << XML_POSITIONX_END_LABEL << endl;
	_contant <<XML_INDENT_LEVEL2 << XML_POSITIONY_START_LABEL << entity->getPosY() << XML_POSITIONY_END_LABEL << endl;
	_contant <<XML_INDENT_LEVEL1 << XML_ENTITY_END_LABEL << endl;
}

void saveXmlComponentVisitor::visit( Relationship* relationship )
{
	_contant <<XML_INDENT_LEVEL1 << XML_RELATION_START_LABEL << endl;
	_contant <<XML_INDENT_LEVEL2 << XML_ID_START_LABEL << relationship->getId() << XML_ID_END_LABEL << endl;
	_contant <<XML_INDENT_LEVEL2 << XML_TEXT_START_LABEL << relationship->getName() << XML_TEXT_END_LABEL <<endl;
	_contant <<XML_INDENT_LEVEL2 << XML_POSITIONX_START_LABEL << relationship->getPosX() << XML_POSITIONX_END_LABEL << endl;
	_contant <<XML_INDENT_LEVEL2 << XML_POSITIONY_START_LABEL << relationship->getPosY() << XML_POSITIONY_END_LABEL << endl;
	_contant <<XML_INDENT_LEVEL1 << XML_RELATION_END_LABEL << endl;
}

void saveXmlComponentVisitor::visit( Connection* connection )
{
	_contant <<XML_INDENT_LEVEL1 << XML_CONNECTOR_START_LABEL << endl;
	_contant <<XML_INDENT_LEVEL2 << XML_ID_START_LABEL << connection->getId() << XML_ID_END_LABEL << endl;
	_contant <<XML_INDENT_LEVEL2 << XML_TEXT_START_LABEL << connection->getName() << XML_TEXT_END_LABEL <<endl;
	_contant <<XML_INDENT_LEVEL2 << XML_SOURCE_START_LABEL << connection->getConnectinons().front()->getId() << XML_SOURCE_END_LABEL << endl;
	_contant <<XML_INDENT_LEVEL2 << XML_TARGET_START_LABEL << connection->getConnectinons().back()->getId() << XML_TARGET_END_LABEL << endl;
	_contant <<XML_INDENT_LEVEL1 << XML_CONNECTOR_END_LABEL << endl;
}

std::string saveXmlComponentVisitor::getData()
{
	return _contant.str() + XML_ERDIAGRAM_END_LABEL;
}
