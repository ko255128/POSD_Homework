/*
 * ConnectState.h
 *
 *  Created on: 2013/11/15
 *      Author: User
 */

#ifndef CONNECTSTATE_H_
#define CONNECTSTATE_H_

#include <qevent.h>
#include "State.h"
#include "PresentationModel.h"
#include "qpoint.h"

class ConnectState: public State
{
public:
	ConnectState(PresentationModel* presentationModel);
	~ConnectState();
	void mousePressEvent(QMouseEvent*,QWidget*);
	void mouseMoveEvent(QMouseEvent*,QWidget*);
	void mouseReleaseEvent(QMouseEvent*,QWidget*);
private:
	QPoint _firstNodePos;

	void chooseRealtionCardinality(QWidget* widget);
	void excuteConnect(QWidget* widget);
};

#endif /* CONNECTSTATE_H_ */
