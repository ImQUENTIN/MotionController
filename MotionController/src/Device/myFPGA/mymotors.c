/*
*  Created on: 2016-12-18
*      Author: QUENTIN
*      E-mail: qiyuexin@yeah.net
*/
//###########################################################################
//
//  FILE:   mymotors.c
//
// TITLE:   FPGA电机部分
//
//###########################################################################


#include "DSP2833x_Device.h"


void testMymotor(void)
{
	int32_t tmp;
	static volatile  int16 rValue;

	MotorRegs[0].MCTL.bit.RST = 1;
	MotorRegs[0].MCTL.bit.ENA = 0;	// disable, and prepare the setting.

	tmp = 6400*5;
	MotorRegs[0].INPL = tmp;
	MotorRegs[0].INPH = tmp >> 16;
	tmp = 64000;
	MotorRegs[0].INVL = tmp;
	MotorRegs[0].INVH = tmp >> 16;
	MotorRegs[0].INAH = 0;
	MotorRegs[0].INAL = 0;
	MotorRegs[0].INJH = 0;
	MotorRegs[0].INJL = 0;

	tmp =30;
	while( tmp-- ){
		rValue = MotorRegs[0].MSTA.all;

		rValue = MotorRegs[0].NOWPH;
		rValue = MotorRegs[0].NOWPL;
		rValue = MotorRegs[0].NOWVH;
		rValue = MotorRegs[0].NOWVL;
		rValue = MotorRegs[0].NOWAH;
		rValue = MotorRegs[0].NOWAL;
		rValue = MotorRegs[0].NOWJH;
		rValue = MotorRegs[0].NOWJL;

	}

}
