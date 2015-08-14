#ifndef PASTECOMMAND_H_
#define PASTECOMMAND_H_

#include "command.h"
#include "ERModel.h"

class PasteCommand :
	public Command
{
public:
	PasteCommand(ERModel* model);
	~PasteCommand(void);
	void execute();
	void unexecute();
private:
	ERModel* _Model;
	unsigned int _NumberOfPastCreatNode;
	vector<Component*> _PastNewNode;
	vector<Component*> _Clipboard;
};
#endif

