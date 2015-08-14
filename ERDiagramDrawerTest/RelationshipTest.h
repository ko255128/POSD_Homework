/*
 * RelationshipTest.h
 *
 *  Created on: 2013/10/21
 *      Author: User
 */

#ifndef RELATIONSHIPTEST_H_
#define RELATIONSHIPTEST_H_

#include "gtest\gtest.h"
#include "Relationship.h"

class RelationshipTest: public testing::Test
{
public:
	Relationship* _relationship;
	void SetUp();
	void TearDown();
};

#endif /* RELATIONSHIPTEST_H_ */
