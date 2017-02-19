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
		case CMD_GO_HOME:
			break;
		case CMD_SET_MOTION:
			break;
		case CMD_START_MOTION:
			break;
		case CMD_PT_MODE:
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
		case CMD_SET_MOTION:
//		case CMD_UPLOAD_ENCODERS:
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
//	case CMD_UPLOAD_ENCODERS:	rtn = upload_encoders(pCmd->mark);	break;
	case CMD_ESTOP:		rtn = reset_estop();			break;
	case CMD_PT_MODE:	rtn = EnterPTmode(gCmd.mark);				break;
	default:
		rtn = RTN_INVALID_COMMAND;
		break;
	}

	return rtn;
}
