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

void M_SetDDA(struct DDA_SET_VARS dat){

	MotorRegs[0].INPL = dat.inpos;
	MotorRegs[0].INPH = dat.inpos >> 16;

	MotorRegs[0].INVL = dat.invel;
	MotorRegs[0].INVH = dat.invel >> 16;

	MotorRegs[0].INAL = dat.inacc;
	MotorRegs[0].INAH = dat.inacc >> 16;

	MotorRegs[0].INJL = dat.injerk;
	MotorRegs[0].INJH = dat.injerk >> 16;
}
void testMymotor(void)
{

	struct DDA_SET_VARS dda_set ;

	MotorRegs[0].MCTL.bit.RST = 1;
	MotorRegs[0].MCTL.bit.ENA = 0;	// disable, and prepare the setting.

	dda_set.inpos = 640;
	dda_set.invel = 0;
	dda_set.inacc = 1000;
	dda_set.injerk = 0;
	M_SetDDA(dda_set);

	dda_set.inpos = 128000;
	dda_set.invel =  64000;
	dda_set.inacc = 0;
	M_SetDDA(dda_set);

	dda_set.inpos = 191000;
	dda_set.invel =  64000;
	dda_set.inacc = -500;
	M_SetDDA(dda_set);

	MotorRegs[0].MCTL.bit.ENA = 1;	// 使能电机
	MotorRegs[0].MCTL.bit.START = 1;	// disable, and prepare the setting.


	ESTOP0;
//	tmp =30;
//	while( tmp-- ){
//		rValue = MotorRegs[0].MSTA.all;
//
//		rValue = MotorRegs[0].NOWPH;
//		rValue = MotorRegs[0].NOWPL;
//		rValue = MotorRegs[0].NOWVH;
//		rValue = MotorRegs[0].NOWVL;
//		rValue = MotorRegs[0].NOWAH;
//		rValue = MotorRegs[0].NOWAL;
//		rValue = MotorRegs[0].NOWJH;
//		rValue = MotorRegs[0].NOWJL;
//
//	}

}
