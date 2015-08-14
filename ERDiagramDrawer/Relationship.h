/*
 * Relationship.h
 *
 *  Created on: 2013/9/17
 *      Author: User
 */

#ifndef RELATIONSHIP_H_
#define RELATIONSHIP_H_

#include "Node.h"
#include "Compoment.h"
#include <QtGui/qwidget.h>
#include <QtGui/QPen.h>

class Relationship: public Node
{
public:
	Relationship();
	virtual ~Relationship();
	string getTypeSymbol();
	bool canConnectTo(Component* target);
	void disConnectTo(Component* target);
	void draw(QWidget* widget);
	bool isCollision(int x, int y);
	void accept(ComponentVisitor* visitor);
	Component* clone();
private:
	Relationship(Component* relationship);
};

#endif /* RELATIONSHIP_H_ */
