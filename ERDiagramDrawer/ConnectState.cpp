/*
 * ConnectState.cpp
 *
 *  Created on: 2013/11/15
 *      Author: User
 */

#include "ConnectState.h"
#include <qmessagebox.h>
#include <qinputdialog.h>
#include <qpainter.h>
#include "stringResource.h"

ConnectState::ConnectState(PresentationModel* presentationModel)
{
	this->_presentation = presentationModel;
}

ConnectState::~ConnectState()
{
	// TODO Auto-generated destructor stub
}

void ConnectState::chooseRealtionCardinality(QWidget* widget)
{
	QStringList items;
	items << QString::fromStdString(REALTION_CARDINALITY_ONE) << QString::fromStdString(REALTION_CARDINALITY_N);
	bool ok;
	QString item = QInputDialog::getItem(widget, QString::fromStdString(SELECT_REALTION_CARDINALITY_DIALOG_TITLE), QString::fromStdString(SELECT_REALTION_CARDINALITY_NOTICE), items, 0, false, &ok);
	if (ok && !item.isEmpty())
		_presentation->Addconnection(item.toStdString());
	else
		_presentation->changeState(PresentationModel::POINTER_STATE);
}

void ConnectState::excuteConnect(QWidget* widget)
{
	if (_presentation->checkCanConnect())
	{
		if (_presentation->checkNeedInputRealtionCardinality())
			chooseRealtionCardinality(widget);
		else
		{
			_presentation->Addconnection(EMPTY_STRING);
		}
	}
	else
	{
		QMessageBox::warning(widget, QString::fromStdString(ILLEGAL_CONNECT), QString::fromStdString(_presentation->getConnectErrorMessage()));
		_presentation->changeState(PresentationModel::POINTER_STATE);
	}
}

void ConnectState::mousePressEvent(QMouseEvent* event, QWidget* widget)
{
	if (_mouseClick)
		return;
	_mouseClick = true;
	if (_presentation->selectConnectNode(event->x(), event->y()))
	{
		_firstNodePos = event->pos();
		_presentation->setPrivewLineStartPoint(_firstNodePos);
		_presentation->setPrivewLineEndPoint(_firstNodePos);
		_presentation->setNeedDrawPrivewLine(true);
	}
}

void ConnectState::mouseMoveEvent(QMouseEvent* event, QWidget* widget)
{
	_presentation->setPrivewLineEndPoint(event->pos());
}

void ConnectState::mouseReleaseEvent(QMouseEvent* event, QWidget* widget)
{
	_mouseClick = false;
	if (_presentation->selectConnectNode(event->x(), event->y()) && !_firstNodePos.isNull())
	{
		excuteConnect(widget);
	}
	else
	{
		_presentation->changeState(PresentationModel::POINTER_STATE);
	}
}
