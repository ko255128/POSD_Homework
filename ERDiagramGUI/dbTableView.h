#ifndef DBTABLEVIEW_H_
#define DBTABLEVIEW_H_

#include "Observer.h"
#include "PresentationModel.h"
#include <QtGui>

class dbTableView :public QTableView, public Observer
{
public:
	dbTableView(PresentationModel* model);
	~dbTableView(void);
	void update();
	void loadData();
private:
	PresentationModel* _Model;
	QStandardItemModel _QStandarItemModel;
	void loadAttributesAndPrimaryKeys(unsigned int entityID);
	void loadForeginKeys( unsigned int entityID, QList<QStandardItem*> &items );

};
#endif
