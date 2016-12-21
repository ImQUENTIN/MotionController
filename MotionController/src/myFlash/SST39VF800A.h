/*
 * SST39VF800A.h
 *
 *  Created on: 2016-12-12
 *      Author: Administrator
 */

#ifndef SST39VF800A_H_
#define SST39VF800A_H_

#define true 	1
#define false 	0

struct FlashSST39_VARS {
	uint16_t MakerId;
	uint16_t DeviceId;

	uint16_t Active:1;
	uint16_t Busy:1;
	uint16_t BlockSize:9;
	uint16_t SectorSize:5;
};

extern volatile struct FlashSST39_VARS flashSST39;
void FlashSST39_Init(void);
void FlashSST39_Reset(void);		// Software ID Exit7/CFI Exit
void FlashSST39_ReadId(void);		// Software ID Entry
void FlashSST39_ChipErase(void);	// Chip-Erase
void FlashSST39_SectorErase( uint16_t SectorNum);	// Sector-Erase
void FlashSST39_BlockErase( uint16_t BlockNum);		// Block-Erase
void FlashSST39_Write(volatile uint16_t * address, uint16_t val);	// Word-Program
void FlashSST39_Wait(volatile uint16_t * address);
void FlashSST39_Test(void);





extern volatile uint16_t EXTFLASH[0x8000];	// Flash ÄÚ´æÓ³Éä




#endif /* SST39VF800A_H_ */
