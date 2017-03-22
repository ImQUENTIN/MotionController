#ifndef __MY_TYPES_H__
#define __MY_TYPES_H__

#include "config.h"
#include "sysDefines.h"			// 系统定义文件，非用户自定义。


#ifndef MY_STD_TYPE
#define MY_STD_TYPE
	typedef           int word;
	typedef unsigned char byte;
	typedef unsigned char  uint8_t;
	typedef 		 char   int8_t;
	typedef unsigned  int uint16_t;
	typedef           int  int16_t;
	typedef unsigned long uint32_t;
	typedef          long  int32_t;

#endif
// 返回码
typedef enum ERROR_CODE {
	RTN_SUCC = 0,
	RTN_ERROR = -1,
	RTN_INVALID_COMMAND = -2,
	RTN_INVALID_MEMORY = -3,
	RTN_DIVIDED_BY_ZERO = -4,
	RTN_NO_SPACE = -5,
	RTN_CMD_EXECUTING = -6

}ERROR_CODE;

//////////////////////////////////////////////////////////
// 指令类型
typedef enum COMMAND_TYPE {
	CMD_MSG   = 0,
	CMD_RESET ,		// Clear data
	CMD_ESTOP,
	CMD_ACTIVATE,
	CMD_START,
	CMD_SET_DDA,
	CMD_GO_HOME,
	CMD_PT_MODE,
	CMD_RD_DDA,
	CMD_RD_MSTA,
	CMD_RD_MFIFO
//	CMD_UPLOAD_ENCODERS,

}COMMAND_TYPE;

/* Velocity Planning Parameters
 * 速度规划的运动参数
 */
typedef struct {
	int32_t max_rise_acc;		// 最大上升加速度
	int32_t max_down_acc;		// 最大下降加速度
	int32_t max_even_vel;		// 匀速阶段最大速度
}VP_PARAM_S;

typedef struct {
	int32_t pos;
	int32_t vel;
	int32_t acc;
	int32_t jerk;
}DDA_VARS_S;

typedef struct{
	int32_t Pos;
	int32_t PrevPos;
	int32_t Period;
}PT_VARS_S;

// user 指令结构体
typedef struct {
	COMMAND_TYPE type;   		// 指令
	int serial, echoSerial;

	// parameters
	unsigned short mark;

	// axis variables
	DDA_VARS_S setDDA[AXISNUM];

	// PT variables
//		VP_PARAM_S vp_param[AXISNUM];		// velocity plan parameters.
	PT_VARS_S ptdata[AXISNUM];

	// data bufer
	uint16_t dat_buf[COMMUNICATION_MAX_LEN];

}COMMAND_S;

/*
//////////////////////////////////////////////////////
// 指令类型
typedef enum MOT_COMMAND_TYPE {
	MOT_CMD_SET_IO_OUTPUT = 0,
	MOT_CMD_SET_MOTION,
	MOT_CMD_START_MOTION
}COMMAND_TYPE;

// task 指令结构
typedef struct {
	MOT_COMMAND_TYPE type;   		// 指令
	int serial, echoSerial;

	// parameters
	unsigned short mark;
}MOT_COMMAND_S;
*/

//////////////////////////////////////////////////////////
// 状态类型
typedef enum STATE_TYPE {
	STATE_IDLE = 0,
	STATE_READY,
	STATE_RUNNING,
	STATE_ESTOP
}STATE_TYPE;

// 电机结构体变量
typedef struct {
	int encoder;
	int profile_pos;
	int profile_vel;
	int profile_acc;
	//
	unsigned int state;  // 0: alarm,  1:

}AXIS_S;


// 状态结构体
typedef struct {
	STATE_TYPE state;
	AXIS_S axis[AXISNUM];

}SYS_INFO;







#endif // __MY_TYPES_H__
