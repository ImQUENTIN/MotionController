/*
 * DA.h
 *
 *  Created on: 2017-3-31
 *      Author: QIYUEXIN
 *  Contact me: qiyuexin@yeah.net
 */

#ifndef MY_DA_H_
#define MY_DA_H_

//---------------------------------------------------------------------------
// Da Register File:
//
struct AD5754_REGS_BITS{
	uint16_t data:16;
	uint16_t addr:3;
	uint16_t reg:3;
	uint16_t z:1;
	uint16_t rw:1;
};
union AD5754_REGS{
	struct AD5754_REGS_BITS bit;
	uint32_t all:24;
};
struct MYDA_REGS_BITS{
	union AD5754_REGS DAC;  // bit 23-0
	uint16_t we:1;		// bit 24,
	uint16_t load:1;	// bit 25,
	uint16_t busy:1;	// bit 25,
	uint16_t rsvd:5;

  };

union MYDA_REGS{
	struct MYDA_REGS_BITS bit;
	uint32_t all;
};
//union MYDA_REGS   MYDA;

extern volatile union MYDA_REGS myDaRegs;
void testMyDAC();
#endif /* DA_H_ */
