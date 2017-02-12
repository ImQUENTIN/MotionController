/*
 * taskComm.h
 *
 *  Created on: 2017-2-10
 *      Author: Administrator
 */

#ifndef TASKCOMM_H_
#define TASKCOMM_H_

#include "sysTypes.h"

ERROR_CODE checkNewCommand();
ERROR_CODE receiveCommand(uint8_t *cmd, int *size);
ERROR_CODE sendData();


#endif /* TASKCOMM_H_ */
