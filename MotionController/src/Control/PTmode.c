
#include "sysTypes.h"
#include "PTmode.h"

int dda_setvars[ins_num++];
int uniform_cmdbuf[ins_num++];
int down_cmdbuf[ins_num++];

struct DDA_SET_VARS{
	int32_t inpos;
	int32_t invel;
	int32_t inacc;
	int32_t injerk;
};

struct DDA_SET_VARS dda_setvars[10];
int ins_num = 0;	// Ö¸Áî¸öÊý


int PT_Mode(int axis, double pos)
{

		float rise_vel;
		float rise_pos;
		float uniform_pos;
		float down_vel;
		float down_pos;
//  fdsjajfil;asjfil;asj

		if(pos >= 0)
		{
			if(pos <= critical_pos)
			{
				t = sqrt(pos / acc_to_cmd);
				rise_vel = acc_to_cmd * t;
				rise_pos = 0.5 * pos;

				dda_setvars[ins_num].inpos = rise_pos;
				dda_setvars[ins_num].invel = rise_vel;
				dda_setvars[ins_num].inacc = acc_to_cmd;
				dda_setvars[ins_num++].injerk = 0;

				uniform_pos = 0;
				uniform_cmdbuf[ins_num++] = {0};
				down_cmdbuf[ins_num++] = {0,-acc_to_cmd,0,0,pos};

			}
			else
			{
				t = vel_to_cmd/acc_to_cmd;
				rise_pos = 0.5*vel_to_cmd*t;
				dda_setvars[ins_num++] = {0,acc_to_cmd,vel_to_cmd,rise_pos};
				uniform_pos = pos - rise_pos;
				uniform_cmdbuf[ins_num++] = {0,0,vel_to_cmd,uniform_pos};
				down_cmdbuf[ins_num++] = {0,-acc_to_cmd,0,pos};

			}
		}
		if(pos < 0)
		{
			if(pos >= -critical_pos)
			{
				t = sqrt(-pos / acc_to_cmd);
				rise_vel = -acc_to_cmd * t;
				rise_pos = -0.5 * pos;
				dda_setvars[ins_num++] = {0,-acc_to_cmd,rise_vel,rise_pos};
				uniform_pos = 0;
				uniform_cmdbuf[ins_num++] = {0};
				down_cmdbuf[ins_num++] = {0,acc_to_cmd,0,0,pos};
			}
			else
			{
				t = vel_to_cmd/acc_to_cmd;
				rise_pos = -0.5*vel_to_cmd*t;
				dda_setvars[ins_num++] = {0,-acc_to_cmd,-vel_to_cmd,rise_pos};
				uniform_pos = pos + rise_pos;
				uniform_cmdbuf[ins_num++] = {0,0,-vel_to_cmd,uniform_pos};
				down_cmdbuf[ins_num++] = {0,-acc_to_cmd,0,pos};
			}
		}
		return ok;
}

