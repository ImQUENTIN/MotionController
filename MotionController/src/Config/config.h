/*
 * config.h
 *
 *  Created on: 2017-2-12
 *      Author: QIYUEXIN
 *  Contact me: qiyuexin@yeah.net
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define USE_SPIA 		1		// 使用SPIA

//#define TEST_BORD		1		// 金子舒用的开放板，该行不能注释掉！！


//============================================================================
//      运动卡相关配置
//============================================================================
#define AXISNUM   8

//----------------------------------------------------------------------------
//      速度规划的默认配置
//----------------------------------------------------------------------------
#define DEFUALT_MAX_RISE_ACC 	 32
#define DEFUALT_MAX_DOWN_ACC 	-32
#define DEFUALT_MAX_EVEN_VEL 	64000



#endif /* CONFIG_H_ */
