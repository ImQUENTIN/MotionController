/*
 * config.h
 *
 *  Created on: 2017-2-12
 *      Author: QIYUEXIN
 *  Contact me: qiyuexin@yeah.net
 */

#ifndef CONFIG_H_
#define CONFIG_H_


//#define TEST_BORD		1		// 金子舒用的开放板，该行不能注释掉！！


//============================================================================
//      运动卡相关配置
//============================================================================
#define AXISNUM   8

/*
 *  1. 缓存区大小定义
 */

// 线性缓冲区大小
#ifndef COMMUNICATION_MAX_LEN
#define COMMUNICATION_MAX_LEN	256
#endif

// 循环缓冲区大小
#ifndef CIRCLE_BUFFER_SIZE
#define CIRCLE_BUFFER_SIZE		128
#endif














//============================================================================
//      内部外设配置
//============================================================================
#define USE_SCIA        		1   		// 使用 SCIA
// 测试端口，ok.
//#define USE_GPIO28_AS_SCIRXDA 	1			// RX - GPIO28
//#define USE_GPIO29_AS_SCITXDA 	1			// TX - GPIO29
#define SCIA_BAUD 				115200	 	    // SCIA BAUD = 115200

#define USE_SPIA 		1
#if( !TEST_BORD )
#define USE_GPIO19_AS_SPISTEA    1 			 // jzs 的板子该行注释掉
#else
#define USE_GPIO57_AS_SPISTEA    1
#endif
#define USE_GPIO54_AS_SPISIMOA   1      // func:2
#define USE_GPIO55_AS_SPISOMIA   1      // func:2
#define USE_GPIO56_AS_SPICLKA    1      // func:2

// 修改你自己的FIFO长度, 注释掉使用默认长度16.
#define SPIA_SWFFTXDEEP 32						// 设置软件FIFO的TX空间大小
#define SPIA_SWFFRXDEEP CIRCLE_BUFFER_SIZE		// 设置软件FIFO的RX空间大小


#endif /* CONFIG_H_ */
