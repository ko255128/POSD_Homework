/*
 * CommandManager.h
 *
 *  Created on: 2013/10/5
 *      Author: User
 */

#ifndef COMMANDMANAGER_H_
#define COMMANDMANAGER_H_

#include <stack>
#include "Command.h"

using namespace std;

class CommandManager
{
public:
	CommandManager();
	~CommandManager();
	void excute(Command* excuteCommand);
	bool redo();
	bool undo();
	bool canReDo();
	bool canUnDo();
private:
	stack<Command*> _RedoCommands;
	stack<Command*> _UndoCommands;

	void clearRedoCommands();
};

#endif /* COMMANDMANAGER_H_ */
