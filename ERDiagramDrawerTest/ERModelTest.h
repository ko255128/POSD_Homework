/*
 * ERModelTest.h
 *
 *  Created on: 2013/10/22
 *      Author: User
 */

#ifndef ERMODELTEST_H_
#define ERMODELTEST_H_

#include <gtest\gtest.h>
#include "ERModel.h"

class ERModelTest: public testing::Test
{
protected:
	ERModel* _model;
	void SetUp();
	void TearDown();
	void creatTestFile();
};

#endif /* ERMODELTEST_H_ */
