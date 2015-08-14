/*
 * Integration.h
 *
 *  Created on: 2013/10/23
 *      Author: User
 */

#ifndef INTEGRATION_H_
#define INTEGRATION_H_

#include <gtest/gtest.h>
#include "ERModel.h"

class IntegrationTest: public testing::Test
{
protected:
	ERModel* _model;
	void SetUp();
	void TearDown();
	void creatTestFile();
};

#endif /* INTEGRATION_H_ */
