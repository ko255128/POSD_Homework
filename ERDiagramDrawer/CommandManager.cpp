/*
 * CommandManager.cpp
 *
 *  Created on: 2013/10/5
 *      Author: User
 */

#include "CommandManager.h"

CommandManager::CommandManager()
{

}
//解構時將所有在REDO、UNDO的Command清除
CommandManager::~CommandManager()
{
	clearRedoCommands();
	while (!_UndoCommands.empty())
	{
		Command* deleteCommand = _UndoCommands.top();
		_UndoCommands.pop();
		delete deleteCommand;
	}
}
//清除所有在Redo的Command
void CommandManager::clearRedoCommands()
{
	while (!_RedoCommands.empty())
	{
		Command* deleteCommand = _RedoCommands.top();
		_RedoCommands.pop();
		delete deleteCommand;
	}
}
//執行COMMAND會將在REDO的Command清除並且在Command存在undo中
void CommandManager::excute(Command* excuteCommand)
{
	excuteCommand->execute();
	_UndoCommands.push(excuteCommand);
	clearRedoCommands();
}
//將redo的最後一個指令取出來執行並放到undo的stack中
bool CommandManager::redo()
{
	if (_RedoCommands.empty())
		return false;
	Command* redoCommand = _RedoCommands.top();
	_RedoCommands.pop();
	_UndoCommands.push(redoCommand);
	redoCommand->execute();
	return true;
}
//將undo的最後一個指令取出來反執行並放到redo的stack中
bool CommandManager::undo()
{
	if (_UndoCommands.empty())
		return false;
	Command* undoCommand = _UndoCommands.top();
	_UndoCommands.pop();
	_RedoCommands.push(undoCommand);
	undoCommand->unexecute();
	return true;
}

bool CommandManager::canReDo()
{
	return !_RedoCommands.empty();
}

bool CommandManager::canUnDo()
{
	return !_UndoCommands.empty();
}