/*
 * erdiagramTableView.h
 *
 *  Created on: 2013/11/29
 *      Author: User
 */

#ifndef ERDIAGRAMTABLEVIEW_H_
#define ERDIAGRAMTABLEVIEW_H_

#include "Observer.h"
#include "PresentationModel.h"
#include <QtGui>

class ErdiagramTableView: public QTableView, public Observer
{
Q_OBJECT
public:
	ErdiagramTableView(PresentationModel*);
	~ErdiagramTableView();
	void update();
protected slots:
	// QStandardModel資料變更
	void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
private:
	void loadData();
	void loadContainData(QStandardItemModel* model);

	PresentationModel* _PresentationModel;
	QStandardItemModel* _QStandardItemModel;
};

#endif /* ERDIAGRAMTABLEVIEW_H_ */
