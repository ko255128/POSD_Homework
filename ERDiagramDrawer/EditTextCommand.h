/*
 * EditTextCommand.h
 *
 *  Created on: 2013/11/29
 *      Author: User
 */

#ifndef EDITTEXTCOMMAND_H_
#define EDITTEXTCOMMAND_H_

#include "Command.h"
#include "ERModel.h"
#include <string>
using namespace std;

class EditTextCommand: public Command
{
public:
	EditTextCommand(ERModel* model,unsigned int id,string oldText,string newText);
	~EditTextCommand();
	void execute();
	void unexecute();
private:
	ERModel* _ERModel;
	string _OldText;
	string _NewText;
	unsigned int _Id;
};

#endif /* EDITTEXTCOMMAND_H_ */
