#include "dbTableView.h"


dbTableView::dbTableView(PresentationModel* model)
{
	this->_Model = model;
	this->setShowGrid(false);
	this->horizontalHeader()->setVisible(false);
	this->verticalHeader()->setVisible(false);
	this->setModel(&_QStandarItemModel);
	_Model->getSubject()->attach(this);
}


dbTableView::~dbTableView(void)
{
	_Model->getSubject()->detach(this);
	_QStandarItemModel.deleteLater();
}

void dbTableView::loadData()
{
	_QStandarItemModel.clear();
	vector<unsigned int> entityIDs = _Model->getAllEntityID();
	QList<QStandardItem*> items;
	for(unsigned int i=0 ; i<entityIDs.size();i++)
	{
		unsigned int entityID = entityIDs.at(i);
		QStandardItem* item = new QStandardItem(QString::fromStdString(_Model->getComponentName(entityID)));
		item->setEditable(false);
		items << item;
		_QStandarItemModel.appendRow(items);
		items.clear();
		loadAttributesAndPrimaryKeys(entityID);
	}
}

void dbTableView::update()
{
	this->loadData();
}

void dbTableView::loadAttributesAndPrimaryKeys(unsigned int entityID)
{
	QList<QStandardItem*> items;
	vector<unsigned int> attributesIDs = _Model->getAllEntityOfAttributeID(entityID);
	if(attributesIDs.empty())
		return;
	for(unsigned int attributeIndex =0 ; attributeIndex < attributesIDs.size();attributeIndex++)
	{
		unsigned int attributeID = attributesIDs.at(attributeIndex);
		QStandardItem* item =  new QStandardItem(QString::fromStdString(_Model->getComponentName(attributeID)));
		item->setBackground(QBrush(QPixmap(":/icons/table.png")));
		item->setEditable(false);
		if(_Model->isPrimaryKey(entityID,attributeID))
			item->setIcon(QIcon(":/icons/pk.png"));
		items << item;
	}
	loadForeginKeys(entityID, items);
	return;
}

void dbTableView::loadForeginKeys( unsigned int entityID, QList<QStandardItem*> &items )
{
	vector<unsigned int> foreginKeys = _Model->getForeginKeyIDs(entityID);
	if(foreginKeys.empty())
	{
		_QStandarItemModel.appendRow(items);
		return;
	}
	for(unsigned int foreginKeyIndex = 0 ; foreginKeyIndex < foreginKeys.size();foreginKeyIndex++)
	{
		unsigned int foreginKeyID = foreginKeys.at(foreginKeyIndex);
		QStandardItem* item =  new QStandardItem(QString::fromStdString(_Model->getComponentName(foreginKeyID)));
		item->setBackground(QBrush(QPixmap(":/icons/table.png")));
		item->setEditable(false);
		item->setIcon(QIcon(":/icons/fk.png"));
		items << item;
	}
	_QStandarItemModel.appendRow(items);
	items.clear();
}
