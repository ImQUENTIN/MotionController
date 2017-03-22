/*
 * taskComm.c
 *
 *  Created on: 2017-2-10
 *      Author: Administrator
 */


#include "my_project.h"

// ȫ�ֱ���
COMMAND_S gCmd;

ERROR_CODE decoupleCommand(word *pCmd, short cmdLen);



/*
 *  �Ƿ����µ�ָ��еĻ�����RTN_SUCC;
 */
ERROR_CODE checkNewCommand()
{
	int dat;
	ERROR_CODE rtn = RTN_ERROR;
	static short cmdLen;

	static word cmdBuf[COMMUNICATION_MAX_LEN] = {1,3,6,3,3,6,0,0,2,5,1,};	// ָ�����
	/*
	 *  �˳�������
	 *  SPI���յ�������ȡ�գ�����ȡ��֮ǰ����һ֡������ָ�
	 */

	while( RTN_ERROR != cb_get(&Spia.cb_rx, &dat)  )
	{
#if 0
		// ���Բ���
		if( dat == 0x23){
			for( cmdLen =0; cmdLen <11; cmdLen++)
				if(SpiaRegs.SPIFFTX.bit.TXFFST < 16)
//					SpiaRegs.SPITXBUF = cmdBuf[cmdLen];
//				else
					cb_append(&Spia.cb_tx, &cmdBuf[cmdLen]);
			SpiaRegs.SPIFFTX.bit.TXFFINTCLR = 1;	// load
		}
#else
		// �н��յ����ݣ������ݰ���ȡָ�
		if (RTN_SUCC == protocol(dat, cmdBuf, &cmdLen)) {
			rtn = decoupleCommand(cmdBuf, cmdLen);
			cmdLen = 0;
			return rtn;
		}
#endif
	}
	return rtn;
}


ERROR_CODE decoupleCommand(word *pCmd, short cmdLen)
{
	int i;
	word *ptr;
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
				memcpy(&gCmd.setDDA[i].pos,ptr,8);
//				gCmd.setDDA[i].pos	= *ptr++<<16 + *ptr++;
//				gCmd.setDDA[i].vel	= *ptr++<<16 + *ptr++;
//				gCmd.setDDA[i].acc	= *ptr++<<16 + *ptr++;
//				gCmd.setDDA[i].jerk	= *ptr++<<16 + *ptr++;
				ptr += 8;
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
		ptr = &pCmd[2];
		for (i = 0; i < AXISNUM; i++) {
			if ((gCmd.mark >> i) & 0x01) {
				memcpy(&gCmd.ptdata[i].Pos,ptr,2);
				memcpy(&gCmd.ptdata[i].Period,ptr+2,2);
				ptr += 4;
			}
		}
		gCmd.serial++;
		break;

 /* ��DDR */
	case CMD_RD_DDA_ADDR:
		gCmd.type = CMD_RD_DDA;
		gCmd.mark = pCmd[1];
		gCmd.serial++;
		break;

	case CMD_RD_MSTA_ADDR:
		gCmd.type = CMD_RD_MSTA;
		gCmd.mark = pCmd[1];
		gCmd.serial++;
		break;

	case CMD_RD_MFIFO_ADDR:
		gCmd.type = CMD_RD_MFIFO;
		gCmd.mark = pCmd[1];
		gCmd.serial++;
		break;

	default:
		rtn = RTN_INVALID_COMMAND;
		break;
	}

	return rtn;
}




ERROR_CODE receiveCommand(word *cmd, int *size)
{

	return RTN_SUCC;
}

ERROR_CODE sendData()
{
	return RTN_SUCC;
}

