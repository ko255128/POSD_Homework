#ifndef COMPONENTVISTOR_H_
#define COMPONENTVISTOR_H_
#include "Attribute.h"
#include "Entity.h"
#include "Relationship.h"
#include "Connection.h"
#include <string>

using namespace std;
class ComponentVisitor
{
public:
	ComponentVisitor(){}
	virtual ~ComponentVisitor(){}
	virtual void visit(Attribute* attribute) = 0;
	virtual void visit(Entity* entity) = 0;
	virtual void visit(Relationship* relationship) = 0;
	virtual void visit(Connection* connection)=0;
};
#endif


