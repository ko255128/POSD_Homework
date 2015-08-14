/*
 * Subject.cpp
 *
 *  Created on: 2013/11/16
 *      Author: User
 */

#include "Subject.h"
#include <algorithm>

using namespace std;

Subject::Subject()
{
	// TODO Auto-generated constructor stub

}

Subject::~Subject()
{
	// TODO Auto-generated destructor stub
}

void Subject::attach(Observer* observer)
{
	_Observers.push_back(observer);
}

void Subject::detach(Observer* observer)
{
	vector<Observer*>::iterator observerIterator = find(_Observers.begin(),_Observers.end(),observer);
	if(observerIterator!=_Observers.end())
		_Observers.erase(observerIterator);
}

void Subject::notify()
{
	for(vector<Observer*>::iterator observerIterator=_Observers.begin();observerIterator!=_Observers.end();observerIterator++)
	{
		(*observerIterator)->update();
	}
}
