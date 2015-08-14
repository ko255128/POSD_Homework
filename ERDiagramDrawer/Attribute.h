/*
 * Attribute.h
 *
 *  Created on: 2013/9/17
 *      Author: User
 */

#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_

#include "Node.h"
#include <QtGui/qwidget.h>
#include <QtGui/QPen.h>

class Attribute: public Node
{
public:
	friend class AttributeTest;
	Attribute();
	virtual ~Attribute();
	string getTypeSymbol();
	bool canConnectTo(Component* target);
	void disConnectTo(Component* target);
	void draw(QWidget* widget);
	bool isCollision(int x, int y);
	void accept(ComponentVisitor* visitor);
	Component* clone();
	bool getIsPrimaryKey()
	{
		return _isPrimaryKey;
	}
	void setIsPrimaryKey(bool isPrimaryKey)
	{
		_isPrimaryKey = isPrimaryKey;
	}
private:
	Attribute(Component* component);
	bool _isPrimaryKey;
};

#endif /* ATTRIBUTE_H_ */
