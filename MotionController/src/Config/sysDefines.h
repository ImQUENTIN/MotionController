/*
 * sysDefines.h
 *
 *  Created on: 2017-2-12
 *      Author: QIYUEXIN
 *  Contact me: qiyuexin@yeah.net
 */

#ifndef SYSDEFINES_H_
#define SYSDEFINES_H_

#include "config.h"


/*
 *  2. 与ARM协议部分
 */

// 报文的头
#define MSG_HEAD  '#'

#define CMD_RESET_ADDR			0x01
#define CMD_ESTOP_ADDR			0x02
#define CMD_GO_HOME_ADDR		0x03
#define CMD_SET_MOTION_ADDR		0x04
#define CMD_START_MOTION_ADDR	0x05




#endif /* SYSDEFINES_H_ */
