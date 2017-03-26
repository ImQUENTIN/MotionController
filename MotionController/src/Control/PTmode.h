#ifndef _PTMODE_H_
#define _PTMODE_H_

#include"CircleBuffer.h"

typedef struct
{
	float jerk;
	float acc;
	float vel;
	int pos;
	int time;
}PT_DATA_S;


extern CIRCLE_BUFFER_S pt_buf[AXISNUM];
extern VP_PARAM_S vp_param[AXISNUM];		// velocity plan parameters.

ERROR_CODE PT_Mode(int axis, PT_VARS_S *pt);
ERROR_CODE PT_Data();

#endif
