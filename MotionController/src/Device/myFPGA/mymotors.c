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
#include "mymotors.h"
#define ONE_CIRCLE 6400


void InitMotors(void)
{
	int axis;
	for(axis=0; axis<AXISNUM; axis++){
		MotorRegs[axis].MCTL.all = 0;
	}
}
// 文件内部测试使用
void MR_SetDDA( int axis, DDA_VARS_S *dda){

	MotorRegs[axis].INPOS =  dda->pos;
	MotorRegs[axis].INVEL =  dda->vel;
	MotorRegs[axis].INACC =  dda->acc;
	MotorRegs[axis].INJERK = dda->jerk;
	MotorRegs[axis].MSTA.bit.NMSG = 1;	// push into the fifo of DDA.
}

void M_SetDDA( int axis, int32_t inpos, int32_t invel, int32_t inacc, int32_t injerk){

	MotorRegs[axis].INPOS =  inpos;
	MotorRegs[axis].INVEL =  invel;
	MotorRegs[axis].INACC =  inacc;
	MotorRegs[axis].INJERK = injerk;

	MotorRegs[axis].MSTA.bit.NMSG = 1;	// push into the fifo of DDA.
}

void testAD(void)
{
//	MotorRegs[0].MCTL.bit.AD_CONV = 1;
}

int M_usedSpace(uint16_t wp, uint16_t rp)
{
	int block_number = 1024;
	int tmp;
	tmp = wp >> 3;
	if (tmp < rp) tmp += block_number;		//fifo num?
	return (block_number - (tmp - rp));
}

