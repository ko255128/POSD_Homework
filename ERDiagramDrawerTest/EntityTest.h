/*
 * EntityTest.h
 *
 *  Created on: 2013/10/21
 *      Author: User
 */

#ifndef ENTITYTEST_H_
#define ENTITYTEST_H_

#include "gtest\gtest.h"
#include "Entity.h"

class EntityTest: public testing::Test
{
protected:
	Entity* _entity;
	void SetUp();
	void TearDown();
};

#endif /* ENTITYTEST_H_ */
