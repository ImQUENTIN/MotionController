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

#include "sysTypes.h"
#include "PTmode.h"

ERROR_CODE upload_encoders(unsigned short mark)
{
	//
	//getEncoderFromFPGA();
	//upload();
	return RTN_SUCC;
}

ERROR_CODE reset_estop()
{
	//
	return RTN_SUCC;
}


extern COMMAND_S   gCmd;		// 来自ARM端的指令
ERROR_CODE EnterPTmode(unsigned int mark)
{
	int axis;
	for (axis = 0; axis < AXISNUM; axis++) {
		if ((gCmd.mark >> axis) & 0x01) {
			PT_Mode(axis, gCmd.ptPos[axis],gCmd.ptTime[axis]) ;
		}
	}
	return RTN_SUCC;
}




