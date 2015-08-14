/*
 * AddPrimaryKeyCommand.h
 *
 *  Created on: 2013/10/5
 *      Author: User
 */

#ifndef ADDPRIMARYKEYCOMMAND_H_
#define ADDPRIMARYKEYCOMMAND_H_

#include "Command.h"
#include "Compoment.h"
#include "ERModel.h"
#include <list>
#include <vector>

class AddPrimaryKeyCommand: public Command
{
public:
	AddPrimaryKeyCommand(ERModel* model, unsigned int entityID, list<unsigned int> primaryKeyID);
	~AddPrimaryKeyCommand();
	void execute();
	void unexecute();
private:
	ERModel* _Model;
	unsigned int _EntityID;
	vector<Component*> _OldPrimaryKeys;
	list<unsigned int> _NewPrimaryKeyID;
};

#endif /* ADDPRIMARYKEYCOMMAND_H_ */
