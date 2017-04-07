/*
 * taskExecute.h
 *
 *  Created on: 2017-2-10
 *      Author: Administrator
 */

#ifndef TASKEXECUTE_H_
#define TASKEXECUTE_H_

ERROR_CODE Message();
ERROR_CODE Reset();
ERROR_CODE Estop();
ERROR_CODE Activate();
ERROR_CODE Start();
ERROR_CODE SetDDA();
ERROR_CODE GoHome();
ERROR_CODE EnterPTmode();

ERROR_CODE ReadDDA(void);
ERROR_CODE ReadMfifo();
ERROR_CODE ReadMotor();
ERROR_CODE ReadSram();


#endif /* TASKEXECUTE_H_ */
