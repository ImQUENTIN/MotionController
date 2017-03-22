/*
 * myram.h
 *
 *  Created on: 2017-3-17
 *      Author: QIYUEXIN
 *  Contact me: qiyuexin@yeah.net
 */

#ifndef MYRAM_H_
#define MYRAM_H_
extern CIRCLE_BUFFER_S ram_dda[AXISNUM];

void EXTRAM_init(void);
void EXTRAM_test(void);
void testPlot(void);

#endif /* MYRAM_H_ */
