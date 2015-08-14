#ifndef PAINTER_M
#define PAINTER_M

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QAction>
#include <QtGui/QScrollBar>
#include <QtGui/QMouseEvent>
#include "PresentationModel.h"

class PainterWidget: public QWidget
{
public:
	PainterWidget(PresentationModel*);
	~PainterWidget(void);

protected:
	void paintEvent(QPaintEvent*);
	void mouseReleaseEvent(QMouseEvent*);
	void mousePressEvent(QMouseEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);
	void keyReleaseEvent(QKeyEvent*);
private:
	PresentationModel *_presentation;
	bool _mouseClick;
};

#endif
