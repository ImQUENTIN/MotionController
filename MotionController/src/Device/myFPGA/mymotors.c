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

void testMymotor(void)
{
	int axis=0;
	int intervel_space = 1000.2;

	axis=0;
	MotorRegs[axis].MCTL.all = 0;	// 复位电机
	MotorRegs[axis].MCTL.all = 1;
#if 1

	M_SetDDA(axis,7300.9,	3e4 ,0, 0);
	M_SetDDA(axis,7300.9,	0 ,0, 0);
	MotorRegs[axis].MCTL.all = 3;
	ESTOP0;
	MotorRegs[axis].MCTL.all = 1;
	M_SetDDA(axis,7573.9,	6961.9 ,0, 0);
	M_SetDDA(axis,7668.2,	4407 ,0, 0);
//	M_SetDDA(axis,7668.2,	0 ,0, 0);
	M_SetDDA(axis,7573.9,	-4407 ,0, 0);
	M_SetDDA(axis,7387.7,	-6961.9 ,0, 0);

//	M_SetDDA(axis, ,0, 0);
#else

	M_SetDDA(axis, intervel_space, 				1e3,      0, 0);
	M_SetDDA(axis, ONE_CIRCLE*3-intervel_space, 4e4,      0, 0);
	M_SetDDA(axis, ONE_CIRCLE*3,   				1e3,      0, 0);

	M_SetDDA(axis, ONE_CIRCLE*3-intervel_space, -1e3,      0, 0);
	M_SetDDA(axis, intervel_space, 				-4e4,      0, 0);
	M_SetDDA(axis, 0,   						-1e3,      0, 0);



	axis=1;
	MotorRegs[axis].MCTL.all = 0;	// 复位电机
	MotorRegs[axis].MCTL.all = 1;
	M_SetDDA(axis, intervel_space, 				1e3,      0, 0);
	M_SetDDA(axis, ONE_CIRCLE*3-intervel_space, 4e4,      0, 0);
	M_SetDDA(axis, ONE_CIRCLE*3,   				1e3,      0, 0);

	M_SetDDA(axis, ONE_CIRCLE*3-intervel_space, -1e3,      0, 0);
	M_SetDDA(axis, intervel_space, 				-4e4,      0, 0);
	M_SetDDA(axis, 0,   						-1e3,      0, 0);
#endif
	MotorRegs[0].MCTL.all = 3;	// 使能电机
	MotorRegs[1].MCTL.all = 3;	// 使能电机

	ESTOP0;

}
