#ifndef ERDIAGRAMDRAWERUI_H
#define ERDIAGRAMDRAWERUI_H

#include <QtGui/QMainWindow>
#include <QtGui/QScrollArea>
#include <QtGui/QMenuBar>
#include <QtGui/QToolBar>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/qsplitter.h>
#include <QtGui/qlabel.h>
#include "Observer.h"
#include "PainterWidget.h"
#include "PresentationModel.h"
#include "erdiagramTableView.h"
#include "dbTableView.h"


class ERDiagramDrawerUI: public QMainWindow,public Observer
{
Q_OBJECT

public:
	 ERDiagramDrawerUI(PresentationModel* presentationModel);
	 ~ERDiagramDrawerUI();
	void closeEvent(QCloseEvent* event);
	void update();
private slots:
	void openFile();
	void addConnection();
	void addEntity();
	void addRelationship();
	void addAttribute();
	void selectPointer();
	void reDo();
	void unDo();
	void deleteComponent();
	void setPrimaryKey();
	void saveFile();
	void saveXmlFile();
	void copy();
	void paste();
	void cut();
	void showAbout();

private:

	QDockWidget *_dbDockWidget;
	QAction *_openFileAction;
	QAction *_exitAction;
	QAction *_pointer;
	QAction *_addConnection;
	QAction *_addAttribute;
	QAction *_addEntity;
	QAction *_addRelationship;
	QAction *_saveFileAction;
	QAction *_saveXmlFileAction;

	QAction *_cutAction;
	QAction *_copyAction;
	QAction *_pasteAction;

	QAction *_reDo;
	QAction *_unDo;
	QAction *_deleteComponent;
	QAction *_setPrimaryKey;

	QAction *_aboutAction;

	QMenu *_addMenu;
	QMenu *_fileMenu;
	QMenu *_editMneu;
	QMenu *_helpMenu;
	QToolBar *_fileToolBar;
	QToolBar *_addToolBar;
	QLabel* _label;
	PainterWidget *_painterWidget;
	QScrollArea _scrollArea;
	PresentationModel *_presentationModel;
	QSplitter* _firstverticalSpiltter;
	QSplitter* _horizonSpiltter;
	QSplitter* _verticalSpiltter;
	ErdiagramTableView* _ERdiagramTableView;
	dbTableView* _DBTableView;
	void setOpenFileAction();
	void setExitAction();
	void clearButtonChecked();
	void setPointerAction();
	void setAddConnectionAction();
	void setAddAttributeAction();
	void setAddEntityAction();
	void setAddRelationshipAction();
	void setSpiltters();
	void setLabel();
	void createActions();

	void setPrimaryKeyAction();

	void setCopyAction();

	void setPasteAction();

	void setCutAction();

	void setSaveFileAction();
	void createMeuns();
	void createToolBars();
};


#endif // ERDIAGRAMDRAWERUI_H
