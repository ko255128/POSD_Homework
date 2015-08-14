/*
 * Connection.cpp
 *
 *  Created on: 2013/9/17
 *      Author: User
 */

#include "Connection.h"
#include "ComponentVisitor.h"
#include "stringResource.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <QPainter>
#include <cmath>
Connection::Connection()
{
	_type = CONNECTION;
	_realtionCardinality = NONE;

}

Connection::~Connection()
{
}
//取得代表的型態字元
string Connection::getTypeSymbol()
{
	return CONNECTION_TYPE_SYMBOL;
}

//回傳此Connection的詳細資料(字串)
string Connection::getDetial()
{
	stringstream stringStream;
	const int NODE_WIDTH = 5;
	for (vector<Component*>::iterator listIterator = _connectinons.begin(); listIterator != _connectinons.end(); listIterator++)
	{
		Component* component = *listIterator;
		stringStream << setw(NODE_WIDTH) << component->getId() << CONNECTION_DETIAL_SEPARATED;
	}
	return stringStream.str();
}

bool Connection::canConnectTo(Component* target)
{
	return false;
}

Component* Connection::getAnthorConnectedNode(Component* contectedNode)
{
	if (contectedNode == NULL || find(_connectinons.begin(), _connectinons.end(), contectedNode) == _connectinons.end())
		return NULL;
	for (vector<Component*>::iterator vectorIterator = _connectinons.begin(); vectorIterator != _connectinons.end(); vectorIterator++)
	{
		Component* component = *vectorIterator;
		if (component != contectedNode)
		{
			return component;
		}
	}
	return NULL;
}

void Connection::disConnectTo(Component* target)
{
}

//設定關連型態(一對一，一對多，或無 連帶設定到名稱中)
void Connection::setRealtionCardinality(REALTION_CARDINALITY realtionCardinality)
{
	_realtionCardinality = realtionCardinality;
	if (_realtionCardinality == ONE)
		_name = REALTION_CARDINALITY_ONE;
	else if (_realtionCardinality == N)
		_name = REALTION_CARDINALITY_N;
	else
		_name = EMPTY_STRING;
}
//計算連接Relation與Entity間的線段位置
void Connection::calcEntityAndRelationConnectPos()
{
	float HALF = 0.5;
	if (_connectinons.front()->getType() == ENTITY && _connectinons.back()->getType() == RELATIONSHIP)
	{
		_posX = _connectinons.front()->getPosX() + _connectinons.front()->getWidth();
		_posY = _connectinons.front()->getPosY() + _connectinons.front()->getHeight() * HALF;
		_width = (_connectinons.back()->getPosX() - _connectinons.back()->getWidth() * HALF);
		_height = _connectinons.back()->getPosY();
	}
	else if (_connectinons.front()->getType() == RELATIONSHIP && _connectinons.back()->getType() == ENTITY)
	{
		_posX = _connectinons.back()->getPosX() + _connectinons.back()->getWidth();
		_posY = _connectinons.back()->getPosY() + _connectinons.back()->getHeight() * HALF;
		_width = (_connectinons.front()->getPosX() - _connectinons.front()->getWidth() * HALF);
		_height = _connectinons.front()->getPosY();
	}
}
//計算連接Attribute與Entity間的線段位置
void Connection::calcAttributeEntityConnectPos()
{
	float HALF = 0.5;
	if (_connectinons.front()->getType() == ATTRIBUTE && _connectinons.back()->getType() == ENTITY)
	{
		_posX = _connectinons.front()->getPosX() + _connectinons.front()->getWidth();
		_posY = _connectinons.front()->getPosY() + _connectinons.front()->getHeight() * HALF;
		_width = _connectinons.back()->getPosX();
		_height = (_connectinons.back()->getPosY() + _connectinons.back()->getHeight() * HALF);
	}
	else if (_connectinons.front()->getType() == ENTITY && _connectinons.back()->getType() == ATTRIBUTE)
	{
		_posX = _connectinons.back()->getPosX() + _connectinons.back()->getWidth();
		_posY = _connectinons.back()->getPosY() + _connectinons.front()->getHeight() * HALF;
		_width = _connectinons.front()->getPosX();
		_height = (_connectinons.front()->getPosY() + _connectinons.front()->getHeight() * HALF);
	}
}
//計算連接Attribute與Relationship間的線段位置
void Connection::calcAttributeRelationshipConnectPos()
{
	float HALF = 0.5;
	if (_connectinons.front()->getType() == ATTRIBUTE && _connectinons.back()->getType() == RELATIONSHIP)
	{
		_posX = _connectinons.front()->getPosX() + _connectinons.front()->getWidth();
		_posY = _connectinons.front()->getPosY() + _connectinons.front()->getHeight() * HALF;
		_width = (_connectinons.back()->getPosX() - _connectinons.back()->getWidth() * HALF);
		_height = _connectinons.back()->getPosY();
	}
	else if (_connectinons.front()->getType() == RELATIONSHIP && _connectinons.back()->getType() == ATTRIBUTE)
	{
		_posX = _connectinons.back()->getPosX() + _connectinons.back()->getWidth();
		_posY = _connectinons.back()->getPosY() + _connectinons.front()->getHeight() * HALF;
		_width = (_connectinons.front()->getPosX() - _connectinons.front()->getWidth() * HALF);
		_height = _connectinons.front()->getPosY();
	}
}
//劃出圖形
void Connection::draw(QWidget* widget)
{
	QPainter painter(widget);
	calcEntityAndRelationConnectPos();
	calcAttributeEntityConnectPos();
	calcAttributeRelationshipConnectPos();
	float HALF = 0.5;
	if (_selected)
		painter.setPen(Qt::green);
	else
		painter.setPen(Qt::black);
	painter.drawLine(_posX, _posY, _width, _height);
	painter.drawText((_posX + _width) * HALF, (_posY + _height) * HALF, QString::fromStdString(getName()));
}
//計算點到線的距離
double Connection::calcDisatanceToConnectionLine(int x, int y)
{
	double slope, b;
	double distance;
	if (_posX != _width)
	{
		slope = (double) ((_posY - _height)) / (_posX - _width);
		b = (double) (_posY) - slope * _posX;
		distance = abs(slope * x - y + b) / sqrt((long double) (slope) * slope + 1);
	}
	else
		distance = abs(x - _posX);

	return distance;
}

//判斷是否被點到
bool Connection::isCollision(int x, int y)
{
	const float ALLOWE_ERROR = 2.0;
	int maxX, minX, maxY, minY;
	if (_width >= _posX)
	{
		maxX = _width;
		minX = _posX;
	}
	else
	{
		maxX = _posX;
		minX = _width;
	}
	if (_height >= _posY)
	{
		maxY = _height;
		minY = _posY;
	}
	else
	{
		maxY = _posY;
		minY = _height;
	}
	if (x < minX || x > maxX || y < minY || y > maxY)
		return false;
	double distance = calcDisatanceToConnectionLine(x, y);
	return distance <= ALLOWE_ERROR;
}

void Connection::accept( ComponentVisitor* visitor )
{
	visitor->visit(this);
}
