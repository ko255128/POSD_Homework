#pragma once
#include "state.h"
#include <qevent.h>

class setPrimaryKeyState :
	public State
{
public:
	setPrimaryKeyState(PresentationModel* presentationModel);
	~setPrimaryKeyState(void);
	void mousePressEvent(QMouseEvent*,QWidget*);
	void mouseMoveEvent(QMouseEvent*,QWidget*);
	void mouseReleaseEvent(QMouseEvent*,QWidget*);
};

