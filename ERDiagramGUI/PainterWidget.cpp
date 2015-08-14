#include "PainterWidget.h"

PainterWidget::PainterWidget(PresentationModel* presentation)
{
	_presentation = presentation;
	setAutoFillBackground(true);
	setFocusPolicy(Qt::WheelFocus);
	resize(1980, 1080);
	_mouseClick = false;
}

PainterWidget::~PainterWidget(void)
{
}
//draw
void PainterWidget::paintEvent(QPaintEvent *event)
{
	_presentation->drawComponent(this);
}

//«ö¤U·Æ¹«
void PainterWidget::mousePressEvent(QMouseEvent *event)
{
	_mouseClick = true;
	_presentation->mousePressEvent(event,this);
}

//CTRL
void PainterWidget::keyPressEvent(QKeyEvent *event)
{
	_presentation->pressCtrl(event->key());
}

void PainterWidget::mouseReleaseEvent(QMouseEvent* event)
{
	_presentation->mouseReleaseEvent(event,this);

}

void PainterWidget::mouseMoveEvent(QMouseEvent* event)
{
	_presentation->mouseMoveEvent(event,this);
}

//Release
void PainterWidget::keyReleaseEvent(QKeyEvent *event)
{
	_presentation->releaseCtrl();
	_mouseClick = false;
}

