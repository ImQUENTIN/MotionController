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
#include <string.h>

#define ONE_CIRCLE 6400


void InitMotors(void)
{
	int axis;
	for(axis=0; axis<AXISNUM; axis++){
		MotorRegs[axis].MCTL.all = 0;
		MotorRegs[axis].MCTL.all = 9;	// no reset & EDITA
		MotorRegs[axis].MCONF.bit.DISPM = 1; // INxx 显示写入的数据
		if(axis>2){
			// 这些轴没有使用
			MotorRegs[axis].MCONF.bit.LIMITNV = 1;
			MotorRegs[axis].MCONF.bit.LIMITPV = 1;
		}
		MotorRegs[axis].MCTL.bit.EDITA = 0;
	}
}


// 打开电机
void M_ServoOn(int axis){
	MotorRegs[axis].MCTL.bit.ENA = 1;
}

// 关闭电机
void M_ServoOff(int axis){
	MotorRegs[axis].MCTL.bit.ENA = 0;
}

// 运行电机运行
void M_Update(int axis){
	MotorRegs[axis].MCTL.bit.START = 1;
}


// 获取当前轴的控制模式
enum MMODE_E M_GetCurMode(int axis){
	return MotorRegs[axis].MCTL.bit.MMODE;
}

// 设置当前轴为PT模式
void M_SetPvatMode(int axis){
	MotorRegs[axis].MCTL.bit.MMODE = DDA_MODE;
}

// 输入pvat的数据
void M_SetPvat( int axis, PVAT_S *dda){
	MotorRegs[axis].INPOS =  dda->aim_pos;
	MotorRegs[axis].INVEL =  dda->start_vel;
	MotorRegs[axis].INACC =  dda->start_acc;
	MotorRegs[axis].INXX  =  dda->min_period;
	MotorRegs[axis].MSTA.bit.NMSG = 1;	// push into the fifo of DDA.
}

// 获取空闲的DDA命令缓存区的大小
int M_GetfreeCmdSize(int axis)
{
	int wp = MotorRegs[axis].FFWP>>3;
	int rp = MotorRegs[axis].FFRP;
	int cmdbufsize = 1 << 1+MotorRegs[axis].MCONF.bit.CBUFS;

	if (wp < rp)	return (rp - wp);
	else			return ( cmdbufsize - (wp - rp));

}

// 设置当前轴为JOG模式
void M_SetJogMode(int axis){
	MotorRegs[axis].MCTL.bit.MMODE = JOG_MODE;
}

// 输入vad 的数据
void M_SetVad( int axis, int32_t aim_vel){
	MotorRegs[axis].MCTL.bit.START = 0;
	MotorRegs[axis].INVEL =  aim_vel;
	MotorRegs[axis].MCTL.bit.START = 1;
}


