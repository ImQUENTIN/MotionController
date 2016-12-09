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
//    ���������� CPU��ʱ��0Ϊ500ms����, ÿ�������ж�ȡ�� GPIO66
//    ȡ����LED����������֮��ת��. Ϊ�˱��ڹ۲죬������������һ����������CpuTimer0.InterruptCount
//    ÿ���жϼ�������+1��
//
//       Watch Variables:
//          CpuTimer0.InterruptCount
//
//       Monitor the GPIO32 LED blink on (for 500 msec) and off (for 500 msec) on the 2833x eZdsp.
//
//###########################################################################

#include "my_project.h"


//===========================================================================
// No more.
//===========================================================================

int main(void)
{
	InitPeripherals();	// as you see.
	ExecuteMyTask();
	return 0;
}

//===========================================================================
// No more.
//===========================================================================
