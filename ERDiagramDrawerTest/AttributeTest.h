/*
 * AttributeTest.h
 *
 *  Created on: 2013/10/21
 *      Author: User
 */

#ifndef ATTRIBUTETEST_H_
#define ATTRIBUTETEST_H_

#include <gtest/gtest.h>
#include "Attribute.h"

class AttributeTest: public testing::Test
{
protected:
	Attribute* _attribute;
	void SetUp();
	void TearDown();
};

#endif /* ATTRIBUTETEST_H_ */
