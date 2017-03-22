/*
 * myram.c
 *
 *  Created on: 2017-3-17
 *      Author: QIYUEXIN
 *  Contact me: qiyuexin@yeah.net
 */

#include "sysTypes.h"
#include "CircleBuffer.h"
#include "string.h"
#include <math.h>
#include "mymotors.h"

#define DDA_RAM_SIZE 		0x80000/AXISNUM/8

// 运动卡的EXTRAM用的zone6，
#pragma DATA_SECTION(dda_stored,"EXTRAM_DATA");

//8 words;
DDA_VARS_S dda_stored[AXISNUM][DDA_RAM_SIZE];	// 8k
CIRCLE_BUFFER_S ram_dda[AXISNUM];

void EXTRAM_init(void) {
	int i;
	for (i = 0; i < AXISNUM; i++) {
		memset(&ram_dda[i], 0, sizeof(CIRCLE_BUFFER_S));
		ram_dda[i].dat = (int*) dda_stored[i];
		ram_dda[i].block_number = DDA_RAM_SIZE;
		ram_dda[i].block_size = sizeof(DDA_VARS_S);
		ram_dda[i].head = 0;
		ram_dda[i].tail = 0;
	}

}
double constrain(double src, double min, double max) {
	return (src > max) ? max : (src < min) ? min : src;
}
void testPlot(void) {
	double STEP_X = (2.1 * 6400) / 100;   //步进电机步长，单位mm
	double A = 30, B = 2 * 3.141593 / 80, D;	// sin 参数
	double step_vel = 1e4, max_vel = 64e3;		// 稳定脉冲速度

	double x, y, v, px, py, vx, vy, x_prev = 0, y_prev = 0, t_sec = 0;

	int x_start = 1, x_end = 100; // 单位mm

	DDA_VARS_S dda[AXISNUM];
	static int32_t pre_pos[AXISNUM];
	const int time_scale = 50;
	int axis = 0;

	D = A;

	for (x = x_start; x < x_end + 1; x+=1) {
		y = A * sin(B * x) + D;
		// 计算脉冲速度
		vx = (x - x_prev) * STEP_X;		// delta x
		vy = (y - y_prev) * STEP_X;		// delta y
		v = sqrt(vx * vx + vy * vy);		// distance
		t_sec = (v / step_vel);			// time, seconds

		vx = vx / t_sec;					// vel x
		vy = vy / t_sec;					// vel y

		// 压入RAM
		memset(&dda[0], 0, sizeof(DDA_VARS_S) * AXISNUM);
		pre_pos[0] = dda[0].pos;
		pre_pos[1] = dda[1].pos;

		px = x * STEP_X;
		py = y * STEP_X;
		// 四舍五入
		if(px>0) px = px + 0.5; else px = px - 0.5;
		if(py>0) py = py + 0.5; else py = py - 0.5;

		// 压入RAM
		dda[0].pos = (int32_t) px;
		dda[0].vel = (int32_t) vx; //
		dda[0].jerk = t_sec*1e6*time_scale;			// used as period:us

		dda[1].pos = (int32_t) py;
		dda[1].vel = (int32_t) vy;
		dda[1].jerk = t_sec*1e6*time_scale;			// used as period:us

		cb_append(&ram_dda[0], &dda[0]);
		cb_append(&ram_dda[1], &dda[1]);

		x_prev = x;
		y_prev = y;
	}

	for (axis = 0; axis < 2; axis++) {
		MotorRegs[axis].MCTL.all = 0;	// 复位电机
		MotorRegs[axis].MCTL.all = 1;
		while (RTN_ERROR != cb_get(&ram_dda[axis], &dda[axis])) {
			// 取轴axis, 压入DDA
			MR_SetDDA(axis, &dda[axis]);
		}
	}

	// 同时启动
	for (axis = 0; axis < 2; axis++) {
		MotorRegs[axis].MCTL.bit.ENA = 1;
	}

//	while(MotorRegs[0].FFRP != MotorRegs[0].FFWP>>3);
//	while( MotorRegs[0].MSTA.bit.MBSY);
//
//	// 关闭电机
//	for (axis = 0; axis < 2; axis++) {
//		MotorRegs[axis].MCTL.bit.ENA = 0;
//	}

}

void EXTRAM_test(void) {
	uint16_t i, j;
	static DDA_VARS_S tmp, tmp2;
	for (i = 0; i < DDA_RAM_SIZE - 1; i++) {
		// 写满
		tmp.pos = i;
		tmp.vel = i + 1;
		tmp.acc = i + 2;
		tmp.jerk = i + 3;
		for (j = 0; j < AXISNUM; j++)
			if (RTN_ERROR == cb_append(&ram_dda[j], &tmp))
				asm("ESTOP0");
//		if( i == 8191) asm("ESTOP0");
	}

	for (; i < DDA_RAM_SIZE + 16; i++) {
		// 检测覆盖写入
		tmp.pos = i;
		tmp.vel = i + 1;
		tmp.acc = i + 2;
		tmp.jerk = i + 3;
		for (j = 0; j < AXISNUM; j++) {
			if (RTN_ERROR == cb_append(&ram_dda[j], &tmp)) {
				cb_get(&ram_dda[j], &tmp2);
				cb_append(&ram_dda[j], &tmp);
			}
		}

	}
	//*****************************
	// 可以直接查看 dda_stored 数据。
	//*****************************

}

