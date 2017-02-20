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

#include "DSP2833x_Device.h"
#include "sysTypes.h"
#include "PTmode.h"

extern COMMAND_S   gCmd;		// ����ARM�˵�ָ��


ERROR_CODE Message(){
	return RTN_SUCC;
}


ERROR_CODE Reset(){
	int axis;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
			MotorRegs[0].MCTL.all 		= 0;	// disable, and prepare the setting.
			MotorRegs[0].MCTL.bit.RST 	= 1;
		}
	}
	return RTN_SUCC;
}



ERROR_CODE Estop()
{
	int axis;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
			MotorRegs[0].MCTL.bit.PAUSE 	= 1;
			ESTOP0;	// TEST HERE.
		}
	}
	return RTN_SUCC;
}

ERROR_CODE Activate(){
	int axis;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
			MotorRegs[0].MCTL.bit.ENA = 1;			// ʹ�ܵ��
			ESTOP0;	// TEST HERE.
		}
	}
	return RTN_SUCC;
}

ERROR_CODE Start(){
	int axis;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
//			tmp = MotorRegs[0].MCTL.all;
			MotorRegs[0].MCTL.bit.PAUSE = 0;			// �������
			ESTOP0;	// TEST HERE.
		}
	}
	return RTN_SUCC;
}

ERROR_CODE SetDDA(){
	int axis;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
			MotorRegs[axis].INPOS =  gCmd.setDDA[axis].pos;
			MotorRegs[axis].INVEL =  gCmd.setDDA[axis].vel;
			MotorRegs[axis].INACC =  gCmd.setDDA[axis].acc;
			MotorRegs[axis].INJERK = gCmd.setDDA[axis].jerk;

			MotorRegs[axis].MSTA.bit.NMSG = 1;	// push into the fifo of DDA.
		}
	}
	return RTN_SUCC;
}

ERROR_CODE EnterPTmode()
{
	int axis;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
			PT_Mode(axis, gCmd.ptPos[axis],gCmd.ptTime[axis]) ;
		}
	}
	return RTN_SUCC;
}

