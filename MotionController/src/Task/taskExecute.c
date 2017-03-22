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
#include "senddata.h"

extern COMMAND_S gCmd;		// ����ARM�˵�ָ��


ERROR_CODE Message() {

	return RTN_SUCC;
}

ERROR_CODE Reset() {
	int axis;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
			MotorRegs[0].MCTL.all = 0;	// disable, and prepare the setting.
			MotorRegs[0].MCTL.bit.RST = 1;
		}
	}
	return RTN_SUCC;
}

ERROR_CODE Estop() {
	int axis;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
			MotorRegs[0].MCTL.bit.PAUSE = 1;
			ESTOP0;
			// TEST HERE.
		}
	}
	return RTN_SUCC;
}

ERROR_CODE Activate() {
	int axis;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
			MotorRegs[0].MCTL.bit.ENA = 1;			// ʹ�ܵ��
			ESTOP0;
			// TEST HERE.
		}
	}
	return RTN_SUCC;
}

ERROR_CODE Start() {
	int axis;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
//			tmp = MotorRegs[0].MCTL.all;
			MotorRegs[0].MCTL.bit.ENA = 1;			// ʹ�ܵ��
			MotorRegs[0].MCTL.bit.PAUSE = 0;			// �������
//			ESTOP0;	// TEST HERE.
		}
	}
	return RTN_SUCC;
}

ERROR_CODE SetDDA() {
	int axis;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
			MotorRegs[axis].INPOS = gCmd.setDDA[axis].pos;
			MotorRegs[axis].INVEL = gCmd.setDDA[axis].vel;
			MotorRegs[axis].INACC = gCmd.setDDA[axis].acc;
			MotorRegs[axis].INJERK = gCmd.setDDA[axis].jerk;

			MotorRegs[axis].MSTA.bit.NMSG = 1;	// push into the fifo of DDA.
		}
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
			i++;
			gCmd.dat_buf[i] = MotorRegs[i].MSTA.all;
		}
	}
	senddata(gCmd.type, gCmd.mark, gCmd.dat_buf, i);

	return RTN_SUCC;
}

ERROR_CODE ReadMfifo() {
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
	int dat[AXISNUM];
	unsigned int axis, i = 0;

	for(axis = 0; axis < AXISNUM; axis++){
			if((gCmd.mark >> axis) & 0x01){
				dat[i++] = cb_usedSpace(&ram_dda[axis]);
				}
	}
	senddata(gCmd.type,gCmd.mark,dat,i);
	return RTN_SUCC;
}

