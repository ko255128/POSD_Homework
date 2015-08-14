/*
 * DeleteCommand.h
 *
 *  Created on: 2013/10/5
 *      Author: User
 */

#ifndef DELETECOMMAND_H_
#define DELETECOMMAND_H_

#include "Command.h"
#include "Compoment.h"
#include "ERModel.h"

using namespace std;
class DeleteCommand: public Command
{
public:
	DeleteCommand(ERModel* model, Component* deleteComponent);
	DeleteCommand(ERModel* model, vector<Component*> deleteComponents);
	~DeleteCommand();
	void execute();
	void unexecute();
private:
	ERModel* _Model;
	vector<Component*> _DeleteComponent;
	vector<vector<Component*> > _EntityPrimaryKey;

	void findAttributeEffectEntityPrimayKey(vector<Component*> connections, Component* deleteComponent);
	void findConnectionEffectEntityPrimaryKey(vector<Component*> connections, Component* deleteComponent);
};

#endif /* DELETECOMMAND_H_ */
