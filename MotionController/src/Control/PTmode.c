
#include "sysTypes.h"
#include "CircleBuffer.h"
#include "PTmode.h"
#include "math.h"
#include "string.h"


CIRCLE_BUFFER_S pt_buf[AXISNUM];	// AXISNUM�����pt_buf.
VP_PARAM_S vp_param[AXISNUM];		// velocity plan parameters.


int PT_Initial()    //��ʼ�� PTģʽ��ѭ�������� pt_buf
{
	int axis;
	for( axis=0; axis<AXISNUM; axis++){
		memset(&pt_buf[axis], 0, sizeof(CIRCLE_BUFFER_S));
		cb_create(&pt_buf[axis], sizeof(PT_DATA_S), 6);

		// ����
		vp_param[axis].max_rise_acc 	 = DEFUALT_MAX_RISE_ACC;
		vp_param[axis].max_down_acc 	 = DEFUALT_MAX_DOWN_ACC;
		vp_param[axis].max_even_vel 	 = DEFUALT_MAX_EVEN_VEL;
	}
	return RTN_SUCC;
}

// �������½��ļ��ٶ���ͬ
ERROR_CODE PT_Mode(int axis, int pos, int time)
{
	PT_DATA_S rise_data;
	PT_DATA_S even_data;
	PT_DATA_S down_data;
	int critical_t, critical_pos ;
	int max_acc = vp_param[axis].max_rise_acc;
	int max_vel = vp_param[axis].max_even_vel;

	critical_pos = max_vel * max_vel / max_acc;
//	critical_pos = vp_param[axis].max_even_vel *
//			(0.5 * vp_param[axis].max_even_vel / vp_param[axis].max_rise_acc +
//			 0.5 * vp_param[axis].max_even_vel / vp_param[axis].max_down_acc );

	if(pos > 0)
	{
		if(pos <= critical_pos)
		{
			critical_t = sqrt(4*pos/max_acc);
			if(time < critical_t)
				return RTN_INVALID_COMMAND; //λ�ú�ʱ�����ò�����

			if(time == critical_t)    //ʱ��Ϊ�ٽ�ֵ-�ٶȹ켣-����
			{
				rise_data.jerk = 0;
				rise_data.acc = max_acc;
				rise_data.vel = 0;		//0.5*max_acc*time;
				rise_data.pos = 0.5*pos;
				rise_data.time = 0.5*time;

				even_data.jerk = 0;
				even_data.acc = 0;
				even_data.vel = 0;
				even_data.pos = 0;
				even_data.time = 0;

				down_data.jerk = 0;
				down_data.acc = -max_acc;
				down_data.vel = 0.5*max_acc*time;		//0;
				down_data.pos = pos;
				down_data.time = time;


				//��ȡ����    int cb_append(CIRCLE_BUFFER_S *buf, void* block_dat)
				cb_append(&pt_buf[axis], &rise_data);
				cb_append(&pt_buf[axis], &even_data);
				cb_append(&pt_buf[axis], &down_data);

				return RTN_SUCC;
			}

			else    //ʱ�����-�ٶȹ켣-����
			{
				critical_t = sqrt(time*time/4 - pos/max_acc)+0.5*time;  //�˴���Ϊ����ʱ�䣬������Ϊ�����˶�ʱ��

				rise_data.jerk = 0;
				rise_data.acc = max_acc;
				rise_data.vel =   0;//                  critical_t*max_acc;
				rise_data.pos = 0.5*max_acc*critical_t*critical_t;
				rise_data.time = critical_t;

				even_data.jerk = 0;
				even_data.acc = 0;
				even_data.vel = critical_t*max_acc;//				rise_data.vel;
				even_data.pos = pos - rise_data.pos;
				even_data.time = time - critical_t;

				down_data.jerk = 0;
				down_data.acc = -max_acc;
				down_data.vel = even_data.vel;			//0;
				down_data.pos = pos;
				down_data.time = time;

				//��ȡ����
				cb_append(&pt_buf[axis], &rise_data);
				cb_append(&pt_buf[axis], &even_data);
				cb_append(&pt_buf[axis], &down_data);

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
				rise_data.vel = 0;			//max_vel;
				rise_data.pos = 0.5*max_vel*max_vel/max_acc;
				rise_data.time = max_vel/max_acc;

				even_data.jerk = 0;
				even_data.acc = 0;
				even_data.vel = max_vel;
				even_data.pos = pos - rise_data.pos;
				even_data.time = time - rise_data.time;

				down_data.jerk = 0;
				down_data.acc = -max_acc;
				down_data.vel = max_vel;			//0;
				down_data.pos = pos;
				down_data.time = time;

				//��ȡ����
				cb_append(&pt_buf[axis], &rise_data);
				cb_append(&pt_buf[axis], &even_data);
				cb_append(&pt_buf[axis], &down_data);

				return RTN_SUCC;
			}
			else
			{
				critical_t = sqrt(time*time/4 - pos/max_acc)+0.5*time; //�˴���Ϊ����ʱ�䣬������Ϊ�����˶�ʱ��

				rise_data.jerk = 0;
				rise_data.acc = max_acc;
				rise_data.vel = 0;			//critical_t*max_acc;
				rise_data.pos = 0.5*max_acc*critical_t*critical_t;
				rise_data.time = critical_t;

				even_data.jerk = 0;
				even_data.acc = 0;
				even_data.vel = critical_t*max_acc;		//rise_data.vel;
				even_data.pos = pos - rise_data.pos;
				even_data.time = time - critical_t;

				down_data.jerk = 0;
				down_data.acc = -max_acc;
				down_data.vel = even_data.vel ;		//0;
				down_data.pos = pos;
				down_data.time = time;

				//��ȡ����
				cb_append(&pt_buf[axis], &rise_data);
				cb_append(&pt_buf[axis], &even_data);
				cb_append(&pt_buf[axis], &down_data);

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
				rise_data.vel = 0;		//-0.5*max_acc*time;
				rise_data.pos = 0.5*pos;
				rise_data.time = 0.5*time;

				even_data.jerk = 0;
				even_data.acc = 0;
				even_data.vel = 0;
				even_data.pos = 0;
				even_data.time = 0;

				down_data.jerk = 0;
				down_data.acc = max_acc;
				down_data.vel = -0.5*max_acc*time;		//0;
				down_data.pos = pos;
				down_data.time = time;

				//��ȡ����
				cb_append(&pt_buf[axis], &rise_data);
				cb_append(&pt_buf[axis], &even_data);
				cb_append(&pt_buf[axis], &down_data);

				return RTN_SUCC;
			}

			else    //ʱ�����-�ٶȹ켣-����
			{
				critical_t = sqrt(time*time/4 - (-pos)/max_acc)+0.5*time;

				rise_data.jerk = 0;
				rise_data.acc = -max_acc;
				rise_data.vel = 0;		//-critical_t*max_acc;
				rise_data.pos = -0.5*max_acc*critical_t*critical_t;
				rise_data.time = critical_t;

				even_data.jerk = 0;
				even_data.acc = 0;
				even_data.vel = -critical_t*max_acc;		//rise_data.vel;
				even_data.pos = pos - rise_data.pos;
				even_data.time = time - critical_t;

				down_data.jerk = 0;
				down_data.acc = max_acc;
				down_data.vel = even_data.vel;		//0;
				down_data.pos = pos;
				down_data.time = time;

				//��ȡ����
				cb_append(&pt_buf[axis], &rise_data);
				cb_append(&pt_buf[axis], &even_data);
				cb_append(&pt_buf[axis], &down_data);

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
				rise_data.vel = 0;		//-max_vel;
				rise_data.pos = -0.5*max_vel*max_vel/max_acc;
				rise_data.time = max_vel/max_acc;

				even_data.jerk = 0;
				even_data.acc = 0;
				even_data.vel = -max_vel;
				even_data.pos = pos - rise_data.pos;
				even_data.time = time - rise_data.time;

				down_data.jerk = 0;
				down_data.acc = max_acc;
				down_data.vel = -max_vel;		//0;
				down_data.pos = pos;
				down_data.time = time;

				//��ȡ����
				cb_append(&pt_buf[axis], &rise_data);
				cb_append(&pt_buf[axis], &even_data);
				cb_append(&pt_buf[axis], &down_data);

				return RTN_SUCC;
			}
			else
			{
				critical_t = sqrt(time*time/4 - (-pos)/max_acc)+0.5*time;

				rise_data.jerk = 0;
				rise_data.acc = -max_acc;
				rise_data.vel = 0;		//-critical_t*max_acc;
				rise_data.pos = -0.5*max_acc*critical_t*critical_t;
				rise_data.time = critical_t;

				even_data.jerk = 0;
				even_data.acc = 0;
				even_data.vel = -critical_t*max_acc;		//-rise_data.vel;
				even_data.pos = pos - rise_data.pos;
				even_data.time = time - critical_t;

				down_data.jerk = 0;
				down_data.acc = max_acc;
				down_data.vel = even_data.vel;		//0;
				down_data.pos = pos;
				down_data.time = time;

				//��ȡ����
				cb_append(&pt_buf[axis], &rise_data);
				cb_append(&pt_buf[axis], &even_data);
				cb_append(&pt_buf[axis], &down_data);

				return RTN_SUCC;
			}

		}
	}
	return RTN_SUCC;
}

