
#include "sysTypes.h"
#include "PTmode.h"
#include "math.h"


struct DDA_SET_VARS{
	int32_t inpos;
	int32_t invel;
	int32_t inacc;
	int32_t injerk;
};

struct DDA_SET_VARS dda_setvars[3];
int ins_num = 0;	// Ö¸Áî¸öÊı



int PT_Mode(int axis, double pos)
{

		float rise_vel;
		float rise_pos;
		float uniform_pos;

		int t;

		if(pos == 0)
		{
			int i;
			for(i = 0; i < 3; i++)
			{
				dda_setvars[ins_num].inacc = 0;
				dda_setvars[ins_num].invel = 0;
				dda_setvars[ins_num].inpos = 0;
				dda_setvars[ins_num++].injerk = 0;
			}
		}

		if(pos > 0)
		{
			if(pos <= critical_pos)
			{
				t = sqrt(pos/acc_to_cmd);
				rise_vel = acc_to_cmd * t;
				rise_pos = 0.5 * pos;

				dda_setvars[ins_num].inpos = rise_pos;
				dda_setvars[ins_num].invel = rise_vel;
				dda_setvars[ins_num].inacc = acc_to_cmd;
				dda_setvars[ins_num++].injerk = 0;

				dda_setvars[ins_num].inacc = 0;
				dda_setvars[ins_num].invel = 0;
				dda_setvars[ins_num].inpos = 0;
				dda_setvars[ins_num++].injerk = 0;

				dda_setvars[ins_num].inpos = pos;
				dda_setvars[ins_num].invel = 0;
				dda_setvars[ins_num].inacc = -acc_to_cmd;
				dda_setvars[ins_num].injerk = 0;

			}
			else
			{
				t = vel_to_cmd/acc_to_cmd;
				rise_pos = 0.5*vel_to_cmd*t;
				dda_setvars[ins_num].inpos = rise_pos;
				dda_setvars[ins_num].invel = vel_to_cmd;
				dda_setvars[ins_num].inacc = acc_to_cmd;
				dda_setvars[ins_num++].injerk = 0;

				uniform_pos = pos - rise_pos;
				dda_setvars[ins_num].inacc = 0;
				dda_setvars[ins_num].invel = vel_to_cmd;
				dda_setvars[ins_num].inpos = uniform_pos;
				dda_setvars[ins_num++].injerk = 0;

				dda_setvars[ins_num].inpos = pos;
				dda_setvars[ins_num].invel = 0;
				dda_setvars[ins_num].inacc = -acc_to_cmd;
				dda_setvars[ins_num].injerk = 0;

			}
		}
		if(pos < 0)
		{
			if(pos >= -critical_pos)
			{
				t = sqrt(-pos/acc_to_cmd);
				rise_vel = -acc_to_cmd * t;
				rise_pos = 0.5 * pos;

				dda_setvars[ins_num].inpos = rise_pos;
				dda_setvars[ins_num].invel = rise_vel;
				dda_setvars[ins_num].inacc = -acc_to_cmd;
				dda_setvars[ins_num++].injerk = 0;

				dda_setvars[ins_num].inacc = 0;
				dda_setvars[ins_num].invel = 0;
				dda_setvars[ins_num].inpos = 0;
				dda_setvars[ins_num++].injerk = 0;

				dda_setvars[ins_num].inpos = pos;
				dda_setvars[ins_num].invel = 0;
				dda_setvars[ins_num].inacc = acc_to_cmd;
				dda_setvars[ins_num].injerk = 0;
			}
			else
			{
				t = vel_to_cmd/acc_to_cmd;
				rise_pos = -0.5*vel_to_cmd*t;

				dda_setvars[ins_num].inpos = rise_pos;
				dda_setvars[ins_num].invel = -vel_to_cmd;
				dda_setvars[ins_num].inacc = -acc_to_cmd;
				dda_setvars[ins_num++].injerk = 0;
				uniform_pos = pos + rise_pos;

				dda_setvars[ins_num].inacc = 0;
				dda_setvars[ins_num].invel = -vel_to_cmd;
				dda_setvars[ins_num].inpos = uniform_pos;
				dda_setvars[ins_num++].injerk = 0;

				dda_setvars[ins_num].inpos = pos;
				dda_setvars[ins_num].invel = 0;
				dda_setvars[ins_num].inacc = acc_to_cmd;
				dda_setvars[ins_num].injerk = 0;
			}
		}
		return ok;
}

