#ifndef _PTMODE_H_
#define _PTMODE_H_

#include "../CircleBuffer/CircleBuffer.h"

int PT_Mode(int axis, int pos,int time) ;

#define max_acc 0.5
#define max_vel 100
#define critical_pos 20000 //三角-梯形 临界值 =0.5a*t^2


#define MAX_BUFF_LEN 32


CIRCLE_BUFFER_S pt_buf;

typedef struct
{
	float jerk;
	float acc;
	float vel;
	int pos;
	int time;
}PT_DATA_S;



#endif
