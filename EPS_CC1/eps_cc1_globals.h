//#########################################################################
//# InterruptVectors_init.c
//#
//# Autor: 	Christian Faustmann
//# Function:	EPS - Communication Controller 1 - interrupt service routines
//#
//# VERSION HISTORY:
//#
//# - 03.04.2015 Created
//#	- 04.04.2015 added RspVector, MCTimeout
//#
//#########################################################################

#ifndef EPS_CC1_GLOBALS_H_
#define EPS_CC1_GLOBALS_H_


//#########################################################################
//		Extern definitions of globals
//#########################################################################
extern volatile unsigned char CC1Mode;
extern volatile unsigned char CC1ModeChanged;
extern volatile unsigned char CC1Measure;
extern volatile unsigned char CC1RspPending;
extern volatile unsigned char DATA_REG[88];
extern volatile unsigned char RspVector[31];
extern volatile unsigned char MCTimeout;
extern volatile unsigned char OSR;
extern volatile unsigned char RegPointer;
extern volatile unsigned char i2cTtcRunning;
extern volatile unsigned char i2cObcRunning;
extern volatile unsigned char txVectorPos;
extern volatile unsigned char initFlag;
extern volatile unsigned char TmpVector[DATA_MAX+1];

extern unsigned char startUpCnt;
extern unsigned char status;
extern unsigned char bootcounter;
extern unsigned char crossPowerStatus;

#endif /* EPS_CC1_GLOBALS_H_ */
