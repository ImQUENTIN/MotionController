#ifndef __MY_TYPES_H__
#define __MY_TYPES_H__

#include "config.h"
#include "sysDefines.h"			// ϵͳ�����ļ������û��Զ��塣


#ifndef MY_STD_TYPE
#define MY_STD_TYPE
	typedef unsigned int word;
	typedef unsigned char byte;
	typedef unsigned char  uint8_t;
	typedef 		 char   int8_t;
	typedef unsigned  int uint16_t;
	typedef           int  int16_t;
	typedef unsigned long uint32_t;
	typedef          long  int32_t;

#endif
// ������
typedef enum ERROR_CODE {
	RTN_SUCC = 0,
	RTN_ERROR = -1,
	RTN_INVALID_COMMAND = -2,
	RTN_INVALID_MEMORY = -3,
	RTN_DIVIDED_BY_ZERO = -4,
	RTN_NO_SPACE = -5,
	RTN_CMD_EXECUTING = -6,
	RTN_TRANS_STATE_INVALID = -7

}ERROR_CODE;

typedef enum  {
	AXIS1 = 0x01,
	AXIS2 = 0x02,
	AXIS3 = 0x04,
	AXIS4 = 0x08,
	AXIS5 = 0x10,
	AXIS6 = 0x20,
	AXIS7 = 0x40,
	AXIS8 = 0x80,
	AXIS_ALL = 0xff
}AXIS_ITEM;

//////////////////////////////////////////////////////////
// ָ������
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
	CMD_RD_MFIFO,
	CMD_RD_SRAM
//	CMD_UPLOAD_ENCODERS,

}COMMAND_TYPE;

/* Velocity Planning Parameters
 * �ٶȹ滮���˶�����
 */
typedef struct {
	int32_t max_rise_acc;		// ����������ٶ�
	int32_t max_down_acc;		// ����½����ٶ�
	int32_t max_even_vel;		// ���ٽ׶�����ٶ�
}VP_PARAM_S;

typedef struct {
	int32_t p1;
	int32_t p2;
	int32_t p3;
	int32_t p4;
}CMD_VARS_S;

/* DDA ģʽ����
 * dda_pvt ģʽʹ�ñ���
 */
typedef struct {
	int32_t aim_pos;		// Ŀ��λ��
	int32_t start_vel;		// ��ʼ�ٶ�
	int32_t start_acc;		// ��ʼ���ٶ�
	int32_t min_period;		// ��С����ʱ��
}PVAT_S;


typedef struct{
	int32_t Pos;
	int32_t PrevPos;
	int32_t Period;
}PT_VARS_S;

// user ָ��ṹ��
typedef struct {
	COMMAND_TYPE type;   		// ָ��
	int serial, echoSerial;

	// parameters
	unsigned short mark;

	// DDA MODE: pvat
	PVAT_S pvat[AXISNUM];

	// JOG MODE: vad
	int32_t   jog_vel[AXISNUM];

	// PT variables
//		VP_PARAM_S vp_param[AXISNUM];		// velocity plan parameters.
	PT_VARS_S ptdata[AXISNUM];

	// data bufer
	uint16_t dat_buf[COMMUNICATION_MAX_LEN];

}COMMAND_S;

/*
//////////////////////////////////////////////////////
// ָ������
typedef enum MOT_COMMAND_TYPE {
	MOT_CMD_SET_IO_OUTPUT = 0,
	MOT_CMD_SET_MOTION,
	MOT_CMD_START_MOTION
}COMMAND_TYPE;

// task ָ��ṹ
typedef struct {
	MOT_COMMAND_TYPE type;   		// ָ��
	int serial, echoSerial;

	// parameters
	unsigned short mark;
}MOT_COMMAND_S;
*/

//////////////////////////////////////////////////////////
// ״̬����
typedef enum STATE_TYPE {
	STATE_IDLE = 0,
	STATE_READY,
	STATE_RUNNING,
	STATE_ESTOP
}STATE_TYPE;

// ����ṹ�����
typedef struct {
	int encoder;
	int profile_pos;
	int profile_vel;
	int profile_acc;
	//
	unsigned int state;  // 0: alarm,  1:

}AXIS_S;


// ״̬�ṹ��
typedef struct {
	STATE_TYPE state;
	AXIS_S axis[AXISNUM];

}SYS_INFO;


#endif // __MY_TYPES_H__
