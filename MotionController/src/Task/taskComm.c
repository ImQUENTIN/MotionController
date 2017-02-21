/*
 * taskComm.c
 *
 *  Created on: 2017-2-10
 *      Author: Administrator
 */


#include "my_project.h"

// ȫ�ֱ���
COMMAND_S gCmd;

ERROR_CODE decoupleCommand(uint8_t *pCmd, short cmdLen);



/*
 *  �Ƿ����µ�ָ��еĻ�����RTN_SUCC;
 */
ERROR_CODE checkNewCommand()
{
	char cTmp;
	ERROR_CODE rtn = RTN_ERROR;
	static short cmdLen;

	static uint8_t cmdBuf[COMMUNICATION_MAX_LEN];	// ָ�����
	/*
	 *  �˳�������
	 *  SPI���յ�������ȡ�գ�����ȡ��֮ǰ����һ֡������ָ�
	 */
//	while( RTN_SUCC == Spia_getchar(&cTmp)  )
	{
		// �н��յ����ݣ������ݰ���ȡָ�
		if (RTN_SUCC == protocol(cTmp, cmdBuf, &cmdLen)) {
			rtn = decoupleCommand(cmdBuf, cmdLen);
			cmdLen = 0;
			return rtn;
		}
	}
	return rtn;
}

int32_t my_atoi(uint8_t *dat){
	int32_t tmp = 0;
	int i;
	for(i=0; i<4; i++){
		tmp <<= 8;
		tmp += dat[i]&0xff;
	}
	return tmp;
}

ERROR_CODE decoupleCommand(uint8_t *pCmd, short cmdLen)
{
	int i;
	uint8_t *ptr;
	ERROR_CODE rtn = RTN_SUCC;
	if (cmdLen <= 0) return RTN_ERROR;

	// ָ�����
	switch(pCmd[0]) {

    
  /* ��λ�� */
	case CMD_RESET_ADDR:
		gCmd.type = CMD_RESET; // 
		gCmd.mark = pCmd[1];
		gCmd.serial++;
		break;

  /* ֹͣ�� */
	case CMD_ESTOP_ADDR:
		gCmd.type = CMD_ESTOP; //
		gCmd.mark = pCmd[1];
		gCmd.serial++;
		break;

  /* ������ */
	case CMD_ACTIVATE_ADDR:
		gCmd.type = CMD_ACTIVATE; //
		gCmd.mark = pCmd[1];
		gCmd.serial++;
		break;

  /* ������ */
	case CMD_START_ADDR:
		gCmd.type = CMD_START; //
		gCmd.mark = pCmd[1];
		gCmd.serial++;
		break;

  /* ֱ������DDAֵ */
	case CMD_SET_DDA_ADDR:
		gCmd.type = CMD_SET_DDA; // �����˶�ָ�����
		gCmd.mark = pCmd[1];
		ptr = &pCmd[2];
		for (i = 0; i < AXISNUM; i++) {
			if ((gCmd.mark >> i) & 0x01) {
				gCmd.setDDA[i].pos	= my_atoi(ptr);
				gCmd.setDDA[i].vel	= my_atoi(ptr+4);
				gCmd.setDDA[i].acc	= my_atoi(ptr+8);
				gCmd.setDDA[i].jerk	= my_atoi(ptr+12);
				ptr += 16;
			}
		}
		gCmd.serial++;
		break;

  /* �����˶� */
	case CMD_GO_HOME_ADDR:
		gCmd.type = CMD_GO_HOME; //
		gCmd.mark = pCmd[1];
		gCmd.serial++;
		break;

  /* PTģʽ */
	case CMD_PT_MODE_ADDR:
		gCmd.type = CMD_PT_MODE; //
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

