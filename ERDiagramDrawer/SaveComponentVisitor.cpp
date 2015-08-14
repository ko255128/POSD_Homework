#include "SaveComponentVisitor.h"
#include "stringResource.h"
using namespace std;
SaveComponentVisitor::SaveComponentVisitor(ERModel* model,bool saveWithPos)
{
	this->_Model = model;
	_ComponentPart.str(EMPTY_STRING);
	_ConnectionPart.str(EMPTY_STRING);
	_PosPart.str(EMPTY_STRING);
	_PrimarykeyPart.str(EMPTY_STRING);
	_ComponentPart.clear();
	_ConnectionPart.clear();
	_PosPart.clear();
	_PrimarykeyPart.clear();
	_saveWithPos = saveWithPos;
}


SaveComponentVisitor::~SaveComponentVisitor(void)
{
}

void SaveComponentVisitor::visit( Attribute* attribute )
{
	 _ComponentPart << attribute->getTypeSymbol() << COMMA << BLANK << attribute->getName() << endl;
	 if(_saveWithPos)
		 _PosPart << attribute->getPosX() << BLANK << attribute->getPosY() << endl;
}

void SaveComponentVisitor::visit( Entity* entity )
{
	_ComponentPart << entity->getTypeSymbol() << COMMA << BLANK << entity->getName() << endl;
	if(_saveWithPos)
		_PosPart << entity->getPosX() << BLANK << entity->getPosY() << endl;
	if(!entity->getPrimaryKey().empty())
	{
		_PrimarykeyPart << distance(_Model->getComponents().begin(),find(_Model->getComponents().begin(), _Model->getComponents().end(), entity)) << BLANK;
		vector<Component*> primarykeys = entity->getPrimaryKey();
		for (unsigned int index = 0 ; index < primarykeys.size()-1 ; index++)
			_PrimarykeyPart << distance(_Model->getComponents().begin(),find(_Model->getComponents().begin(), _Model->getComponents().end(), primarykeys.at(index))) << COMMA;
		_PrimarykeyPart << distance(_Model->getComponents().begin(),find(_Model->getComponents().begin(), _Model->getComponents().end(), primarykeys.back())) << endl;	
	}
}

void SaveComponentVisitor::visit( Relationship* relationship )
{
	if(_saveWithPos)
		_PosPart << relationship->getPosX() << BLANK << relationship->getPosY() << endl;
	_ComponentPart << relationship->getTypeSymbol() << COMMA << BLANK << relationship->getName() << endl;
}

void SaveComponentVisitor::visit( Connection* connection )
{
	if(connection->getRealtionCardinality() != Connection::NONE)
		_ComponentPart << connection->getTypeSymbol() << COMMA << BLANK << connection->getName() << endl;
	else
		_ComponentPart << connection->getTypeSymbol() << endl;
	_ConnectionPart <<  distance(_Model->getComponents().begin(),find(_Model->getComponents().begin(), _Model->getComponents().end(), connection)) << BLANK;
	_ConnectionPart <<  distance(_Model->getComponents().begin(),find(_Model->getComponents().begin(), _Model->getComponents().end(), connection->getConnectinons().front())) << COMMA;
	_ConnectionPart <<  distance(_Model->getComponents().begin(),find(_Model->getComponents().begin(), _Model->getComponents().end(), connection->getConnectinons().back())) << endl;
}

std::string SaveComponentVisitor::getPosData()
{
	return _PosPart.str();
}

std::string SaveComponentVisitor::getComponentData()
{
	return _ComponentPart.str() + ENDLINE + _ConnectionPart.str() + ENDLINE + _PrimarykeyPart.str();
}
