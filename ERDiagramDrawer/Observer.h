/*
 * Observer.h
 *
 *  Created on: 2013/11/16
 *      Author: User
 */

#ifndef OBSERVER_H_
#define OBSERVER_H_

class Observer
{
public:
	Observer();
	virtual ~Observer();
	virtual void update()=0;
};

#endif /* OBSERVER_H_ */
