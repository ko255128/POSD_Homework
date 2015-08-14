#ifndef SAVECOMPONENTVISITOR_H_
#define SAVECOMPONENTVISITOR_H_
#include "componentvisitor.h"
#include "Attribute.h"
#include "Entity.h"
#include "Relationship.h"
#include "Connection.h"
#include "ERModel.h"
#include <sstream>

using namespace std;
class SaveComponentVisitor :
	public ComponentVisitor
{
public:
	SaveComponentVisitor(ERModel* model,bool saveWithPos);
	~SaveComponentVisitor(void);
	void visit(Attribute* attribute);
	void visit(Entity* entity);
	void visit(Relationship* relationship);
	void visit(Connection* connection);
	string getPosData();
	string getComponentData();
private:
	stringstream _ComponentPart;
	stringstream _ConnectionPart;
	stringstream _PrimarykeyPart;
	stringstream _PosPart;
	bool _saveWithPos;
	ERModel* _Model;
};
#endif

