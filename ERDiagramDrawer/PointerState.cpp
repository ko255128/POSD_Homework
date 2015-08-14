/*
 * PointerState.cpp
 *
 *  Created on: 2013/11/15
 *      Author: User
 */

#include "PointerState.h"
#include "PresentationModel.h"
#include <iostream>

using namespace std;
PointerState::PointerState(PresentationModel* presentationModel)
{
	this->_presentation = presentationModel;
	beginMove = false;
}

PointerState::~PointerState()
{
	// TODO Auto-generated destructor stub
}

void PointerState::mousePressEvent(QMouseEvent* event, QWidget* widget)
{
	if (!_mouseClick)
	{
		_mouseClick = true;
		_presentation->selectComponent(event->x(),event->y());
		widget->repaint();
	}
}

void PointerState::mouseMoveEvent(QMouseEvent* event, QWidget* widget)
{
	if(!beginMove)
	{
		begingPoint = event->pos();
		beginMove = true;
		beginMovePoint = event->pos();
	}
	else
	{
		_presentation->moveComponent(event->x()-begingPoint.x(),event->y()-begingPoint.y());
		begingPoint = event->pos();
	}
}

void PointerState::mouseReleaseEvent(QMouseEvent* event, QWidget* widget)
{
	_mouseClick = false;
	if(beginMove&&_presentation->isHaveSelect())
	{
		if(abs(event->x()-beginMovePoint.x()) > 0 || abs(event->y()-beginMovePoint.y())>0)
	        _presentation->commandMove(event->x()-beginMovePoint.x(),event->y()-beginMovePoint.y());
	}
	beginMove = false;
}
