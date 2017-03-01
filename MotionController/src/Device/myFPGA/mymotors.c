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
#include "sysTypes.h"


// 文件内部测试使用
void M_SetDDA( int32_t inpos, int32_t invel, int32_t inacc, int32_t injerk){

	MotorRegs[0].INPOS =  inpos;
	MotorRegs[0].INVEL =  invel;
	MotorRegs[0].INACC =  inacc;
	MotorRegs[0].INJERK = injerk;

	MotorRegs[0].MSTA.bit.NMSG = 1;	// push into the fifo of DDA.
}
#define ONE_CIRCLE 6400

void testAD(void)
{
//	MotorRegs[0].MCTL.bit.AD_CONV = 1;
}
void testMymotor(void)
{

	MotorRegs[0].MCTL.all = 0;// disable, and prepare the setting.

	MotorRegs[0].MCTL.bit.RST = 1;


//	M_SetDDA(45 ,     0,   1, 0);
//	M_SetDDA(90 ,  2400,   -1, 0);

	M_SetDDA(1000 ,     0,   32, 0);
	M_SetDDA(1000+64000 , 64000,    0, 0);
	M_SetDDA(2000+64000 ,64000,   -32, 0);


//	M_SetDDA(4000, 000,  8, 0);
//	M_SetDDA(4000 +4800, 64000,  0, 0);
//	M_SetDDA(64000*2, 64000,  -8, 0);

	MotorRegs[0].MCTL.all = 3;	// 使能电机

	ESTOP0;

	MotorRegs[0].MCTL.bit.RST = 0;
	MotorRegs[0].MCTL.bit.RST = 1;
}
