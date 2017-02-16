
#include "sysTypes.h"
#include "PTmode.h"
#include "math.h"
#include "string.h"


struct DDA_SET_VARS{
	int32_t inpos;
	int32_t invel;
	int32_t inacc;
	int32_t injerk;
};

struct DDA_SET_VARS dda_setvars[3];
int ins_num = 0;	// ָ�����

int PT_Initial()    //��ʼ�� PTģʽ��ѭ�������� pt_buf
{
	memset(&pt_buf, 0, sizeof(CIRCLE_BUFFER_S));
	cb_create(&pt_buf, sizeof(PT_DATA_S), 6);
	return RTN_SUCC;
}



ERROR_CODE PT_Mode(int axis, int pos, int time)
{
	PT_DATA_S rise_data;
	PT_DATA_S uniform_data;
	PT_DATA_S down_data;

	int critical_t ;
	if(pos > 0)
	{
		if(pos <= critical_pos)
		{
			critical_t = sqrt(4*pos/max_acc);
			if(time < critical_t) return RTN_INVALID_COMMAND; //λ�ú�ʱ�����ò�����

			if(time == critical_t)    //ʱ��Ϊ�ٽ�ֵ-�ٶȹ켣-����
			{
				rise_data.jerk = 0;
				rise_data.acc = max_acc;
				rise_data.vel = 0.5*max_acc*time;
				rise_data.pos = 0.5*pos;
				rise_data.time = 0.5*time;

				uniform_data.jerk = 0;
				uniform_data.acc = 0;
				uniform_data.vel = 0;
				uniform_data.pos = 0;
				uniform_data.time = 0;

				down_data.jerk = 0;
				down_data.acc = -max_acc;
				down_data.vel = 0;
				down_data.pos = pos;
				down_data.time = time;


				//��ȡ����    int cb_append(CIRCLE_BUFFER_S *buf, void* block_dat)
				cb_append(&pt_buf, &rise_data);
				cb_append(&pt_buf, &uniform_data);
				cb_append(&pt_buf, &down_data);

				return RTN_SUCC;
			}

			else    //ʱ�����-�ٶȹ켣-����
			{
				critical_t = sqrt(time*time/4 - pos/max_acc)+0.5*time;  //�˴���Ϊ����ʱ�䣬������Ϊ�����˶�ʱ��

				rise_data.jerk = 0;
				rise_data.acc = max_acc;
				rise_data.vel = critical_t*max_acc;
				rise_data.pos = 0.5*max_acc*critical_t*critical_t;
				rise_data.time = critical_t;

				uniform_data.jerk = 0;
				uniform_data.acc = 0;
				uniform_data.vel = rise_data.vel;
				uniform_data.pos = pos - rise_data.pos;
				uniform_data.time = time - critical_t;

				down_data.jerk = 0;
				down_data.acc = -max_acc;
				down_data.vel = 0;
				down_data.pos = pos;
				down_data.time = time;

				//��ȡ����
				cb_append(&pt_buf, &rise_data);
				cb_append(&pt_buf, &uniform_data);
				cb_append(&pt_buf, &down_data);

				return RTN_SUCC;

			}

		}
		else     //����ﵽ����ٶ�-�ٶȹ켣-����
		{
			critical_t = (pos - max_vel*max_vel/max_acc)/max_vel + 2*max_vel/max_acc;

			if(time < critical_t) return RTN_INVALID_COMMAND;  //λ��ʱ�䲻����

			if(time == critical_t)   //����ٶȵ����ι켣
			{
				rise_data.jerk = 0;
				rise_data.acc = max_acc;
				rise_data.vel = max_vel;
				rise_data.pos = 0.5*max_vel*max_vel/max_acc;
				rise_data.time = max_vel/max_acc;

				uniform_data.jerk = 0;
				uniform_data.acc = 0;
				uniform_data.vel = max_vel;
				uniform_data.pos = pos - rise_data.pos;
				uniform_data.time = time - rise_data.time;

				down_data.jerk = 0;
				down_data.acc = -max_acc;
				down_data.vel = 0;
				down_data.pos = pos;
				down_data.time = time;

				//��ȡ����
				cb_append(&pt_buf, &rise_data);
				cb_append(&pt_buf, &uniform_data);
				cb_append(&pt_buf, &down_data);

				return RTN_SUCC;
			}
			else
			{
				critical_t = sqrt(time*time/4 - pos/max_acc)+0.5*time; //�˴���Ϊ����ʱ�䣬������Ϊ�����˶�ʱ��

				rise_data.jerk = 0;
				rise_data.acc = max_acc;
				rise_data.vel = critical_t*max_acc;
				rise_data.pos = 0.5*max_acc*critical_t*critical_t;
				rise_data.time = critical_t;

				uniform_data.jerk = 0;
				uniform_data.acc = 0;
				uniform_data.vel = rise_data.vel;
				uniform_data.pos = pos - rise_data.pos;
				uniform_data.time = time - critical_t;

				down_data.jerk = 0;
				down_data.acc = -max_acc;
				down_data.vel = 0;
				down_data.pos = pos;
				down_data.time = time;

				//��ȡ����
				cb_append(&pt_buf, &rise_data);
				cb_append(&pt_buf, &uniform_data);
				cb_append(&pt_buf, &down_data);

				return RTN_SUCC;

			}

		}
	}
	if(pos < 0)
	{
		if(pos >= critical_pos)
		{
			critical_t = sqrt(4*(-pos)/max_acc);
			if(time < critical_t) return RTN_INVALID_COMMAND; //λ�ú�ʱ�����ò�����

			if(time == critical_t)    //ʱ��Ϊ�ٽ�ֵ-�ٶȹ켣-����
			{
				rise_data.jerk = 0;
				rise_data.acc = -max_acc;
				rise_data.vel = -0.5*max_acc*time;
				rise_data.pos = 0.5*pos;
				rise_data.time = 0.5*time;

				uniform_data.jerk = 0;
				uniform_data.acc = 0;
				uniform_data.vel = 0;
				uniform_data.pos = 0;
				uniform_data.time = 0;

				down_data.jerk = 0;
				down_data.acc = max_acc;
				down_data.vel = 0;
				down_data.pos = pos;
				down_data.time = time;

				//��ȡ����
				cb_append(&pt_buf, &rise_data);
				cb_append(&pt_buf, &uniform_data);
				cb_append(&pt_buf, &down_data);

				return RTN_SUCC;
			}

			else    //ʱ�����-�ٶȹ켣-����
			{
				critical_t = sqrt(time*time/4 - (-pos)/max_acc)+0.5*time;

				rise_data.jerk = 0;
				rise_data.acc = -max_acc;
				rise_data.vel = -critical_t*max_acc;
				rise_data.pos = -0.5*max_acc*critical_t*critical_t;
				rise_data.time = critical_t;

				uniform_data.jerk = 0;
				uniform_data.acc = 0;
				uniform_data.vel = rise_data.vel;
				uniform_data.pos = pos - rise_data.pos;
				uniform_data.time = time - critical_t;

				down_data.jerk = 0;
				down_data.acc = max_acc;
				down_data.vel = 0;
				down_data.pos = pos;
				down_data.time = time;

				//��ȡ����
				cb_append(&pt_buf, &rise_data);
				cb_append(&pt_buf, &uniform_data);
				cb_append(&pt_buf, &down_data);

				return RTN_SUCC;
			}

		}
		else     //����ﵽ����ٶ�-�ٶȹ켣-����
		{
			critical_t = ((-pos) - max_vel*max_vel/max_acc)/max_vel + 2*max_vel/max_acc;

			if(time < critical_t) return RTN_INVALID_COMMAND;  //λ��ʱ�䲻����

			if(time == critical_t)   //����ٶȵ����ι켣
			{
				rise_data.jerk = 0;
				rise_data.acc = -max_acc;
				rise_data.vel = -max_vel;
				rise_data.pos = -0.5*max_vel*max_vel/max_acc;
				rise_data.time = max_vel/max_acc;

				uniform_data.jerk = 0;
				uniform_data.acc = 0;
				uniform_data.vel = -max_vel;
				uniform_data.pos = pos - rise_data.pos;
				uniform_data.time = time - rise_data.time;

				down_data.jerk = 0;
				down_data.acc = max_acc;
				down_data.vel = 0;
				down_data.pos = pos;
				down_data.time = time;

				//��ȡ����
				cb_append(&pt_buf, &rise_data);
				cb_append(&pt_buf, &uniform_data);
				cb_append(&pt_buf, &down_data);

				return RTN_SUCC;
			}
			else
			{
				critical_t = sqrt(time*time/4 - (-pos)/max_acc)+0.5*time;

				rise_data.jerk = 0;
				rise_data.acc = -max_acc;
				rise_data.vel = -critical_t*max_acc;
				rise_data.pos = -0.5*max_acc*critical_t*critical_t;
				rise_data.time = critical_t;

				uniform_data.jerk = 0;
				uniform_data.acc = 0;
				uniform_data.vel = -rise_data.vel;
				uniform_data.pos = pos - rise_data.pos;
				uniform_data.time = time - critical_t;

				down_data.jerk = 0;
				down_data.acc = max_acc;
				down_data.vel = 0;
				down_data.pos = pos;
				down_data.time = time;

				//��ȡ����
				cb_append(&pt_buf, &rise_data);
				cb_append(&pt_buf, &uniform_data);
				cb_append(&pt_buf, &down_data);

				return RTN_SUCC;
			}

		}
	}
}

