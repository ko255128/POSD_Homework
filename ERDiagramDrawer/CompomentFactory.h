/*
 * CompomentFactory.h
 *
 *  Created on: 2013/9/17
 *      Author: User
 */

#ifndef COMPOMENTFACTORY_H_
#define COMPOMENTFACTORY_H_

#include "Compoment.h"

class CompomentFactory
{
public:
	CompomentFactory();
	~CompomentFactory();
	Component* createComponent(Component::COMPOMENT_TYPE type);

};

#endif /* COMPOMENTFACTORY_H_ */
