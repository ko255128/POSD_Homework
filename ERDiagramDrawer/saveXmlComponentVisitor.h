#ifndef SAVEXMLCOMPONENTVISITOR_H_
#define SAVEXMLCOMPONENTVISITOR_H_
#include "componentvisitor.h"
#include "componentvisitor.h"
#include "Attribute.h"
#include "Entity.h"
#include "Relationship.h"
#include "Connection.h"
#include "ERModel.h"
#include <sstream>

using namespace std;

class saveXmlComponentVisitor :
	public ComponentVisitor
{
public:
	saveXmlComponentVisitor(void);
	~saveXmlComponentVisitor(void);
	void visit(Attribute* attribute);
	void visit(Entity* entity);
	void visit(Relationship* relationship);
	void visit(Connection* connection);
	string getData();
private:
	stringstream _contant;
};
#endif

