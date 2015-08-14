/*
 * State.h
 *
 *  Created on: 2013/11/15
 *      Author: User
 */

#ifndef STATE_H_
#define STATE_H_
#include <qwidget.h>

class PresentationModel;
class State
{
public:
	State();
	virtual ~State();
	virtual void mousePressEvent(QMouseEvent*,QWidget*)=0;
	virtual void mouseMoveEvent(QMouseEvent*,QWidget*)=0;
	virtual void mouseReleaseEvent(QMouseEvent*,QWidget*)=0;
protected:
	bool _mouseClick;
	PresentationModel* _presentation;
};

#endif /* STATE_H_ */
