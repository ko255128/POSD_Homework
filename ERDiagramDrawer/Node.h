/*
 * Node.h
 *
 *  Created on: 2013/9/16
 *      Author: User
 */

#ifndef NODE_H_
#define NODE_H_

#include "Compoment.h"
#include <QtGui/qwidget.h>

using namespace std;

class Node: public Component
{
public:
	Node();
	virtual ~Node();
	string getTypeSymbol() = 0;
	bool canConnectTo(Component* target) = 0;
	void disConnectTo(Component* target) = 0;
	void draw(QWidget* widget) = 0;
	bool isCollision(int x, int y)=0;
	void accept(ComponentVisitor* visitor)=0;
	Component* clone()=0;
};

#endif /* NODE_H_ */
