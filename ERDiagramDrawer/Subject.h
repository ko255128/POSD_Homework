/*
 * Subject.h
 *
 *  Created on: 2013/11/16
 *      Author: User
 */

#ifndef SUBJECT_H_
#define SUBJECT_H_
#include "Observer.h"
#include <vector>

using namespace std;

class Subject
{
public:
	Subject();
	~Subject();
	void attach(Observer*);
	void detach(Observer*);
	void notify();
private:
	vector<Observer*> _Observers;
};

#endif /* SUBJECT_H_ */
