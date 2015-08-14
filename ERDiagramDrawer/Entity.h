/*
 * Entity.h
 *
 *  Created on: 2013/9/17
 *      Author: User
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include "Node.h"
#include <QtGui/qwidget.h>
#include <QtGui/QPen.h>
#include <list>
#include <vector>
class Entity: public Node
{
public:
	Entity();
	virtual ~Entity();
	string getTypeSymbol();
	bool canConnectTo(Component* target);
	vector<unsigned int> getAllAttributeID();
	list<unsigned int> getallPrimaryKeyID();
	void addPrimaryKey(Component* attribuide);
	void clearPrimaryKey();
	void disConnectTo(Component* target);
	bool isPrimaryKey(Component* target);
	vector<Component*> getAllConnectRelation();
	void draw(QWidget* widget);
	bool isCollision(int x, int y);
	void accept(ComponentVisitor* visitor);
	Component* clone();

	const vector<Component*>& getPrimaryKey() const
	{
		return _primaryKey;
	}

private:
	vector<Component*> _primaryKey;
	Entity(Component* entity);
};

#endif /* ENTITY_H_ */
