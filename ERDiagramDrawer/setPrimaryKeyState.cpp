#include "setPrimaryKeyState.h"
#include "PresentationModel.h"


setPrimaryKeyState::setPrimaryKeyState(PresentationModel* presentationModel)
{
	this->_presentation = presentationModel;
	this->_mouseClick = false;
}


setPrimaryKeyState::~setPrimaryKeyState(void)
{
}

void setPrimaryKeyState::mousePressEvent( QMouseEvent* event ,QWidget*  widget)
{
	if(!_mouseClick)
	{
		_mouseClick = true;
		_presentation->setPrimaryKey(event->x(),event->y());
		_presentation->changeState(PresentationModel::POINTER_STATE);
	}
}

void setPrimaryKeyState::mouseMoveEvent(QMouseEvent* event ,QWidget*  widget )
{

}

void setPrimaryKeyState::mouseReleaseEvent(QMouseEvent* event ,QWidget*  widget)
{
	_mouseClick = false;
}
