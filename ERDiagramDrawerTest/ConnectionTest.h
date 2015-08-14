/*
 * ConnectionTest.h
 *
 *  Created on: 2013/10/21
 *      Author: User
 */

#ifndef CONNECTIONTEST_H_
#define CONNECTIONTEST_H_

#include "gtest/gtest.h"
#include "Connection.h"

class ConnectionTest: public testing::Test
{
protected:
	Connection* _connection;
	void SetUp();
	void TearDown();
};

#endif /* CONNECTIONTEST_H_ */
