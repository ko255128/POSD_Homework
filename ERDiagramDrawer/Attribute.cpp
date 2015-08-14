/*
 * Attribute.cpp
 *
 *  Created on: 2013/9/17
 *      Author: User
 */

#include "Attribute.h"
#include "ComponentVisitor.h"
#include <QtGui/qwidget.h>
#include <QtGui/QPen.h>
#include <QPainter>
#include <qfont.h>
#include <cmath>


//建構元
Attribute::Attribute()
{
	const int ATTRIBUTE_HEIGHT = 40;
	const int ATTRIBUTE_WIDTH = 100;
	const int ATTRIBUTE_LOCATE_X = 10;
	_isPrimaryKey = false;

	_posX = ATTRIBUTE_LOCATE_X;

	_type = ATTRIBUTE;
	_height = ATTRIBUTE_HEIGHT;
	_width = ATTRIBUTE_WIDTH;
}

Attribute::Attribute( Component* component )
{
	this->_name = component->getName();
	this->_type = Component::ATTRIBUTE;
	this->_height = component->getHeight();
	this->_width = component->getWidth();
	this->_posX = component->getPosX() + pastPosOffset;
	this->_posY = component->getPosY() + pastPosOffset;
	this->_id = 0;
	this->_isPreview = false;
	this->_selected = false;
	this->_isPrimaryKey = ((Attribute*)component)->getIsPrimaryKey();
	_connectinons.clear();
}

Attribute::~Attribute()
{

}
//取得形態符號
string Attribute::getTypeSymbol()
{
	const string Attribute_SYMBOL = "A";
	return Attribute_SYMBOL;
}
//是否能連結到target點
bool Attribute::canConnectTo(Component* target)
{
	if (target->getType() == ATTRIBUTE || target->getType() == CONNECTION)
		return false;
	else
		return true;
}
//取消連結至Target點
void Attribute::disConnectTo(Component* target)
{
	hookdisConnectTo(target);
}

void Attribute::draw(QWidget* widget)
{
	QPainter painter(widget);
	QFont font;
	const float HELF = 0.5;
	painter.setPen(Qt::black);
	if(_isPreview)
		painter.setPen(Qt::DashLine);
	font.setUnderline(_isPrimaryKey);
	painter.setFont(font);
	painter.drawEllipse(_posX, _posY, _width, _height);
	painter.drawText(_posX + _width * HELF, _posY + _height * HELF, QString::fromStdString(this->getName()));
	if (_selected)
	{
		painter.setPen(Qt::green);
		painter.drawEllipse(_posX - outBoundOffset * HELF, _posY - outBoundOffset * HELF, _width + outBoundOffset, _height + outBoundOffset);
	}

}

bool Attribute::isCollision(int x, int y)
{
	const float HELF = 0.5;
	const float power = 2;
	const float centerX = _posX + _width * HELF;
	const float centerY = _posY + _height * HELF;
	float eclipseFocusDistance = sqrt(pow(_width * HELF, power) - pow(_height * HELF, power));
	return (sqrt(pow(x - (centerX - eclipseFocusDistance), power) + pow(y - centerY, power)) + sqrt(pow(x - (centerX + eclipseFocusDistance), power) + pow(y - centerY, power))) <= _width;
}

void Attribute::accept( ComponentVisitor* visitor )
{
	visitor->visit(this);
}

Component* Attribute::clone()
{
	return new Attribute(this);
}
