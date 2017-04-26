
#include "sysTypes.h"
#include <math.h>
#include "mymotors.h"
#include "string.h"
#include "CircleBuffer.h"
#include "myram.h"

/* 电机/机械结构参数 */
const double STEP_X = (2.1 * 6400) / 100;   // X轴步进电机步长，单位pulse/mm
const double STEP_Y = (2.1 * 6400) / 100;   // Y轴步进电机步长，单位pulse/mm
const double STEP_Z = (2.1 * 6400) / 100;   // Z轴步进电机步长，单位pulse/mm
const double STEP_V = 6400;		// 期望合速度，单位pulse/second

/* 绘图尺寸范围限制 */
const int x_start=0;      // 坐标下限（相对于系统坐标原点，单位mm）
const int y_start=0;
const int z_start=0;
const int x_end=100;       // 坐标上限（相对于系统坐标原点，单位mm）
const int y_end=100;
const int z_end=100;
// offset
 int x_offset = 0;
 int y_offset = 0;
 int z_offset = 0;

const int step;
const int time_scale = 50;
const double pi = 3.141592654;

/* 数据密化参数 */
struct CAL_PVAT{
	double x_prev, y_prev, z_prev;	//
	double px, py, pz;				// Position
	double vx, vy, vz;				// Velocity
	double s;						// distance
	double t_sec;					// period
} cal_pvat;

PVAT_S pvat[AXISNUM];			// pvat data

inline double constrain(double src, double min, double max){
	return (src>max)?max:(src<min)?min:src;
}
/* 计算pvat数据 */
void cal_pt_data3(struct CAL_PVAT *cal){
	// 计算速度
	cal->vx = cal->px - cal->x_prev;
	cal->vy = cal->py - cal->y_prev;
	cal->vz = cal->pz - cal->z_prev;

	cal->s = sqrt( cal->vx*cal->vx +cal->vy*cal->vy +cal->vz*cal->vz );
	cal->t_sec = cal->s/STEP_V;

	cal->vx /= cal->t_sec;
	cal->vy /= cal->t_sec;
	cal->vz /= cal->t_sec;

	cal->x_prev = cal->px;
	cal->y_prev = cal->py;
	cal->z_prev = cal->pz;
}

void PlotSin(struct CAL_PVAT *cal);
void PlotCirlce(struct CAL_PVAT *cal) ;
int32_t DDA_MOVE(int axis, int32_t aim_pos, int32_t delay_time);

void testPlot(void){
	int axis;
	for( axis = 0; axis <3; axis++) {
		M_SetPvatMode(axis);	// Enter DDA MODE
		M_ServoOn(axis);		// Turn On servo
	}

	DDA_MOVE(0, 	0, 0.5*1e6*time_scale);
	DDA_MOVE(1, 	0, 0.5*1e6*time_scale);
	DDA_MOVE(2, 	0, 0.5*1e6*time_scale);
	PlotCirlce(&cal_pvat);
	DDA_MOVE(0, 	0, 0.5*1e6*time_scale);
	DDA_MOVE(1, 	0, 0.5*1e6*time_scale);
	DDA_MOVE(2, 	0, 0.5*1e6*time_scale);
//	PlotSin(&cal_pvat);

	for (axis = 0; axis < 3; axis++) {
		while (RTN_ERROR != cb_get(&cmd_buf[axis], &pvat[axis])) {
			// 取轴axis, 压入DDA
			if(M_GetfreeCmdSize(axis)>2)	M_SetPvat(axis, &pvat[axis]);
		}
	}

	/* start all the axis at the same time */
	for( axis = 0; axis <3; axis++) {
		M_Update(axis);
	}


}

//	INxxx 显示下一步DDA输入参数
//	MotorRegs[axis].MCTL.bit.EDITA = 1;
//	MotorRegs[axis].MCONF.bit.INDISPM = 0;
//	MotorRegs[axis].MCTL.bit.EDITA = 0;

//for (axis = 0; axis < 2; axis++) {
//	MotorRegs[axis].MCTL.bit.START = 1;
//}

// 调试模式
//while(MotorRegs[0].FFRP != MotorRegs[0].FFWP>>3){

//			while( MotorRegs[0].MSTA.bit.MBSY);// && abs(MotorRegs[0].INPOS - MotorRegs[0].NOWPOS) < 500);
		// 同时启动
//			for (axis = 0; axis < 2; axis++) {
//				MotorRegs[axis].MCTL.bit.START = 1;
//			}
//
//			// 同时停止
//			for (axis = 0; axis < 2; axis++) {
//				MotorRegs[axis].MCTL.bit.START = 0;
//			}
//
//			ESTOP0;	// stop here.
//	}
// test clear limit bit
//{
//	MotorRegs[0].MSTA.bit.LMTN = 1;
//	MotorRegs[0].MSTA.bit.LMTP = 1;
//}


void PlotSin(struct CAL_PVAT *cal) {
	double A = 30, B = 2 * 3.141593 / 80, D;	// sin 参数
	double x,y;

	D = A;

	cal->x_prev = 0;
	cal->y_prev = 0;
	cal->z_prev = 0;

	for (x = x_start; x < x_end + 1; x+=1) {
		y = A * sin(B * x) + D;
		// constrain:
		x = constrain(x, x_start, x_end);
		y = constrain(y, y_start, y_end);

		cal->px = (x+x_offset) * STEP_X;
		cal->py = (y+y_offset) * STEP_Y;
		cal->pz = (0+z_offset) * STEP_Z;

		cal_pt_data3(cal);		// calculated pvat data.

		// 压入RAM
		memset(&pvat[0], 0, sizeof(PVAT_S) * AXISNUM);		// clear all.
		pvat[0].aim_pos 	= (int32_t) cal->px;
		pvat[0].start_vel 	= (int32_t) cal->vx; 			//
		pvat[0].min_period 	= cal->t_sec*1e6*time_scale;			// used as period:us

		pvat[1].aim_pos 	= (int32_t) cal->py;
		pvat[1].start_vel 	= (int32_t) cal->vy; 			//
		pvat[1].min_period 	= cal->t_sec*1e6*time_scale;			// used as period:us

		cb_append(&cmd_buf[0], &pvat[0]);
		cb_append(&cmd_buf[1], &pvat[1]);
	}
}
void PlotCirlce(struct CAL_PVAT *cal) {
	const int R = 40;
	const double pitch = pi/4;
	double theta,x,y,z;

	x_offset 	= 0;
	y_offset 	= 0;
	z_offset 	= 0;

	for (theta = 0; theta <= 2*pi; theta +=2*pi/100) {

		x = R*cos(pitch)*cos(pi - theta) + x_offset;
		y = R*cos(pitch)*sin(pi - theta) + y_offset;
		z = x*tan(pitch) + z_offset;
		cal->px = (x+x_offset) * STEP_X;
		cal->py = (y+y_offset) * STEP_Y;
		cal->pz = (z+z_offset) * STEP_Z;

		if(theta == 0){
			cal->x_prev = DDA_MOVE(0, cal->px, 0.5*1e6*time_scale);
			cal->y_prev = DDA_MOVE(1, cal->py, 0.5*1e6*time_scale);
			cal->z_prev = DDA_MOVE(2, cal->pz, 0.5*1e6*time_scale);
			continue;
		}

		cal_pt_data3(cal);		// calculated pvat data.
		// 压入RAM
		memset(&pvat[0], 0, sizeof(PVAT_S) * AXISNUM);		// clear all.
		pvat[0].aim_pos 	= (int32_t) cal->px;
		pvat[0].start_vel 	= (int32_t) cal->vx; 			//
		pvat[0].min_period 	= cal->t_sec*1e6*time_scale;			// used as period:us

		pvat[1].aim_pos 	= (int32_t) cal->py;
		pvat[1].start_vel 	= (int32_t) cal->vy; 			//
		pvat[1].min_period 	= cal->t_sec*1e6*time_scale;			// used as period:us

		pvat[2].aim_pos 	= (int32_t) cal->pz;
		pvat[2].start_vel 	= (int32_t) cal->vz; 			//
		pvat[2].min_period 	= cal->t_sec*1e6*time_scale;			// used as period:us

		cb_append(&cmd_buf[0], &pvat[0]);
		cb_append(&cmd_buf[1], &pvat[1]);
		cb_append(&cmd_buf[2], &pvat[2]);
	}
}

int32_t DDA_MOVE(int axis, int32_t aim_pos, int32_t delay_time){
	PVAT_S pvat, tmp;
	cb_getLast(&cmd_buf[axis], &tmp);

	pvat.min_period = delay_time;
	pvat.aim_pos 	= aim_pos;

	if( aim_pos >= tmp.aim_pos){
		pvat.start_vel 	= 4000;
	}
	else{
		pvat.start_vel 	= -4000;
	}
	cb_append(&cmd_buf[axis], &pvat);
	return aim_pos;
}
