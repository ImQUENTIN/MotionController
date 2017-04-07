 /*
 *  Created on: 2016-12-1
 *      Author: QUENTIN
 *      E-mail: qiyuexin@yeah.net
 */
//###########################################################################
//
// FILE:  main.c
//
//
// TITLE:   ��DSP�������������ʵ��
//
//###########################################################################
//
//    ������RAM�е��Ե���Ҫ�������Ŀ���ó�"boot to SARAM".2833x����ģʽ
//    ��������ʾ. ���õĻ���"boot to Flash"ģʽ����������RAM�������ƺ��
//    ���Խ������ս�Flash�в�ʹ��"boot to Flash"����ģʽ.
//
//       $Boot_Table:
//
//         GPIO87   GPIO86     GPIO85   GPIO84
//          XA15     XA14       XA13     XA12
//           PU       PU         PU       PU
//        ==========================================
//            1        1          1        1    Jump to Flash
//            1        1          1        0    SCI-A boot
//            1        1          0        1    SPI-A boot
//            1        1          0        0    I2C-A boot
//            1        0          1        1    eCAN-A boot
//            1        0          1        0    McBSP-A boot
//            1        0          0        1    Jump to XINTF x16
//            1        0          0        0    Jump to XINTF x32
//            0        1          1        1    Jump to OTP
//            0        1          1        0    Parallel GPIO I/O boot
//            0        1          0        1    Parallel XINTF boot
//            0        1          0        0    Jump to SARAM	    <- "boot to SARAM"
//            0        0          1        1    Branch to check boot mode
//            0        0          1        0    Boot to flash, bypass ADC cal
//            0        0          0        1    Boot to SARAM, bypass ADC cal
//            0        0          0        0    Boot to SCI-A, bypass ADC cal
//                                              Boot_Table_End$
//
// ��������:
//
//###########################################################################

#include "my_project.h"


// ��ARM������ָ�Ӳ�� --> taskComm --> taskPlan  --> taskExecute

void main(void)
{

/* 
	tmp = sizeof(char);			// 1 - 16bit
	tmp = sizeof(int);			// 1
	tmp = sizeof(short);		// 1
	tmp = sizeof(long);			// 2
	tmp = sizeof(long long);	// 4
	tmp = sizeof(float);		// 2
	tmp = sizeof(double);		// 2
 */
	ERROR_CODE rtn;

	// step 1: initial devices
	InitDevices();

//	FlashSST39_Test();
//	EXTRAM_test();
//	TestSpi();	// ע�͵�������Ҫ���Ե�ʱ��


	testPlot();
	testpt();  //����
	PT_Data();  //��ʱ�������
	testMyDAC();
//	testPlot();


//	while(1)
//		testMymotor();

	while(1)
     {
		rtn = checkNewCommand();
		if (rtn == RTN_SUCC) {
			taskPlan();
			// taskExecute();

		}

     }
}

//===========================================================================
// No more.
//===========================================================================
