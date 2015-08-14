/*
 * AddCommand.h
 *
 *  Created on: 2013/10/5
 *      Author: User
 */

#ifndef ADDCOMMAND_H_
#define ADDCOMMAND_H_

#include <string>
#include "ERModel.h"
#include "Compoment.h"
#include "Command.h"

using namespace std;

class AddCommand: public Command
{
public:
	AddCommand(ERModel* model, Component::COMPOMENT_TYPE type, const string name);
	AddCommand(ERModel* model, Component* component);
	~AddCommand();
	void execute();
	void unexecute();

private:
	Component::COMPOMENT_TYPE _Type;
	string _Name;
	ERModel* _Model;
	Component* _UndoAddNode;
	int _posX;
	int _posY;
	int _height;
	int _width;
};

#endif /* ADDCOMMAND_H_ */
