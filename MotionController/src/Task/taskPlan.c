#include "my_project.h"
#include "taskExecute.h"

// 指令缓冲区
extern COMMAND_S gCmd;
SYS_INFO gSysInfo;


ERROR_CODE taskPlan()
{
	ERROR_CODE rtn = RTN_SUCC;
	if (gCmd.serial == gCmd.echoSerial) return RTN_ERROR;

	switch(gSysInfo.state)
	{
	case STATE_IDLE:
		switch(gCmd.type)
		{
		case CMD_MSG:
		case CMD_RESET:
			rtn = handleCommand(&gCmd);
			break;
		case CMD_ACTIVATE:
		case CMD_START:
			rtn = handleCommand(&gCmd);
			break;
		case CMD_SET_DDA:
			//		case CMD_UPLOAD_ENCODERS:
						rtn = handleCommand(&gCmd);
			break;
		case CMD_PT_MODE:
		case CMD_GO_HOME:
		case CMD_RD_DDA:
		case CMD_RD_MSTA:
		case CMD_RD_MFIFO:
		case CMD_RD_SRAM:
			rtn = handleCommand(&gCmd);
			break;
		default:
			rtn = RTN_INVALID_COMMAND;
		}
		break;

	case STATE_READY:
		break;

	case STATE_RUNNING:
		switch(gCmd.type)
		{
		case CMD_SET_DDA:

			break;

		case CMD_RD_DDA:
			rtn = handleCommand(&gCmd);
			break;

		case CMD_RD_MSTA:
			rtn = handleCommand(&gCmd);
			break;

		case CMD_RD_MFIFO:
			rtn = handleCommand(&gCmd);
			break;

		case CMD_RD_SRAM:
			rtn = handleCommand(&gCmd);
			break;

		case CMD_RD_PVAJ:
			rtn = handleCommand(&gCmd);
			break;

		default:
			rtn = RTN_INVALID_COMMAND;
		}
		break;

	case STATE_ESTOP:
		switch(gCmd.type)
		{
//		case CMD_UPLOAD_ENCODERS:
		case CMD_ESTOP:
			rtn = handleCommand(&gCmd);
			break;

		default:
			rtn = RTN_INVALID_COMMAND;
		}
		break;

	default:
		break;
	}

	gCmd.echoSerial = gCmd.serial;
	return rtn;
}


ERROR_CODE handleCommand(COMMAND_S *pCmd )
{
	ERROR_CODE rtn = RTN_ERROR;
	// 新指令
	switch(gCmd.type)
	{
	case CMD_MSG:			rtn = Message();		break;
	case CMD_RESET:			rtn = Reset();			break;
	case CMD_ESTOP:			rtn = Estop();			break;
	case CMD_ACTIVATE:		rtn = Activate();		break;
	case CMD_START:			rtn = Start();			break;
	case CMD_SET_DDA:		rtn = SetDDA();			break;
	case CMD_PT_MODE:		rtn = EnterPTmode();	break;
	case CMD_RD_DDA:        rtn = ReadDDA();        break;
	case CMD_RD_MSTA:		rtn = ReadMotor();		break;
	case CMD_RD_MFIFO:		rtn = ReadMfifo();		break;
	case CMD_RD_SRAM:		rtn = ReadSram();		break;


	default:
		rtn = RTN_INVALID_COMMAND;
		break;
	}

	return rtn;
}
