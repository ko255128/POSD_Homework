/*
 * erdiagramTableView.cpp
 *
 *  Created on: 2013/11/29
 *      Author: User
 */

#include "erdiagramTableView.h"
#include "stringResource.h"
#include <qstring.h>

ErdiagramTableView::ErdiagramTableView(PresentationModel* model)
{
	this->_PresentationModel = model;
	_PresentationModel->getSubject()->attach(this);
	_QStandardItemModel = NULL;
	loadData();
	QHeaderView* headerView = horizontalHeader();
	headerView->setStretchLastSection(true);
}

ErdiagramTableView::~ErdiagramTableView()
{
	_PresentationModel->getSubject()->detach(this);
	if (_QStandardItemModel != NULL)
		delete _QStandardItemModel;
}

void ErdiagramTableView::update()
{
	loadData();
}

void ErdiagramTableView::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
	QTableView::dataChanged(topLeft, bottomRight);
	QStandardItem* textItem = _QStandardItemModel->takeItem(topLeft.row(), topLeft.column());
	if (!_PresentationModel->editTextCmd(topLeft.row(), textItem->text().toStdString()))
	{
		QMessageBox::critical(this, "Input Error", QString::fromStdString("Please input 1 or N"));
		loadData();
	}
}

void ErdiagramTableView::loadContainData(QStandardItemModel* model)
{
	unsigned int numberOfComponent = _PresentationModel->getNumberOfComponent();
	vector<unsigned int> allComponentId = _PresentationModel->getAllComponentID();
	vector<string> allComponentName = _PresentationModel->getAllComponetNames();
	vector<string> allComponentTypeName = _PresentationModel->getAllComponetType();
	for (unsigned int i = 0; i < numberOfComponent; i++)
	{
		unsigned int componentId = allComponentId.at(i);
		string componentName = allComponentName.at(i);
		string componenetType = allComponentTypeName.at(i);
		QList<QStandardItem*> items;
		QStandardItem* idItem = new QStandardItem(QString::number(componentId));
		QStandardItem* typeItem = new QStandardItem(QString(componenetType.c_str()));
		QStandardItem* textItem = new QStandardItem(QString(componentName.c_str()));
		typeItem->setEditable(false);
		textItem->setEditable(_PresentationModel->canInputComponentName(i));
		items << typeItem << textItem;
		model->appendRow(items);
		model->setVerticalHeaderItem(i, idItem);
		items.clear();
	}
}

void ErdiagramTableView::loadData()
{
	QStandardItem* typeItem = new QStandardItem(QString(TITLE_TYPE.c_str()));
	QStandardItem* textItem = new QStandardItem(QString(TITLE_NAME.c_str()));
	QHeaderView* header = horizontalHeader();
	int typeItemSize = header->sectionSize(0);
	int textItemSize = header->sectionSize(1);
	QStandardItemModel* model = new QStandardItemModel();
	loadContainData(model);
	model->setHorizontalHeaderItem(0, typeItem);
	model->setHorizontalHeaderItem(1, textItem);
	header->resizeSection(0, typeItemSize);
	header->resizeSection(1, textItemSize);
	this->setModel(model);
	if(_QStandardItemModel!=NULL)
	    delete _QStandardItemModel;
	_QStandardItemModel = model;

}
