#ifndef _PROCOTOL_H_
#define _PROCOTOL_H_


#include "sysTypes.h"

/*
 *  2. 与ARM协议部分
 */

// 报文的头
#define MSG_HEAD  '#'

#define CMD_MSG_ADDR			0x00
#define CMD_RESET_ADDR			0x01
#define CMD_ESTOP_ADDR			0x02
#define CMD_ACTIVATE_ADDR		0x03
#define CMD_START_ADDR			0x04
#define CMD_SET_DDA_ADDR		0x05
#define CMD_GO_HOME_ADDR		0x06
#define CMD_PT_MODE_ADDR		0x07





ERROR_CODE protocol(word chc, word *dat_buf, short *dat_len);

#endif
