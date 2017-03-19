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
	MotorRegs[0].MCTL.all = 0;	// 复位电机
	MotorRegs[1].MCTL.all = 0;	// 复位电机

	MotorRegs[0].MCTL.all = 1;	// 复位电机
	MotorRegs[1].MCTL.all = 1;	// 复位电机
//	M_SetDDA(1000 ,     0,   32, 0);
//	M_SetDDA(1000+64000 , 64000,    0, 0);
//	M_SetDDA(2000+64000 ,64000,   -32, 0);]


//	  时段控制的DDA数据格式为：[deltaTime, startVel, startAcc, Jerk];
	// 500ms加速到最大速度
	// delta_pos = 0.5*a*t^2 = 16e3
//	M_SetDDA(500e3*time_us,    0, 2*64e3, 0);
	// 最大速度运行3秒
	// delta_pos = v*t = 192e3
	M_SetDDA(0, ONE_CIRCLE, 64e3,      0, 0);
//	M_SetDDA(0, 0, -64e2,      0, 0);
	M_SetDDA(1, ONE_CIRCLE, 64e2,      0, 0);
//	M_SetDDA(1, 0, -64e2,      0, 0);

	// 1s减速到0
	// delta_pos = 0.5*a*t^2 = 32e3
//	M_SetDDA(1000e3*time_us,64e3,  -64e3, 0);

	// nowpos should be 240e3
//-------------------------------------------
//	M_SetDDA(4000, 000,  8, 0);
//	M_SetDDA(4000 +4800, 64000,  0, 0);
//	M_SetDDA(64000*2, 64000,  -8, 0);

	MotorRegs[0].MCTL.all = 3;	// 使能电机
	MotorRegs[1].MCTL.all = 3;	// 使能电机

	ESTOP0;
}
