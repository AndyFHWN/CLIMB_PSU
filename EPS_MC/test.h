//
// test.h
//

#ifndef TEST_H_
	#define TEST_H_

	#include "timer.h"

	// define test number to perform test case according to CubeSat_EPS_TestCases.xlsx
	// in test cases 1-7,18 the uC only goes to power save mode
	// in test cases 26,28 the uC only goes to power save mode because burst mode is not implemented yet
	//#define TEST 1

	// function prototypes
	void test(void);
#endif