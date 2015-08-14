/*
 * Compoment.cpp
 *
 *  Created on: 2013/9/16
 *      Author: User
 */

#include "Compoment.h"
#include <iomanip>
#include "Connection.h"
#include "stringResource.h"

using namespace std;


Component::Component() :
		_id(0), _type(ATTRIBUTE), _name(EMPTY_STRING),_selected(false),_isPreview(false),_posX(0),_posY(0),_width(0),_height(0)
{
}

Component::~Component()
{
	_connectinons.clear();
}

//�N�s���I�s�J�}�C
void Component::connectTo(Component* target)
{
	_connectinons.push_back(target);
}

//�����p��(�Ҧ�Component���|��������)
void Component::hookdisConnectTo(Component* target)
{
	for (vector<Component*>::iterator vectorIterator = _connectinons.begin(); vectorIterator != _connectinons.end(); vectorIterator++)
	{
		if (*vectorIterator == target)
		{
			_connectinons.erase(vectorIterator);
			break;
		}
	}
}

//�ˬd�Ӹ`�I�O�_�w�P�ؼе��I�s��
bool Component::checkAlreadyConnect(Component* target)
{
	for (vector<Component*>::iterator vectorIterator = _connectinons.begin(); vectorIterator != _connectinons.end(); vectorIterator++)
	{
		if ((*vectorIterator)->getType() == CONNECTION)
		{
			Connection* connection = (Connection*) *vectorIterator;
			if (connection->getAnthorConnectedNode(this) == target)
				return true;
		}
	}
	return false;
}
