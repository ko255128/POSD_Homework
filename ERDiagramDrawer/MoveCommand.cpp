#include "MoveCommand.h"


MoveCommand::MoveCommand(ERModel* model,vector<unsigned int> moveNodes,int deltaX,int deltaY)
{
	_Model = model;
	_DeltaX = deltaX;
	_DeltaY = deltaY;
	_MoveNodes = moveNodes;
	_FirstExecute = true;
}


MoveCommand::~MoveCommand(void)
{
}

void MoveCommand::execute()
{
	if(!_FirstExecute)
	{
		_Model->moveComponent(_MoveNodes,_DeltaX,_DeltaY);
	}
	_FirstExecute = false;
}

void MoveCommand::unexecute()
{
	_Model->moveComponent(_MoveNodes,-_DeltaX,-_DeltaY);
}
