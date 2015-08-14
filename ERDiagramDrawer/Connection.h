/*
 * Connection.h
 *
 *  Created on: 2013/9/17
 *      Author: User
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "Compoment.h"
#include <QtGui/qwidget.h>
#include <QtGui/QPen.h>

class Connection: public Component
{
public:
	enum REALTION_CARDINALITY
	{
		NONE, N, ONE
	};
	Connection();
	virtual ~Connection();
	string getTypeSymbol();
	string getDetial();
	bool canConnectTo(Component* target);
	Component* getAnthorConnectedNode(Component* contectedNode);
	void disConnectTo(Component* target);
	void draw(QWidget* widget);
	bool isCollision(int x,int y);
	void accept(ComponentVisitor* visitor);
	Component* clone()
	{
		return NULL;
	}

	REALTION_CARDINALITY getRealtionCardinality() const
	{
		return _realtionCardinality;
	}

	void setRealtionCardinality(REALTION_CARDINALITY realtionCardinality);

private:
	REALTION_CARDINALITY _realtionCardinality;

	void calcEntityAndRelationConnectPos();
	void calcAttributeEntityConnectPos();
	void calcAttributeRelationshipConnectPos();
	bool collisionRange(double boundary1, double boundary2, double number);
	double calcDisatanceToConnectionLine(int x, int y);
};

#endif /* CONNECTION_H_ */
