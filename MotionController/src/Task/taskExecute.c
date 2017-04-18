/*
 * taskExecute.c
 *
 *  Created on: 2017-2-10
 *      Author: Administrator
 */

/*
 * taskExecute.c
 *
 *  Created on: 2017-2-10
 *      Author: Administrator
 */

#include "my_project.h"
#include "PTmode.h"
#include "senddata.h"

extern COMMAND_S gCmd;		// 来自ARM端的指令
extern SYS_INFO gSysInfo;

ERROR_CODE transState(STATE_TYPE sta)
{
	ERROR_CODE rtn = RTN_TRANS_STATE_INVALID;

	switch (gSysInfo.state){
	case STATE_IDLE:
		switch(sta) {
		case STATE_IDLE:
		case STATE_RUNNING:
		case STATE_ESTOP:
			gSysInfo.state = sta;
			rtn = RTN_SUCC;
			break;
		}
		break;

	case STATE_READY:
		switch(sta) {
		case STATE_IDLE:
		case STATE_READY:
			gSysInfo.state = sta;
			rtn = RTN_SUCC;
			break;
		}
		break;

	case STATE_RUNNING:
		switch(sta) {
		case STATE_IDLE:
		case STATE_RUNNING:
		case STATE_ESTOP:
			gSysInfo.state = sta;
			rtn = RTN_SUCC;
			break;
		}
		break;

	case STATE_ESTOP:
		switch(sta) {
		case STATE_READY:
		case STATE_ESTOP:
			gSysInfo.state = sta;
			rtn = RTN_SUCC;
			break;
		}
		break;

	default:
		break;
	}
	return rtn;
}


ERROR_CODE Message() {

	return RTN_SUCC;
}

ERROR_CODE Reset() {
	int axis;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
			MotorRegs[axis].MCTL.all = 0;	// disable, and prepare the setting.
			MotorRegs[axis].MCTL.bit.RST = 1;
		}
	}
	return transState(STATE_READY);
}

// Emergence Stop: Stop all the activated motors.
ERROR_CODE Estop() {
	int axis;
	for (axis = 0; axis < AXISNUM; axis++) {
		if (MotorRegs[axis].MCTL.bit.ENA) {
			MotorRegs[axis].MCTL.all = 1;
		}
	}
	return RTN_SUCC;
}

ERROR_CODE Stop() {
	int axis;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ( (gCmd.mark >> axis) & 0x01 ) {
			MotorRegs[axis].MCTL.all = 1;
		}
	}
	return transState(STATE_ESTOP);
}

ERROR_CODE Activate() {
	int axis;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
			MotorRegs[axis].MCTL.bit.ENA = 1;			// 使能电机
		}
	}
	return RTN_SUCC;
}

ERROR_CODE Start() {
	int axis;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
			MotorRegs[axis].MCTL.bit.START = 1; // 启动电机
			}
		}
	return transState(STATE_RUNNING);
}

ERROR_CODE GoHome(){
	int axis;
	PVAT_S home ={ 	0,						// aim_pos
					-DEFUALT_MAX_EVEN_VEL,	// start_vel
					0,						// start_acc
					0};						// min_period
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01)
			M_SetPvat( axis, &home);
	}
	return transState(STATE_RUNNING);
}

ERROR_CODE SetDDA() {
	int axis;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01)
			M_SetPvat( axis, &gCmd.pvat[axis]);
	}
	return RTN_SUCC;
}

ERROR_CODE EnterPTmode() {
	int axis;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
			PT_Mode(axis, &gCmd.ptdata[axis]);
		}
	}

	return RTN_SUCC;
}

ERROR_CODE ReadDDA(void) {
	int axis;
	short i = 0;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
			memcpy(gCmd.dat_buf + i, (Uint16 *) &MotorRegs[axis].NOWPOS, 8);
			i += 8;
		}
	}
	senddata(gCmd.type, gCmd.mark, gCmd.dat_buf, i);
	return RTN_SUCC;
}

ERROR_CODE ReadMotor() {
	int axis;
	int i = 0;

	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
			gCmd.dat_buf[i++] = MotorRegs[axis].MSTA.all;
			gCmd.dat_buf[i++] = MotorRegs[axis].NOWPOS;
			gCmd.dat_buf[i++] = MotorRegs[axis].NOWVEL;
			gCmd.dat_buf[i++] = MotorRegs[axis].NOWACC;
			gCmd.dat_buf[i++] = MotorRegs[axis].INXX;

		}
	}
	senddata(gCmd.type, gCmd.mark, gCmd.dat_buf, i);

	return RTN_SUCC;
}

ERROR_CODE ReadMfifo() {   //dsp-fpga
	int axis;
	int i = 0;

	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
			gCmd.dat_buf[i++] = MotorRegs[axis].FFRP;
			gCmd.dat_buf[i++] = MotorRegs[axis].FFWP;
		}

	}
	senddata(gCmd.type, gCmd.mark, gCmd.dat_buf, i);
	return RTN_SUCC;
}

ERROR_CODE ReadSram()
{
	unsigned int dat[AXISNUM];
	unsigned int axis, i = 0;

	for(axis = 0; axis < AXISNUM; axis++) {
		if((gCmd.mark >> axis) & 0x01) {
			dat[i++] = cb_usedSpace(&cmd_buf[axis]);
		}
	}
	senddata(gCmd.type,gCmd.mark,dat,i);
	return RTN_SUCC;
}

