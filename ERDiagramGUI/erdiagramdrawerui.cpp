#include "erdiagramdrawerui.h"
#include "stringResource.h"
#include <string>
#include <qinputdialog.h>

using namespace std;
ERDiagramDrawerUI::ERDiagramDrawerUI(PresentationModel* presentationModel)
{
	this->setWindowTitle(QString::fromStdString(PROGRAM_NAME));
	_presentationModel = presentationModel;
	_presentationModel->getSubject()->attach(this);
	_painterWidget = new PainterWidget(_presentationModel);
	_ERdiagramTableView = new ErdiagramTableView(_presentationModel);
	_DBTableView = new dbTableView(_presentationModel);
	_dbDockWidget = new QDockWidget(this);
	_dbDockWidget->setAcceptDrops(false);
	_dbDockWidget->setFeatures(QDockWidget::DockWidgetClosable);
	_dbDockWidget->setWidget(_DBTableView);
	_dbDockWidget->setWindowTitle(QString::fromStdString("Table View"));
	createActions();
	createToolBars();
	createMeuns();
	resize(800, 800);
	this->addDockWidget(Qt::BottomDockWidgetArea,_dbDockWidget);
	_label = new QLabel(this);
	setSpiltters();
	setCentralWidget(_horizonSpiltter);
	setLabel();
	_scrollArea.setWidget(_painterWidget);
	_painterWidget->setFocus();
	QIcon icon(":icons/erdiagram.png");
	setWindowIcon(icon);
	update();
}

ERDiagramDrawerUI::~ERDiagramDrawerUI()
{
	_presentationModel->getSubject()->detach(this);
	delete _openFileAction;
	delete _exitAction;
	delete _pointer;
	delete _addConnection;
	delete _addAttribute;
	delete _addEntity;
	delete _addRelationship;
	delete _painterWidget;
	delete _addMenu;
	delete _fileMenu;
	delete _fileToolBar;
	delete _addToolBar;
	delete _reDo;
	delete _unDo;
	delete _deleteComponent;
	delete _setPrimaryKey;
	delete _label;
	delete _verticalSpiltter;
	delete _saveFileAction;
	delete _saveXmlFileAction;

	delete _cutAction;
	delete _copyAction;
	delete _pasteAction;
	_horizonSpiltter->deleteLater();
	_firstverticalSpiltter->deleteLater();
}

void ERDiagramDrawerUI::setOpenFileAction()
{
	// Open File
	_openFileAction = new QAction(QIcon(":/icons/open.png"), "Open file", this);
	_openFileAction->setShortcut(Qt::CTRL + Qt::Key_O);
	connect(_openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));
}

void ERDiagramDrawerUI::addConnection()
{
	if (!_addConnection->isChecked())
	{
		_addConnection->setChecked(_presentationModel->isConnectButtonChecked());
		return;
	}
	_presentationModel->clearButtonChecked();
	_presentationModel->setConnectButtonChecked(true);
	_presentationModel->changeState(PresentationModel::CONNECT_STATE);
	update();
}

void ERDiagramDrawerUI::addEntity()
{
	if (!_addEntity->isChecked())
	{
		_addEntity->setChecked(_presentationModel->isAddEntityButtonChecked());
		return;
	}
	_presentationModel->clearButtonChecked();
	_presentationModel->setAddEntityButtonChecked(true);
	_presentationModel->changeState(PresentationModel::ADD_NODE_STATE);
	bool ok;
	QString entityName = QInputDialog::getText(this, "Input Entity Name", "Please enter entity name", QLineEdit::Normal, "", &ok);
	if (!ok)
		_presentationModel->changeState(PresentationModel::POINTER_STATE);
	else
		_presentationModel->creatPrivewNode(ENTITY_TYPE_NAME, entityName.toStdString());
	update();
}

void ERDiagramDrawerUI::addRelationship()
{
	if (!_addRelationship->isChecked())
	{
		_addRelationship->setChecked(_presentationModel->isAddRelationshipButtonChecked());
		return;
	}
	_presentationModel->clearButtonChecked();
	_presentationModel->setAddRelationshipButtonChecked(true);
	_presentationModel->changeState(PresentationModel::ADD_NODE_STATE);
	bool ok;
	QString relationshipName = QInputDialog::getText(this, "Input Relation Name", "Please enter relation name", QLineEdit::Normal, "", &ok);
	if (!ok)
		_presentationModel->changeState(PresentationModel::POINTER_STATE);
	else
		_presentationModel->creatPrivewNode(RELATIONSHIP_TYPE_NAME, relationshipName.toStdString());
	update();
}

void ERDiagramDrawerUI::addAttribute()
{
	if (!_addAttribute->isChecked())
	{
		_addAttribute->setChecked(_presentationModel->isAddAttributeButtonChecked());
		return;
	}
	_presentationModel->clearButtonChecked();
	_presentationModel->setAddAttributeButtonChecked(true);
	_presentationModel->changeState(PresentationModel::ADD_NODE_STATE);
	bool ok;
	QString attributeName = QInputDialog::getText(this, "Input Attribute Name", "Please enter Attribute name", QLineEdit::Normal, "", &ok);
	if (!ok)
		_presentationModel->changeState(PresentationModel::POINTER_STATE);
	else
		_presentationModel->creatPrivewNode(ATTRITUBE_TYPE_NAME, attributeName.toStdString());
	update();
}

void ERDiagramDrawerUI::selectPointer()
{
	if (!_pointer->isChecked())
	{
		_pointer->setChecked(_presentationModel->isPointerButtonChecked());
		return;
	}
	_presentationModel->changeState(PresentationModel::POINTER_STATE);
}

void ERDiagramDrawerUI::update()
{
	_setPrimaryKey->setChecked(_presentationModel->isSetPrimaryKeyChecked());
	_saveXmlFileAction->setEnabled(_presentationModel->canSave());
	_copyAction->setEnabled(_presentationModel->isHaveSelect());
	_cutAction->setEnabled(_presentationModel->isHaveSelect());
	_pasteAction->setEnabled(_presentationModel->canPaste());
	_saveFileAction->setEnabled(_presentationModel->canSave());
	_painterWidget->setMouseTracking(_presentationModel->isTrackingMouse());
	_pointer->setChecked(_presentationModel->isPointerButtonChecked());
	_addAttribute->setChecked(_presentationModel->isAddAttributeButtonChecked());
	_addConnection->setChecked(_presentationModel->isConnectButtonChecked());
	_addEntity->setChecked(_presentationModel->isAddEntityButtonChecked());
	_addRelationship->setChecked(_presentationModel->isAddRelationshipButtonChecked());
	_reDo->setEnabled(_presentationModel->canRedo());
	_unDo->setEnabled(_presentationModel->canUndo());
	_deleteComponent->setEnabled(_presentationModel->isHaveSelect());
	_painterWidget->repaint();
}

void ERDiagramDrawerUI::setExitAction()
{
	// Exit
	_exitAction = new QAction(QIcon(":/icons/exit.png"), "Exit", this);
	_exitAction->setShortcut(Qt::CTRL + Qt::Key_X);
	connect(_exitAction, SIGNAL(triggered()), this, SLOT(close()));
}

void ERDiagramDrawerUI::setPointerAction()
{
	//pointer
	_pointer = new QAction(QIcon(":/icons/mouse.png"), "Pointer", this);
	_pointer->setCheckable(true);
	connect(_pointer, SIGNAL(triggered()), this, SLOT(selectPointer()));
}

void ERDiagramDrawerUI::setAddConnectionAction()
{
	_addConnection = new QAction(QIcon(":/icons/connect.png"), "Connect two node", this);
	_addConnection->setCheckable(true);
	connect(_addConnection, SIGNAL(triggered()), this, SLOT(addConnection()));
}

void ERDiagramDrawerUI::setAddAttributeAction()
{
	_addAttribute = new QAction(QIcon(":/icons/ellipse.png"), "Add Attribute", this);
	_addAttribute->setCheckable(true);
	connect(_addAttribute, SIGNAL(triggered()), this, SLOT(addAttribute()));
}

void ERDiagramDrawerUI::setAddEntityAction()
{
	_addEntity = new QAction(QIcon(":/icons/restangle.png"), "Add Entity", this);
	_addEntity->setCheckable(true);
	connect(_addEntity, SIGNAL(triggered()), this, SLOT(addEntity()));
}

void ERDiagramDrawerUI::reDo()
{
	_presentationModel->processRedoForGui();
}

void ERDiagramDrawerUI::unDo()
{
	_presentationModel->processUndoForGui();
}

void ERDiagramDrawerUI::deleteComponent()
{
	_presentationModel->processDeleteComponetFromGui();
}

void ERDiagramDrawerUI::setPrimaryKey()
{
	if (!_setPrimaryKey->isChecked())
	{
		_setPrimaryKey->setChecked(_presentationModel->isSetPrimaryKeyChecked());
		return;
	}
	_presentationModel->clearButtonChecked();
	_presentationModel->setPrimaryKeyButtonChecked(true);
	_presentationModel->changeState(PresentationModel::SET_PRIMARY_KEY_STATE);
}

void ERDiagramDrawerUI::setAddRelationshipAction()
{
	_addRelationship = new QAction(QIcon(":/icons/rhombus.png"), "Add Relationship", this);
	_addRelationship->setCheckable(true);
	connect(_addRelationship, SIGNAL(triggered()), this, SLOT(addRelationship()));
}

void ERDiagramDrawerUI::createActions()
{
	// Open File
	setOpenFileAction();
	// Exit
	setExitAction();
	//pointer
	setPointerAction();
	//ConnectedAction
	setAddConnectionAction();
	setAddAttributeAction();
	setAddEntityAction();
	setAddRelationshipAction();
	setSaveFileAction();
	setCutAction();
	setPasteAction();
	setCopyAction();
	setPrimaryKeyAction();
	_aboutAction = new QAction("About",this);
	connect(_aboutAction,SIGNAL(triggered()), this, SLOT(showAbout()));
	_saveXmlFileAction =  new QAction("Save as XML",this);
	connect(_saveXmlFileAction,SIGNAL(triggered()),this,SLOT(saveXmlFile()));
	_reDo = new QAction(QIcon(":/icons/redo.png"), "Redo", this);
	connect(_reDo, SIGNAL(triggered()), this, SLOT(reDo()));
	_unDo = new QAction(QIcon(":/icons/undo.png"), "Undo", this);
	connect(_unDo, SIGNAL(triggered()), this, SLOT(unDo()));
	_deleteComponent = new QAction(QIcon(":/icons/delete.png"), "Delete", this);
	connect(_deleteComponent, SIGNAL(triggered()), this, SLOT(deleteComponent()));
	_dbDockWidget->toggleViewAction()->setIcon(QIcon(":/icons/displayTable.jpg"));
}

void ERDiagramDrawerUI::createMeuns()
{
	QString fileMenuName = "File";
	QString addMenuName = "Add";
	QString editMenuName = "Edit";
	QString helpMenuName = "Help";
	_fileMenu = menuBar()->addMenu(fileMenuName);
	_fileMenu->addAction(_openFileAction);
	_fileMenu->addSeparator();
	_fileMenu->addAction(_saveFileAction);
	_fileMenu->addAction(_saveXmlFileAction);
	_fileMenu->addAction(_exitAction);
	_addMenu = menuBar()->addMenu(addMenuName);
	_addMenu->addAction(_pointer);
	_addMenu->addSeparator();
	_addMenu->addAction(_addAttribute);
	_addMenu->addAction(_addEntity);
	_addMenu->addAction(_addRelationship);
	_addMenu->addSeparator();
	_addMenu->addAction(_addConnection);
	_addMenu->addSeparator();
	_addMenu->addAction(_setPrimaryKey);
	_editMneu = menuBar()->addMenu(editMenuName);
	_editMneu->addAction(_unDo);
	_editMneu->addAction(_reDo);
	_editMneu->addSeparator();
	_editMneu->addAction(_deleteComponent);
	_editMneu->addSeparator();
	_editMneu->addAction(_cutAction);
	_editMneu->addAction(_copyAction);
	_editMneu->addAction(_pasteAction);
	_helpMenu = menuBar()->addMenu(helpMenuName);
	_helpMenu->addAction(_aboutAction);
}

void ERDiagramDrawerUI::createToolBars()
{
	QString filetoolBar = "fileToolBar";
	QString addtoolBar = "addToolBar";
	_fileToolBar = addToolBar(filetoolBar);
	_addToolBar = addToolBar(addtoolBar);
	// add Action
	_fileToolBar->addAction(_openFileAction);
	_fileToolBar->addAction(_saveFileAction);
	_fileToolBar->addAction(_exitAction);
	_addToolBar->addAction(_unDo);
	_addToolBar->addAction(_reDo);
	_addToolBar->addSeparator();
	_addToolBar->addAction(_deleteComponent);
	_addToolBar->addSeparator();
	_addToolBar->addAction(_cutAction);
	_addToolBar->addAction(_copyAction);
	_addToolBar->addAction(_pasteAction);
	_addToolBar->addSeparator();
	_addToolBar->addAction(_pointer);
	_addToolBar->addSeparator();
	_addToolBar->addAction(_addAttribute);
	_addToolBar->addAction(_addEntity);
	_addToolBar->addAction(_addRelationship);
	_addToolBar->addSeparator();
	_addToolBar->addAction(_addConnection);
	_addToolBar->addSeparator();
	_addToolBar->addAction(_setPrimaryKey);
	_addToolBar->addSeparator();
	_addToolBar->addAction(_dbDockWidget->toggleViewAction());
}

void ERDiagramDrawerUI::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, "DIALOG", "C:\\", "*.erd");
	if (_presentationModel->processLoadFileForGUI(string((const char *) fileName.toLocal8Bit())))
	{
		_painterWidget->repaint();
		this->setWindowTitle(fileName);
	}
	else if (!fileName.isEmpty())
	{
		QMessageBox::critical(this, "Can't load file!", QString::fromStdString(FILE_FORMAT_ERROR_NOTICE));
	}
}

void ERDiagramDrawerUI::setSpiltters()
{
	_firstverticalSpiltter = new QSplitter(Qt::Vertical);
	_horizonSpiltter = new QSplitter(Qt::Horizontal);
	_verticalSpiltter = new QSplitter(Qt::Vertical);

	_firstverticalSpiltter->addWidget(&_scrollArea);
	_firstverticalSpiltter->addWidget(_dbDockWidget);
	_horizonSpiltter->addWidget(_firstverticalSpiltter);
	_horizonSpiltter->addWidget(_verticalSpiltter);
	_horizonSpiltter->setStretchFactor(0, 70);
	_verticalSpiltter->addWidget(_label);
	_verticalSpiltter->addWidget(_ERdiagramTableView);
}

void ERDiagramDrawerUI::setLabel()
{
	_label->setText(QString::fromStdString(COMPONENT_LABEL));
	_label->setAlignment(Qt::AlignCenter);
	_label->setStyleSheet("QLabel { background-color : lightgray; color : black; font: bold 14px;}");
}

void ERDiagramDrawerUI::saveFile()
{
	QString fileName = QFileDialog::getSaveFileName(this, "DIALOG", "C:\\", "*.erd");
	if(fileName.isEmpty())
		return;
	if(!_presentationModel->saveFileWithGUI(fileName.toStdString()))
		QMessageBox::critical(this, "Can't save file!", QString::fromStdString("Can't save file!"));
	else
		QMessageBox::information(this, "save succeed!", QString::fromStdString("save succeed"));
}

void ERDiagramDrawerUI::setSaveFileAction()
{
	_saveFileAction = new QAction(QIcon(":/icons/savefile.png"),"Save",this);
	connect(_saveFileAction,SIGNAL(triggered()),this,SLOT(saveFile()));
	_saveFileAction->setShortcut(Qt::CTRL + Qt::Key_S);
}

void ERDiagramDrawerUI::saveXmlFile()
{
	QString fileName = QFileDialog::getSaveFileName(this, "DIALOG", "C:\\", "*.xml");
	if(fileName.isEmpty())
		return;
	if(!_presentationModel->saveXmlFile(fileName.toStdString()))
		QMessageBox::critical(this, "Can't save file!", QString::fromStdString("Can't save file!"));
	else
		QMessageBox::information(this, "save succeed!", QString::fromStdString("save xml succeed"));
}

void ERDiagramDrawerUI::copy()
{
	_presentationModel->copy();
}

void ERDiagramDrawerUI::paste()
{
	_presentationModel->paste();
}

void ERDiagramDrawerUI::cut()
{
	_presentationModel->cut();
}

void ERDiagramDrawerUI::setCutAction()
{
	_cutAction = new QAction(QIcon(":/icons/cut.png"),"Cut",this);
	connect(_cutAction,SIGNAL(triggered()),this,SLOT(cut()));
	_cutAction->setShortcut(Qt::CTRL + Qt::Key_X);
}

void ERDiagramDrawerUI::setPasteAction()
{
	_pasteAction = new QAction(QIcon(":/icons/paste.png"),"Paste",this);
	connect(_pasteAction,SIGNAL(triggered()),this,SLOT(paste()));
	_pasteAction->setShortcut(Qt::CTRL+Qt::Key_V);
}

void ERDiagramDrawerUI::setCopyAction()
{
	_copyAction = new QAction(QIcon(":/icons/copy.png"),"Copy",this);
	connect(_copyAction,SIGNAL(triggered()),this,SLOT(copy()));
	_copyAction->setShortcut(Qt::CTRL+Qt::Key_C);
}

void ERDiagramDrawerUI::setPrimaryKeyAction()
{
	_setPrimaryKey = new QAction(QIcon(":/icons/primarykey.png"), "Set Primary Key", this);
	_setPrimaryKey->setCheckable(true);
	connect(_setPrimaryKey, SIGNAL(triggered()), this, SLOT(setPrimaryKey()));
}

void ERDiagramDrawerUI::showAbout()
{

	QMessageBox::about(this, QString::fromStdString(ABOUT_PROGRAM),QString::fromStdString(ABOUT_CONTEXT));
}

void ERDiagramDrawerUI::closeEvent( QCloseEvent* event )
{
	int reply = QMessageBox::Close;
	if(_presentationModel->canSave())
	{
		reply = QMessageBox::warning(this, QString::fromStdString(PROGRAM_NAME), "Do you want to save ?", 
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		if(reply == QMessageBox::Save)
			saveFile();
		if(reply == QMessageBox::Cancel)
			event->ignore();
	}
	
}
