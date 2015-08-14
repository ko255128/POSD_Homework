/*
 * PointerState.h
 *
 *  Created on: 2013/11/15
 *      Author: User
 */

#ifndef POINTERSTATE_H_
#define POINTERSTATE_H_

#include "State.h"
#include <qwidget.h>
#include <QtGui/QMouseEvent>


class PointerState: public State
{
public:
	PointerState(PresentationModel* presentationModel);
	~PointerState();
	void mousePressEvent(QMouseEvent*,QWidget*);
	void mouseMoveEvent(QMouseEvent*,QWidget*);
	void mouseReleaseEvent(QMouseEvent*,QWidget*);
private:
	bool beginMove;

	QPoint begingPoint;
	QPoint beginMovePoint;
};

#endif /* POINTERSTATE_H_ */
