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
//�Ѻc�ɱN�Ҧ��bREDO�BUNDO��Command�M��
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
//�M���Ҧ��bRedo��Command
void CommandManager::clearRedoCommands()
{
	while (!_RedoCommands.empty())
	{
		Command* deleteCommand = _RedoCommands.top();
		_RedoCommands.pop();
		delete deleteCommand;
	}
}
//����COMMAND�|�N�bREDO��Command�M���åB�bCommand�s�bundo��
void CommandManager::excute(Command* excuteCommand)
{
	excuteCommand->execute();
	_UndoCommands.push(excuteCommand);
	clearRedoCommands();
}
//�Nredo���̫�@�ӫ��O���X�Ӱ���é��undo��stack��
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
//�Nundo���̫�@�ӫ��O���X�Ӥϰ���é��redo��stack��
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