//#########################################################################
//		EPS_CC1 Globals
//#########################################################################
volatile unsigned char CC1Mode=CC1_FLIGHT; 	//global EPS_CC1 status (0..safe mode // 1..flight mode)
volatile unsigned char CC1ModeChanged=0;	//global trigger variable when cc1mode is changed
volatile unsigned char CC1Measure=1;		//global EPS_CC1 measure trigger
volatile unsigned char CC1RspPending=0;		//global EPS_CC1 UART Acknowledge is pending
volatile unsigned char MCTimeout=0;			//global Main Controller Timeout flag
volatile unsigned char OSR=0;				//Output status register
volatile unsigned char RegPointer=0; 		//Register Pointer
volatile unsigned char i2cTtcRunning = 0;	//I2C TTC running flag
volatile unsigned char i2cObcRunning = 0;	//I2C TTC running flag
volatile unsigned char txVectorPos = 0; 	//tx position of response vector
volatile unsigned char initFlag = 0;		//flag that indicates first initialization of cmd vector
volatile unsigned char TmpVector[DATA_MAX+1] = {0};					// temporare data vector for uart communication

#pragma PERSISTENT(startUpCnt)
unsigned char startUpCnt = 0;
#pragma PERSISTENT(status)
unsigned char status = 0x40; //Flight Mode
#pragma PERSISTENT(bootcounter)
unsigned char bootcounter = 0;
#pragma PERSISTENT(crossPowerStatus)
unsigned char crossPowerStatus = 0;

volatile unsigned char DATA_REG[88] = {0};	//	88 Byte Register(Char): DATA_REG[88]
											//
											//		00-63 			... Houskeeping data of EPS (voltages, temperatures, statuses)
											//		64-86			... Settings and power requests (Limits and Force Output Registers)
											//
											//		for CC1:
											//			79			... CC1 Force Output Value Register
											//			80			... CC1 Output Value Register

volatile unsigned char RspVector[31] = {0};	// 	Response vector to MC with all his requests from the OBC
											//	0,1			 	... '$','A'
											//	2-25	64-87   ... OBC Requests
											//	26		54		... Reboot Counter CC1
											//  27		56		...	Vcc_CC1
											//	28		57		... Temp_CC1
											//	29		60		... Status_CC1
											//	30 				... Checksum
