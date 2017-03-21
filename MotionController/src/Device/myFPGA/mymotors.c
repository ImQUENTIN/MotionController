/*
*  Created on: 2016-12-18
*      Author: QUENTIN
*      E-mail: qiyuexin@yeah.net
*/
//###########################################################################
//
//  FILE:   mymotors.c
//
// TITLE:   FPGA�������
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
// �ļ��ڲ�����ʹ��
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

void testMymotor(void)
{
	int time_us = 50;
	MotorRegs[0].MCTL.all = 0;	// ��λ���
	MotorRegs[0].MCTL.all = 1;
	M_SetDDA(0, ONE_CIRCLE, 64e3,      0, 0);
	MotorRegs[0].MCTL.all = 3;	// ʹ�ܵ��

	ESTOP0;
}
