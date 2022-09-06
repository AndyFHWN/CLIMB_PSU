//---------------------------------------------------------
// Title:		test.c
//
// Author:		Patrick Kappl
//
// Creation:	31.01.2015
//
// Description:	the function test() performs the correct
//				tasks for the different test procedures in
//				CubeSat_EPS_TestCases.xlsx if TEST in
//				test.h is defined as the right test number
//---------------------------------------------------------

#include "test.h"

void test(void)
{
	#if TEST == 8
		// activate heater 1
		PORT_EN_HEATER_1 |= 1<<EN_HEATER_1;
	#endif
	#if TEST == 9
		// activate heater 2
		PORT_EN_HEATER_2 |= 1<<EN_HEATER_2;
	#endif
	#if TEST == 10
		// enable FET 1-1
		PORT_EN_FET_1_1 |= 1<<EN_FET_1_1;
	#endif
	#if TEST == 11
		// enable FET 1-1
		PORT_EN_FET_1_1 |= 1<<EN_FET_1_1;
	#endif
	#if TEST == 12
		// enable FET 1-2
		PORT_EN_FET_1_2 |= 1<<EN_FET_1_2;
	#endif
	#if TEST == 13
		// enable FET 1-2
		PORT_EN_FET_1_2 |= 1<<EN_FET_1_2;
	#endif
	#if TEST == 14
		// enable FET 2-1
		PORT_EN_FET_2_1 |= 1<<EN_FET_2_1;
	#endif
	#if TEST == 15
		// enable FET 2-1
		PORT_EN_FET_2_1 |= 1<<EN_FET_2_1;
	#endif
	#if TEST == 16
		// enable FET 2-2
		PORT_EN_FET_2_2 |= 1<<EN_FET_2_2;
	#endif
	#if TEST == 17
		// enable FET 2-2
		PORT_EN_FET_2_2 |= 1<<EN_FET_2_2;
	#endif
	#if TEST == 19
		// enable FET 5-1
		PORT_EN_FET_5_1 |= 1<<EN_FET_5_1;
	#endif
	#if TEST == 20
		// enable FET 5-2
		PORT_EN_FET_5_2 |= 1<<EN_FET_5_2;
	#endif
	#if TEST == 21
		// enable FET 3-1
		PORT_EN_FET_3_1 |= 1<<EN_FET_3_1;
	#endif
	#if TEST == 22
		// enable FET 3-2
		PORT_EN_FET_3_2 |= 1<<EN_FET_3_2;
	#endif
	#if TEST == 23
		// enable FET 4-1
		PORT_EN_FET_4_1 |= 1<<EN_FET_4_1;
	#endif
	#if TEST == 24
		// enable FET 4-2
		PORT_EN_FET_4_2 |= 1<<EN_FET_4_2;
	#endif
	#if TEST == 25
		// enable 3V3
		PORT_EN_FET_5_1 |= 1<<EN_FET_5_1;		// actually either 5-1 or 5-2 should be enabled
		PORT_EN_FET_5_2 |= 1<<EN_FET_5_2;
		PORT_EN_3V3 |= 1<<EN_3V3;
	#endif
	#if TEST == 26
		// automatic switching to burst mode not implemented yet
	#endif
	#if TEST == 27
		// enable 5V
		PORT_EN_FET_3_1 |= 1<<EN_FET_3_1;		// actually either 3-1 or 3-2 should be enabled
		PORT_EN_FET_3_2 |= 1<<EN_FET_3_2;
		PORT_EN_5V |= 1<<EN_5V;
	#endif
	#if TEST == 28
		// automatic switching to burst mode not implemented yet
	#endif
	#if TEST == 29
		// enable 3V3 backup
		PORT_EN3V3_BACKUP |= 1<<EN3V3_BACKUP;
	#endif
	#if (TEST == 30)||(TEST == 33)||(TEST == 36)
		// enable 3V3-1
		PORT_EN_FET_5_1 |= 1<<EN_FET_5_1;
		PORT_EN_FET_5_2 |= 1<<EN_FET_5_2;
		PORT_EN_3V3 |= 1<<EN_3V3;
		PORT_EN3V3_A |= 1<<EN3V3_A;					// not sure if 3V3-A = 3V3-1
	#endif
	#if (TEST == 31)||(TEST == 34)||(TEST == 37)
		// enable 3V3-2
		PORT_EN_FET_5_1 |= 1<<EN_FET_5_1;
		PORT_EN_FET_5_2 |= 1<<EN_FET_5_2;
		PORT_EN_3V3 |= 1<<EN_3V3;
		PORT_EN3V3_B |= 1<<EN3V3_B;					// not sure if 3V3-B = 3V3-2
	#endif
	#if (TEST == 32)||(TEST == 35)||(TEST == 38)
		// enable 3V3-3
		PORT_EN_FET_5_1 |= 1<<EN_FET_5_1;
		PORT_EN_FET_5_2 |= 1<<EN_FET_5_2;
		PORT_EN_3V3 |= 1<<EN_3V3;
		PORT_EN3V3_D |= 1<<EN3V3_D;					// not sure if 3V3-D = 3V3-3
	#endif
	#if (TEST == 39)||(TEST == 43)||(TEST == 47)
		// enable 5V-1
		PORT_EN_FET_3_1 |= 1<<EN_FET_3_1;
		PORT_EN_FET_3_2 |= 1<<EN_FET_3_2;
		PORT_EN_5V |= 1<<EN_5V;
		PORT_EN5V_A |= 1<<EN5V_A;					// not sure if 5V-A = 5V-1
	#endif
	#if (TEST == 40)||(TEST == 44)||(TEST == 48)
		// enable 5V-2
		PORT_EN_FET_3_1 |= 1<<EN_FET_3_1;
		PORT_EN_FET_3_2 |= 1<<EN_FET_3_2;
		PORT_EN_5V |= 1<<EN_5V;
		PORT_EN5V_B |= 1<<EN5V_B;					// not sure if 5V-B = 5V-2
	#endif
	#if (TEST == 41)||(TEST == 45)||(TEST == 49)
		// enable 5V-3
		PORT_EN_FET_3_1 |= 1<<EN_FET_3_1;
		PORT_EN_FET_3_2 |= 1<<EN_FET_3_2;
		PORT_EN_5V |= 1<<EN_5V;
		PORT_EN5V_C |= 1<<EN5V_C;					// not sure if 5V-C = 5V-3
	#endif
	#if (TEST == 42)||(TEST == 46)||(TEST == 50)
		// enable 5V-4
		PORT_EN_FET_3_1 |= 1<<EN_FET_3_1;
		PORT_EN_FET_3_2 |= 1<<EN_FET_3_2;
		PORT_EN_5V |= 1<<EN_5V;
		PORT_EN5V_D |= 1<<EN5V_D;					// not sure if 5V-D = 5V-4
	#endif

	cli();								// clear global interrupt enable flag
	EnterADCNoiseReduction(0);			// go to adc noise reduction mode and never wake up
}