/*
 * AddNodeState.cpp
 *
 *  Created on: 2013/11/15
 *      Author: User
 */

#include "AddNodeState.h"
#include "PresentationModel.h"
#include <qevent.h>

AddNodeState::AddNodeState(PresentationModel* presentationModel)
{
	this->_presentation = presentationModel;
}

AddNodeState::~AddNodeState()
{
	// TODO Auto-generated destructor stub
}

void AddNodeState::mousePressEvent(QMouseEvent* event, QWidget* widget)
{
	_mouseClick = true;
}

void AddNodeState::mouseMoveEvent(QMouseEvent* event, QWidget* widget)
{

	_presentation->setPrivewNodePosistion(event->x(),event->y());
}

void AddNodeState::mouseReleaseEvent(QMouseEvent* event, QWidget* widget)
{
	if(_mouseClick == true)
	{
		_presentation->setPrivewNodePosistion(event->x(),event->y());
		_presentation->AddNode();
		_presentation->changeState(PresentationModel::POINTER_STATE);
	}
	_mouseClick = false;
}
