/*
 * Relationship.cpp
 *
 *  Created on: 2013/9/17
 *      Author: User
 */

#include "Relationship.h"
#include "stringResource.h"
#include <qpainter.h>
#include "ComponentVisitor.h"
Relationship::Relationship()
{
	const int RELATIONSHIP_HEIGHT = 40;
	const int RELATIONSHIP_WIDTH = 100;
	const int RELATIONSHIP_LOCATE_X = 610;
	_posX = RELATIONSHIP_LOCATE_X;
	_type = RELATIONSHIP;
	_height = RELATIONSHIP_HEIGHT;
	_width = RELATIONSHIP_WIDTH;
}

Relationship::Relationship( Component* relationship )
{
	this->_name = relationship->getName();
	this->_type = Component::RELATIONSHIP;
	this->_height = relationship->getHeight();
	this->_width = relationship->getWidth();
	this->_posX = relationship->getPosX() + pastPosOffset;
	this->_posY = relationship->getPosY() + pastPosOffset;
	this->_id = 0;
	this->_isPreview = false;
	this->_selected = false;
	_connectinons.clear();
}

Relationship::~Relationship()
{
}

//取得形別代表字符
string Relationship::getTypeSymbol()
{
	return RELATIONSHIP_TYPE_SYMBPL;
}

//回船是否能連接到目標點
bool Relationship::canConnectTo(Component* target)
{
	if (target->getType() == RELATIONSHIP || target->getType() == CONNECTION)
		return false;
	else
		return true;
}

void Relationship::disConnectTo(Component* target)
{
	hookdisConnectTo(target);
}

void Relationship::draw(QWidget* widget)
{
	QPainter painter(widget);
	const float HALF = 0.5;
	const int NUBER_OF_EDGE = 4;
	painter.setPen(Qt::black);
	if(_isPreview)
		painter.setPen(Qt::DashLine);
	QPointF points[] = { QPointF(_posX - _width * HALF, _posY), QPointF(_posX, _posY - _height * HALF), QPointF(_posX + _width * HALF, _posY), QPointF(_posX, _posY + _height * HALF) };
	painter.drawPolygon(points, NUBER_OF_EDGE);
	painter.drawText(_posX - _width * HALF * HALF, _posY, QString::fromStdString(getName()));
	if (_selected)
	{
		QPointF outBoundPoint[] = { QPointF(_posX - _width * HALF - outBoundOffset * HALF, _posY), QPointF(_posX, _posY - _height * HALF - outBoundOffset * HALF), QPointF(_posX + _width * HALF + outBoundOffset * HALF, _posY), QPointF(_posX, _posY + _height * HALF + outBoundOffset * HALF) };
		painter.setPen(Qt::green);
		painter.drawPolygon(outBoundPoint, NUBER_OF_EDGE);
	}
}
//Tmp = (x2-x1)(y3-y1) - (y2-y1)(x3-x1)
bool Relationship::isCollision(int x, int y)
{
	const float HALF = 0.5;
	if (x <= _posX && x >= (_posX - _width * HALF) && y <= _posY && y >= (_posY - _height * HALF))
		return (((_posX - _width * HALF) - _posX) * (y - (_posY - _height * HALF))) - ((_posY - (_posY - _height * HALF)) * (x - _posX)) <= 0;
	else if (x <= _posX && x >= (_posX - _width * HALF) && y >= _posY && y <= (_posY + _height * HALF))
		return (((_posX - (_posX - _width * HALF)) * (y - _posY))) - (((_posY + _height * HALF) - _posY) * (x - (_posX - _width * HALF))) <= 0;
	else if (x >= _posX && x <= (_posX + _width * HALF) && y >= (_posY - _height * HALF) && y <= _posY)
		return (((_posX + _width * HALF) - _posX) * (y - (_posY - _height * HALF))) - ((_posY - (_posY - _height * HALF)) * (x - _posX)) >= 0;
	else if (x >= _posX && x <= (_posX + _width * HALF) && y >= _posY && y <= (_posY + _height * HALF))
		return (((_posX - (_posX + _width * HALF)) * (y - _posY))) - (((_posY + _height * HALF) - _posY) * (x - (_posX + _width * HALF))) >= 0;
	else
		return false;
}

void Relationship::accept( ComponentVisitor* visitor )
{
	visitor->visit(this);
}

Component* Relationship::clone()
{
	return new Relationship(this);
}
