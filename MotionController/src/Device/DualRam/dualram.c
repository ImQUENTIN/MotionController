#include "dualram.h"

char flag;
extern COMMAND_S gCmd;
ERROR_CODE FPGA_Space()
{
	int axis;
	flag = 0;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
			 if( M_usedSpace(MotorRegs[axis].FFWP, MotorRegs[axis].FFRP) == 0)
				flag |= (1 << axis);
		}
	}
	return RTN_SUCC;
}
ERROR_CODE LIMIT()
{
	int axis;
	int i;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
			if( (MotorRegs[axis].MCONF.bit.LIMITNV == 1) || (MotorRegs[axis].MCONF.bit.LIMITPV == 1)){
				for(i = 0; i < AXISNUM; i++){
					if ((gCmd.mark >> i) & 0x01)
					{

						MotorRegs[i].MCTL.bit.START = 0;
						MotorRegs[i].MCTL.bit.ENA = 0;
						MotorRegs[i].MCTL.bit.EDITA = 1;
						MotorRegs[i].MCONF.bit.LIMITNV = 1;
						MotorRegs[i].MCONF.bit.LIMITPV = 1;
						MotorRegs[i].MCTL.bit.EDITA = 0;
						MotorRegs[i].FFWP = 8*MotorRegs[i].FFRP;
						cb_clear(&ram_dda[i]);

					}
				}
				break;
			}
		}
	}
	return RTN_SUCC;
}
