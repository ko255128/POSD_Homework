/*
 * ComponentTest.h
 *
 *  Created on: 2013/10/22
 *      Author: User
 */

#ifndef COMPONENTTEST_H_
#define COMPONENTTEST_H_

#include <gtest\gtest.h>
#include "Compoment.h"

class ComponentTest: public testing::Test
{
protected:
	Component* _component;
	void SetUp();
	void TearDown();
};

#endif /* COMPONENTTEST_H_ */
