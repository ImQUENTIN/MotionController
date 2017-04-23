/*
 * myEXIO.h
 *
 *  Created on: 2017-4-20
 *      Author: QIYUEXIN
 *  Contact me: qiyuexin@yeah.net
 */

#ifndef MYEXIO_H_
#define MYEXIO_H_

/*  External Input */
#define M_EXI1 GpioDataRegs.GPADAT.bit.GPIO2
#define M_EXI2 GpioDataRegs.GPADAT.bit.GPIO3
#define M_EXI3 GpioDataRegs.GPADAT.bit.GPIO4
#define M_EXI4 GpioDataRegs.GPADAT.bit.GPIO5
#define M_EXI5 GpioDataRegs.GPADAT.bit.GPIO6
#define M_EXI6 GpioDataRegs.GPADAT.bit.GPIO7
#define M_EXI7 GpioDataRegs.GPADAT.bit.GPIO8
#define M_EXI8 GpioDataRegs.GPADAT.bit.GPIO9

/*  External Output */
#define M_EXO1 GpioDataRegs.GPADAT.bit.GPIO10
#define M_EXO2 GpioDataRegs.GPADAT.bit.GPIO11
#define M_EXO3 GpioDataRegs.GPADAT.bit.GPIO12
#define M_EXO4 GpioDataRegs.GPADAT.bit.GPIO13
#define M_EXO5 GpioDataRegs.GPADAT.bit.GPIO14
#define M_EXO6 GpioDataRegs.GPADAT.bit.GPIO15
#define M_EXO7 GpioDataRegs.GPADAT.bit.GPIO16
#define M_EXO8 GpioDataRegs.GPADAT.bit.GPIO17

/*  Test Jog keypad */
#define M_JXP	GpioDataRegs.GPADAT.bit.GPIO2
#define M_JXN	GpioDataRegs.GPADAT.bit.GPIO4
#define M_JYP	GpioDataRegs.GPADAT.bit.GPIO3
#define M_JYN	GpioDataRegs.GPADAT.bit.GPIO5
#define M_JZP	GpioDataRegs.GPADAT.bit.GPIO7
#define M_JZN	GpioDataRegs.GPADAT.bit.GPIO6

void InitEXIO(void);
void testJog(int axis);
void EnterJogMode(int axis);


#endif /* MYEXIO_H_ */
