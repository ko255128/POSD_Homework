#ifndef MOVECOMMAND_H_
#define MOVECOMMAND_H_

#include "command.h"
#include "ERModel.h"
class MoveCommand :
	public Command
{
public:
	MoveCommand(ERModel* model,vector<unsigned int> moveNodes,int deltaX,int deltaY);
	~MoveCommand(void);
	void execute();
	void unexecute();
private:
	ERModel* _Model;
    vector<unsigned int> _MoveNodes;
	int _DeltaX;
	int _DeltaY;
	bool _FirstExecute;
};
#endif
