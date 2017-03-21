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
 *  1. ��������С����
 */

// ���Ի�������С
#ifndef COMMUNICATION_MAX_LEN
#define COMMUNICATION_MAX_LEN	256
#endif

// ѭ����������С
#ifndef CIRCLE_BUFFER_SIZE
#define CIRCLE_BUFFER_SIZE		128
#endif


//============================================================================
//      �ڲ���������
//============================================================================
#define USE_SCIA        		0   		// ʹ�� SCIA
// ���Զ˿ڣ�ok.
//#define USE_GPIO28_AS_SCIRXDA 	1			// RX - GPIO28
//#define USE_GPIO29_AS_SCITXDA 	1			// TX - GPIO29
#define SCIA_BAUD 				115200	 	    // SCIA BAUD = 115200

#define USE_SPIA 		1
#if( !TEST_BORD )
#define USE_GPIO19_AS_SPISTEA    1 			 // jzs �İ��Ӹ���ע�͵�
#else
#define USE_GPIO57_AS_SPISTEA    1
#endif
#define USE_GPIO54_AS_SPISIMOA   1      // func:2
#define USE_GPIO55_AS_SPISOMIA   1      // func:2
#define USE_GPIO56_AS_SPICLKA    1      // func:2

// �޸����Լ���FIFO����, ע�͵�ʹ��Ĭ�ϳ���16.
#define SPIA_SWFFTXDEEP CIRCLE_BUFFER_SIZE		// �������FIFO��TX�ռ��С
#define SPIA_SWFFRXDEEP CIRCLE_BUFFER_SIZE		// �������FIFO��RX�ռ��С


#endif /* SYSDEFINES_H_ */
