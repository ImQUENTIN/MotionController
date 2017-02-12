#ifndef __MY_TYPES_H__
#define __MY_TYPES_H__

#include "config.h"
#include "sysDefines.h"			// ϵͳ�����ļ������û��Զ��塣


#ifndef MY_STD_TYPE
#define MY_STD_TYPE
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
	RTN_ERROR,
	RTN_INVALID_COMMAND,
	RTN_INVALID_MEMORY,
	RTN_DIVIDED_BY_ZERO
}ERROR_CODE;

//////////////////////////////////////////////////////////
// ָ������
typedef enum COMMAND_TYPE {
	CMD_RESET =0,	// Clear data
	CMD_ESTOP,
	CMD_GO_HOME,
	CMD_SET_MOTION,
	CMD_START_MOTION
//	CMD_UPLOAD_ENCODERS,

}COMMAND_TYPE;

// user ָ��ṹ��
typedef struct {
	COMMAND_TYPE type;   		// ָ��
	int serial, echoSerial;

	// parameters
	unsigned short mark;

	// axis varibles
	int32_t position[AXISNUM];
	int32_t velocity[AXISNUM];
	int32_t acceleration[AXISNUM];
	int32_t jerk[AXISNUM];

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

// ״̬�ṹ��
typedef struct {
	STATE_TYPE state;
	int encoders[AXISNUM];

}MOTOR_INFO;


// ����ṹ�����
typedef struct {
	int encoder[AXISNUM];
	int profile_pos[AXISNUM];
	int profile_vel[AXISNUM];
	int profile_acc[AXISNUM];

	//
	unsigned int state[AXISNUM];  // 0: alarm,  1:

}AXIS_S;



#endif // __MY_TYPES_H__
