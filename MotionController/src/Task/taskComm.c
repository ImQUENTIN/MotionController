/*
 * taskComm.c
 *
 *  Created on: 2017-2-10
 *      Author: Administrator
 */


#include "my_project.h"

// 全局变量
COMMAND_S gCmd;

ERROR_CODE decoupleCommand(uint8_t *pCmd, short cmdLen);

/*
 *  是否有新的指令，有的话返回RTN_SUCC;
 */
ERROR_CODE checkNewCommand()
{
	uint8_t cTmp;
	ERROR_CODE rtn;
	short cmdLen;

	static uint8_t cmdBuf[COMMUNICATION_MAX_LEN];	// 指令缓冲区
	/*
	 *  退出条件：
	 *  SPI接收到的数据取空，或者取空之前遇到一帧完整的指令。
	 */
	while( RTN_SUCC == Spia_getchar(&cTmp) ){
		// 有接收到数据，从数据包提取指令。
		if (RTN_SUCC == protocol(cTmp, cmdBuf, &cmdLen)) {
			rtn = decoupleCommand(cmdBuf, cmdLen);
			cmdLen = 0;
			return RTN_SUCC;
		}
	}
	return rtn;
}


ERROR_CODE decoupleCommand(uint8_t *pCmd, short cmdLen)
{
	int i;
	uint8_t *ptr;
	ERROR_CODE rtn = RTN_SUCC;
	if (cmdLen <= 0) return RTN_ERROR;

	// 指令分析
	switch(pCmd[0]) {

	case CMD_SET_MOTION_ADDR:
		gCmd.type = CMD_SET_MOTION; // 设置运动指令参数
		gCmd.mark = pCmd[1];
		ptr = &pCmd[2];
		for (i = 0; i < AXISNUM; i++) {
			if ((gCmd.mark >> i) & 0x01) {
				gCmd.setDDA[i].pos	= *((int32_t*)(&ptr[0]));
				gCmd.setDDA[i].vel	= *((int32_t*)(&ptr[4]));
				gCmd.setDDA[i].acc	= *((int32_t*)(&ptr[8]));
				gCmd.setDDA[i].jerk	= *((int32_t*)(&ptr[12]));
				ptr += 16;
			}
		}
		gCmd.serial++;
		break;
    
  /* 复位清零 */
	case CMD_RESET_ADDR:
		gCmd.type = CMD_RESET; // 
		gCmd.mark = pCmd[1];
		gCmd.serial++;
		break;

  /* 停止运动 */
	case CMD_ESTOP_ADDR:
		gCmd.type = CMD_ESTOP; //
		gCmd.mark = pCmd[1];
		gCmd.serial++;
		break;

  /* 归零运动 */
	case CMD_GO_HOME_ADDR:
		gCmd.type = CMD_GO_HOME; //
		gCmd.mark = pCmd[1];
		gCmd.serial++;
		break;

  /* 启动运动 */
	case CMD_START_MOTION_ADDR:
		gCmd.type = CMD_START_MOTION; // 
		gCmd.mark = pCmd[1];
		gCmd.serial++;
		break;

  /* PT模式 */
	case CMD_PT_MODE_ADDR:
		gCmd.type = CMD_START_MOTION; //
		gCmd.mark = pCmd[1];
		gCmd.serial++;
		break;

	default:
		rtn = RTN_INVALID_COMMAND;
		break;
	}

	return rtn;
}




ERROR_CODE receiveCommand(uint8_t *cmd, int *size)
{

	return RTN_SUCC;
}

ERROR_CODE sendData()
{
	return RTN_SUCC;
}

