/*
*  Created on: 2016-12-12
*      Author: QUENTIN
*      E-mail: qiyuexin@yeah.net
*/
//###########################################################################
//
//  FILE:   SST39VF800A.c
//
// TITLE:   外设的初始化以及功能实现
//
//###########################################################################


#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File


#pragma DATA_SECTION(EXTFLASH,"EXTFLASH_DATA");
volatile uint16_t EXTFLASH[0x8000];		//外部Flash的映射地址

volatile struct FlashSST39_VARS flashSST39;


void FlashSST39_Init(void)
{
  flashSST39.Active = false;
  flashSST39.Busy = false;
  flashSST39.MakerId = 0;
  flashSST39.DeviceId = 0;
  FlashSST39_Reset();
  FlashSST39_ReadId();
//  FlashSST39_Test();
}

// Software ID Exit7/CFI Exit
void FlashSST39_Reset(void)
{
flashSST39.Busy = true;
  EXTFLASH[0x5555] = 0xaa;
  EXTFLASH[0x2AAA] = 0x55;
  EXTFLASH[0x5555] = 0xf0;
  FlashSST39_Wait(&EXTFLASH[0x5555]);
  flashSST39.Busy = false;
}

// Software ID Entry
void FlashSST39_ReadId(void)
{
	flashSST39.Busy = true;
  EXTFLASH[0x5555] = 0xaa;
  EXTFLASH[0x2AAA] = 0x55;
  EXTFLASH[0x5555] = 0x90;
  FlashSST39_Wait(&EXTFLASH[0x5555]);
  flashSST39.MakerId = EXTFLASH[0x0000] & 0xff;
  flashSST39.DeviceId = EXTFLASH[0x0001] & 0xff;

//  With AMS-A1 =0; SST Manufacturer’s ID= 00BFH, is read with A0 = 0,
//  SST39LF/VF200A Device ID = 2789H, is read with A0 = 1.
//  SST39LF/VF400A Device ID = 2780H, is read with A0 = 1.
//  SST39LF/VF800A Device ID = 2781H, is read with A0 = 1
//  SST39VF800A MakerId=0xBF DeviceId=0x81
  flashSST39.Active = (flashSST39.MakerId == 0xBF) && (flashSST39.DeviceId == 0x81);
  FlashSST39_Reset();
  flashSST39.Busy = false;
}



// Chip-Erase
void FlashSST39_ChipErase(void)
{
	flashSST39.Busy = true;
  EXTFLASH[0x5555] = 0xaa;
  EXTFLASH[0x2AAA] = 0x55;
  EXTFLASH[0x5555] = 0x80;
  EXTFLASH[0x5555] = 0xaa;
  EXTFLASH[0x2AAA] = 0x55;
  EXTFLASH[0x5555] = 0x10;
  FlashSST39_Wait(&EXTFLASH[0x5555]);
  flashSST39.Busy = false;
}

// Sector-Erase
void FlashSST39_SectorErase( uint16_t SectorNum)
{
	flashSST39.Busy = true;
  EXTFLASH[0x5555] = 0xaa;
  EXTFLASH[0x2AAA] = 0x55;
  EXTFLASH[0x5555] = 0x80;
  EXTFLASH[0x5555] = 0xaa;
  EXTFLASH[0x2AAA] = 0x55;
  EXTFLASH[flashSST39.SectorSize * SectorNum] = 0x30;
  FlashSST39_Wait(&EXTFLASH[0x5555]);
  flashSST39.Busy = false;
}


// Block-Erase
void FlashSST39_BlockErase( uint16_t BlockNum)
{
	flashSST39.Busy = true;
  EXTFLASH[0x5555] = 0xaa;
  EXTFLASH[0x2AAA] = 0x55;
  EXTFLASH[0x5555] = 0x80;
  EXTFLASH[0x5555] = 0xaa;
  EXTFLASH[0x2AAA] = 0x55;
  EXTFLASH[flashSST39.BlockSize * BlockNum] = 0x50;
  FlashSST39_Wait(&EXTFLASH[0x5555]);
  flashSST39.Busy = false;
}

// Word-Program
void FlashSST39_Write(volatile uint16_t * address, uint16_t val)
{
  flashSST39.Busy = true;
  EXTFLASH[0x5555] = 0xaa;
  EXTFLASH[0x2AAA] = 0x55;
  EXTFLASH[0x5555] = 0xa0;
  *address = val;
  FlashSST39_Wait(address);
  flashSST39.Busy = false;
}


void FlashSST39_Wait(volatile uint16_t * address)
{
uint16_t temp, val;
flashSST39.Busy = true;
  do
  {
	DELAY_US(1);
    val = *address;
    DELAY_US(1);
    temp = *address;
  }
  while(((val ^ temp) & BIT6) != 0);
  flashSST39.Busy = false;
}

void FlashSST39_Test(void)
{

	flashSST39.Busy = true;
  if (flashSST39.Active)//SST39VF800A测试正常
  {
    FlashSST39_Reset();
    FlashSST39_ChipErase();
    FlashSST39_Reset();
    FlashSST39_Write(&EXTFLASH[0x0000], 0x1234);
    FlashSST39_Write(&EXTFLASH[0x5555], 0x1234);
    FlashSST39_Write(&EXTFLASH[0x2aaa], 0xabcd);
    FlashSST39_Reset();
    FlashSST39_SectorErase(0);
    FlashSST39_Reset();
    FlashSST39_Write(&EXTFLASH[0x0000], 0x8888);
    FlashSST39_Reset();
    FlashSST39_BlockErase(0);
    FlashSST39_Write(&EXTFLASH[0x0000], 0x6666);
    FlashSST39_Reset();
  }
	  flashSST39.Busy = false;
}
