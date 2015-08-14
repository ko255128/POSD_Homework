#include "PasteCommand.h"
#include <algorithm>

PasteCommand::PasteCommand(ERModel* model)
{
	this->_Model = model;
	_Clipboard = model->getCilpboard();
	_PastNewNode.clear();
}


PasteCommand::~PasteCommand(void)
{
	while(!_PastNewNode.empty())
	{
		Component* deleteNode = _PastNewNode.back();
		_PastNewNode.pop_back();
		if (deleteNode != NULL)
		{
			
			if (find(_Model->getComponents().begin(),_Model->getComponents().end(),deleteNode)==_Model->getComponents().end())
			{
				delete deleteNode;
			}
		}
	}
}

void PasteCommand::execute()
{
	if(_PastNewNode.empty())
	    _NumberOfPastCreatNode = _Model->paste(_Clipboard);
	else
	{
		for(unsigned int i=0 ;i <_PastNewNode.size();i++)
		{
			_Model->addComponent(_PastNewNode.at(i));
		}
	}
}

void PasteCommand::unexecute()
{
	bool saveFlag = _PastNewNode.empty();
	for(unsigned int i=0;i < _NumberOfPastCreatNode ;i++)
	{
		if(saveFlag)
			_PastNewNode.push_back(_Model->deleteLastComponent());
		else
			_Model->deleteLastComponent();
	}
}
