/*
 * AddNodeState.h
 *
 *  Created on: 2013/11/15
 *      Author: User
 */

#ifndef ADDNODESTATE_H_
#define ADDNODESTATE_H_
#include "PresentationModel.h"
#include "State.h"
#include <qwidget.h>

class AddNodeState:public State
{
public:
	AddNodeState(PresentationModel* presentationModel);
	~AddNodeState();
	void mousePressEvent(QMouseEvent*,QWidget*);
	void mouseMoveEvent(QMouseEvent*,QWidget*);
	void mouseReleaseEvent(QMouseEvent*,QWidget*);
};

#endif /* ADDNODESTATE_H_ */
