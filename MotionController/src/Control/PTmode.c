#include "sysTypes.h"
#include "CircleBuffer.h"
#include "PTmode.h"
#include "math.h"
#include "string.h"
#include "myram.h"
#include "DSP2833x_Device.h"
#include "mymotors.h"

//CIRCLE_BUFFER_S pt_buf[AXISNUM];	// AXISNUM个轴的pt_buf.
VP_PARAM_S vp_param[AXISNUM];		// velocity plan parameters.
extern uint8_t flag;
extern COMMAND_S gCmd;
const int time_ms2us =  50*1000;


//int PT_Initial()    //初始化 PT模式的循环缓冲区 pt_buf
//{
//	int axis;
//	for( axis=0; axis<AXISNUM; axis++){
//		memset(&pt_buf[axis], 0, sizeof(CIRCLE_BUFFER_S));
//		cb_create(&pt_buf[axis], sizeof(PT_DATA_S), 6);
//
//		// 加载
//		vp_param[axis].max_rise_acc 	 = DEFUALT_MAX_RISE_ACC;
//		vp_param[axis].max_down_acc 	 = DEFUALT_MAX_DOWN_ACC;
//		vp_param[axis].max_even_vel 	 = DEFUALT_MAX_EVEN_VEL;
//	}
//	return RTN_SUCC;
//}

// 上升和下降的加速度相同
ERROR_CODE PT_Mode(int axis, PT_VARS_S *pt) {
	double vel;
	int32_t delta_pos;
	DDA_VARS_S dda;

	delta_pos = pt->Pos - pt->PrevPos;
	vel = delta_pos * 1000 / pt->Period;
	if (vel >= 0)
		vel += 0.5;
	else
		vel -= 0.5;
	pt->PrevPos = pt->Pos;

	dda.pos = pt->Pos;
	dda.vel = (int32_t)vel;
	dda.acc = 0;
	dda.jerk = pt->Period*time_ms2us;
	return  cb_append(&ram_dda[axis], &dda);
}

ERROR_CODE PT_Data()
{
		int axis;
		DDA_VARS_S dda;

		for (axis = 0; axis < AXISNUM; axis++) {
			// 取轴axis, 压入DDA
			if( MotorRegs[axis].MCTL.bit.ENA && M_freeSpace(axis) )
				if( RTN_ERROR != cb_get(&ram_dda[axis], &dda) )
					M_SetDDA(axis, &dda);
		}
		return RTN_SUCC;
}




//	PT_DATA_S rise_data;
//	PT_DATA_S even_data;
//	PT_DATA_S down_data;
//	int critical_t, critical_pos ;
//	int max_acc = vp_param[axis].max_rise_acc;
//	int max_vel = vp_param[axis].max_even_vel;
//
//	critical_pos = max_vel * max_vel / max_acc;
////	critical_pos = vp_param[axis].max_even_vel *
////			(0.5 * vp_param[axis].max_even_vel / vp_param[axis].max_rise_acc +
////			 0.5 * vp_param[axis].max_even_vel / vp_param[axis].max_down_acc );
//
//	if(pos > 0)
//	{
//		if(pos <= critical_pos)
//		{
//			critical_t = sqrt(4*pos/max_acc);
//			if(time < critical_t)
//				return RTN_INVALID_COMMAND; //位置和时间设置不合理
//
//			if(time == critical_t)    //时间为临界值-速度轨迹-三角
//			{
//				rise_data.jerk = 0;
//				rise_data.acc = max_acc;
//				rise_data.vel = 0;		//0.5*max_acc*time;
//				rise_data.pos = 0.5*pos;
//				rise_data.time = 0.5*time;
//
//				even_data.jerk = 0;
//				even_data.acc = 0;
//				even_data.vel = 0;
//				even_data.pos = 0;
//				even_data.time = 0;
//
//				down_data.jerk = 0;
//				down_data.acc = -max_acc;
//				down_data.vel = 0.5*max_acc*time;		//0;
//				down_data.pos = pos;
//				down_data.time = time;
//
//
//				//获取数据    int cb_append(CIRCLE_BUFFER_S *buf, void* block_dat)
//				M_SetDDA(rise_data.time, rise_data.vel, rise_data.acc, rise_data.jerk);
//				M_SetDDA(down_data.time, down_data.vel, down_data.acc, down_data.jerk);
//				//cb_append(&pt_buf[axis], &rise_data);
//				//cb_append(&pt_buf[axis], &even_data);
//				//cb_append(&pt_buf[axis], &down_data);
//
//				return RTN_SUCC;
//			}
//
//			else    //时间充足-速度轨迹-梯形
//			{
//				critical_t = sqrt(time*time/4 - pos/max_acc)+0.5*time;  //此处用为上升时间，其他用为整体运动时间
//
//				rise_data.jerk = 0;
//				rise_data.acc = max_acc;
//				rise_data.vel =   0;//                  critical_t*max_acc;
//				rise_data.pos = 0.5*max_acc*critical_t*critical_t;
//				rise_data.time = critical_t;
//
//				even_data.jerk = 0;
//				even_data.acc = 0;
//				even_data.vel = critical_t*max_acc;//				rise_data.vel;
//				even_data.pos = pos - rise_data.pos;
//				even_data.time = time - critical_t;
//
//				down_data.jerk = 0;
//				down_data.acc = -max_acc;
//				down_data.vel = even_data.vel;			//0;
//				down_data.pos = pos;
//				down_data.time = time;
//
//				//获取数据
//				M_SetDDA(rise_data.time, rise_data.vel, rise_data.acc, rise_data.jerk);
//				M_SetDDA(even_data.time, even_data.vel, even_data.acc, even_data.jerk);
//				M_SetDDA(down_data.time, down_data.vel, down_data.acc, down_data.jerk);
//				//cb_append(&pt_buf[axis], &rise_data);
//				//cb_append(&pt_buf[axis], &even_data);
//				//cb_append(&pt_buf[axis], &down_data);
//
//				return RTN_SUCC;
//
//			}
//
//		}
//		else     //必须达到最大速度-速度轨迹-梯形
//		{
//			critical_t = (pos - max_vel*max_vel/max_acc)/max_vel + 2*max_vel/max_acc;
//
//			if(time < critical_t) return RTN_INVALID_COMMAND;  //位置时间不合理
//
//			if(time == critical_t)   //最大速度的梯形轨迹
//			{
//				rise_data.jerk = 0;
//				rise_data.acc = max_acc;
//				rise_data.vel = 0;			//max_vel;
//				rise_data.pos = 0.5*max_vel*max_vel/max_acc;
//				rise_data.time = max_vel/max_acc;
//
//				even_data.jerk = 0;
//				even_data.acc = 0;
//				even_data.vel = max_vel;
//				even_data.pos = pos - rise_data.pos;
//				even_data.time = time - rise_data.time;
//
//				down_data.jerk = 0;
//				down_data.acc = -max_acc;
//				down_data.vel = max_vel;			//0;
//				down_data.pos = pos;
//				down_data.time = time;
//
//				//获取数据
//				M_SetDDA(rise_data.time, rise_data.vel, rise_data.acc, rise_data.jerk);
//				M_SetDDA(even_data.time, even_data.vel, even_data.acc, even_data.jerk);
//				M_SetDDA(down_data.time, down_data.vel, down_data.acc, down_data.jerk);
//				//cb_append(&pt_buf[axis], &rise_data);
//				//cb_append(&pt_buf[axis], &even_data);
//				//cb_append(&pt_buf[axis], &down_data);
//
//				return RTN_SUCC;
//			}
//			else
//			{
//				critical_t = sqrt(time*time/4 - pos/max_acc)+0.5*time; //此处用为上升时间，其他用为整体运动时间
//
//				rise_data.jerk = 0;
//				rise_data.acc = max_acc;
//				rise_data.vel = 0;			//critical_t*max_acc;
//				rise_data.pos = 0.5*max_acc*critical_t*critical_t;
//				rise_data.time = critical_t;
//
//				even_data.jerk = 0;
//				even_data.acc = 0;
//				even_data.vel = critical_t*max_acc;		//rise_data.vel;
//				even_data.pos = pos - rise_data.pos;
//				even_data.time = time - critical_t;
//
//				down_data.jerk = 0;
//				down_data.acc = -max_acc;
//				down_data.vel = even_data.vel ;		//0;
//				down_data.pos = pos;
//				down_data.time = time;
//
//				//获取数据
//				M_SetDDA(rise_data.time, rise_data.vel, rise_data.acc, rise_data.jerk);
//				M_SetDDA(even_data.time, even_data.vel, even_data.acc, even_data.jerk);
//				M_SetDDA(down_data.time, down_data.vel, down_data.acc, down_data.jerk);
//				//cb_append(&pt_buf[axis], &rise_data);
//				//cb_append(&pt_buf[axis], &even_data);
//				//cb_append(&pt_buf[axis], &down_data);
//
//				return RTN_SUCC;
//
//			}
//
//		}
//	}
//	if(pos < 0)
//	{
//		if(pos >= critical_pos)
//		{
//			critical_t = sqrt(4*(-pos)/max_acc);
//			if(time < critical_t) return RTN_INVALID_COMMAND; //位置和时间设置不合理
//
//			if(time == critical_t)    //时间为临界值-速度轨迹-三角
//			{
//				rise_data.jerk = 0;
//				rise_data.acc = -max_acc;
//				rise_data.vel = 0;		//-0.5*max_acc*time;
//				rise_data.pos = 0.5*pos;
//				rise_data.time = 0.5*time;
//
//				even_data.jerk = 0;
//				even_data.acc = 0;
//				even_data.vel = 0;
//				even_data.pos = 0;
//				even_data.time = 0;
//
//				down_data.jerk = 0;
//				down_data.acc = max_acc;
//				down_data.vel = -0.5*max_acc*time;		//0;
//				down_data.pos = pos;
//				down_data.time = time;
//
//				//获取数据
//				M_SetDDA(rise_data.time, rise_data.vel, rise_data.acc, rise_data.jerk);
//				//M_SetDDA(even_data.time, even_data.vel, even_data.acc, even_data.jerk);
//				M_SetDDA(down_data.time, down_data.vel, down_data.acc, down_data.jerk);
//				//cb_append(&pt_buf[axis], &rise_data);
//				//cb_append(&pt_buf[axis], &even_data);
//				//cb_append(&pt_buf[axis], &down_data);
//
//				return RTN_SUCC;
//			}
//
//			else    //时间充足-速度轨迹-梯形
//			{
//				critical_t = sqrt(time*time/4 - (-pos)/max_acc)+0.5*time;
//
//				rise_data.jerk = 0;
//				rise_data.acc = -max_acc;
//				rise_data.vel = 0;		//-critical_t*max_acc;
//				rise_data.pos = -0.5*max_acc*critical_t*critical_t;
//				rise_data.time = critical_t;
//
//				even_data.jerk = 0;
//				even_data.acc = 0;
//				even_data.vel = -critical_t*max_acc;		//rise_data.vel;
//				even_data.pos = pos - rise_data.pos;
//				even_data.time = time - critical_t;
//
//				down_data.jerk = 0;
//				down_data.acc = max_acc;
//				down_data.vel = even_data.vel;		//0;
//				down_data.pos = pos;
//				down_data.time = time;
//
//				//获取数据
//				M_SetDDA(rise_data.time, rise_data.vel, rise_data.acc, rise_data.jerk);
//				M_SetDDA(even_data.time, even_data.vel, even_data.acc, even_data.jerk);
//				M_SetDDA(down_data.time, down_data.vel, down_data.acc, down_data.jerk);
//				//cb_append(&pt_buf[axis], &rise_data);
//				//cb_append(&pt_buf[axis], &even_data);
//				//cb_append(&pt_buf[axis], &down_data);
//
//				return RTN_SUCC;
//			}
//
//		}
//		else     //必须达到最大速度-速度轨迹-梯形
//		{
//			critical_t = ((-pos) - max_vel*max_vel/max_acc)/max_vel + 2*max_vel/max_acc;
//
//			if(time < critical_t) return RTN_INVALID_COMMAND;  //位置时间不合理
//
//			if(time == critical_t)   //最大速度的梯形轨迹
//			{
//				rise_data.jerk = 0;
//				rise_data.acc = -max_acc;
//				rise_data.vel = 0;		//-max_vel;
//				rise_data.pos = -0.5*max_vel*max_vel/max_acc;
//				rise_data.time = max_vel/max_acc;
//
//				even_data.jerk = 0;
//				even_data.acc = 0;
//				even_data.vel = -max_vel;
//				even_data.pos = pos - rise_data.pos;
//				even_data.time = time - rise_data.time;
//
//				down_data.jerk = 0;
//				down_data.acc = max_acc;
//				down_data.vel = -max_vel;		//0;
//				down_data.pos = pos;
//				down_data.time = time;
//
//				//获取数据
//				M_SetDDA(rise_data.time, rise_data.vel, rise_data.acc, rise_data.jerk);
//				M_SetDDA(even_data.time, even_data.vel, even_data.acc, even_data.jerk);
//				M_SetDDA(down_data.time, down_data.vel, down_data.acc, down_data.jerk);
//				//cb_append(&pt_buf[axis], &rise_data);
//				//cb_append(&pt_buf[axis], &even_data);
//				//cb_append(&pt_buf[axis], &down_data);
//
//				return RTN_SUCC;
//			}
//			else
//			{
//				critical_t = sqrt(time*time/4 - (-pos)/max_acc)+0.5*time;
//
//				rise_data.jerk = 0;
//				rise_data.acc = -max_acc;
//				rise_data.vel = 0;		//-critical_t*max_acc;
//				rise_data.pos = -0.5*max_acc*critical_t*critical_t;
//				rise_data.time = critical_t;
//
//				even_data.jerk = 0;
//				even_data.acc = 0;
//				even_data.vel = -critical_t*max_acc;		//-rise_data.vel;
//				even_data.pos = pos - rise_data.pos;
//				even_data.time = time - critical_t;
//
//				down_data.jerk = 0;
//				down_data.acc = max_acc;
//				down_data.vel = even_data.vel;		//0;
//				down_data.pos = pos;
//				down_data.time = time;
//
//				//获取数据
//				M_SetDDA(rise_data.time, rise_data.vel, rise_data.acc, rise_data.jerk);
//				M_SetDDA(even_data.time, even_data.vel, even_data.acc, even_data.jerk);
//				M_SetDDA(down_data.time, down_data.vel, down_data.acc, down_data.jerk);
//				//cb_append(&pt_buf[axis], &rise_data);
//				//cb_append(&pt_buf[axis], &even_data);
//				//cb_append(&pt_buf[axis], &down_data);
//
//				return RTN_SUCC;
//			}
//
//		}
//	}
//	return RTN_SUCC;
//}

