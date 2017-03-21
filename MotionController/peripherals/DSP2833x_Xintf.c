// TI File $Revision: /main/5 $
// Checkin $Date: August 16, 2007   11:06:26 $
//###########################################################################
//
// FILE:   DSP2833x_Xintf.c
//
// TITLE:   DSP2833x Device External Interface Init & Support Functions.
//
// DESCRIPTION:
//
//          Example initialization function for the external interface (XINTF).
//          This example configures the XINTF to its default state.  For an
//          example of how this function being used refer to the
//          examples/run_from_xintf project.
//
//###########################################################################
// $TI Release: DSP2833x Header Files V1.01 $
// $Release Date: September 26, 2007 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File



//---------------------------------------------------------------------------
// InitXINTF:
//---------------------------------------------------------------------------
// This function initializes the External Interface the default reset state.
//
// Do not modify the timings of the XINTF while running from the XINTF.  Doing
// so can yield unpredictable results


void ConfigureXTIMING( volatile union XTIMING_REG *zone, struct XTIMING_BITS *cfg )
{
	   // Zone 6------------------------------------
	    // When using ready, ACTIVE must be 1 or greater
	    // Lead must always be 1 or greater
// ----------------------------------------------------------------------------------------------------
//		Description 					X2TIMING = 0 					X2TIMING = 1
// ----------------------------------------------------------------------------------------------------
//	LR 	Lead period, read access 		XRDLEAD x tc(xtim) 				(XRDLEADx2) x tc(xtim)
//	AR 	Active period, read access 		(XRDACTIVE+WS+1) x tc(xtim) 	(XRDACTIVEx2+WS+1) x tc(xtim)
//	TR 	Trail period, read access 		XRDTRAIL x tc(xtim) 			(XRDTRAILx2) x tc(xtim)
//	LW Lead period, write access
//	AW Active period, write access
//	TW Trail period, write access
// ----------------------------------------------------------------------------------------------------
//
//	(1) tc(xtim) - Cycle time, XTIMCLK
//	(2) WS refers to the number of wait states inserted by hardware when using XREADY. If the zone is configured to ignore XREADY
//	(USEREADY= 0) then WS = 0.
		EALLOW;
		// Zone write timing
		zone->bit.XWRLEAD 	= cfg->XWRLEAD;
	    zone->bit.XWRACTIVE = cfg->XWRACTIVE;
	    zone->bit.XWRTRAIL 	= cfg->XWRTRAIL;
	    // Zone read timing
	    zone->bit.XRDLEAD 	= cfg->XRDLEAD;
	    zone->bit.XRDACTIVE = cfg->XRDACTIVE;
	    zone->bit.XRDTRAIL 	= cfg->XRDTRAIL;

	    // double all Zone read/write lead/active/trail timing
	    zone->bit.X2TIMING = cfg->X2TIMING;

	    // Zone will sample XREADY signal
	    zone->bit.USEREADY = cfg->USEREADY;
	    zone->bit.READYMODE = cfg->READYMODE;

	    // Size must be either:
	    // 0,1 = x32 or
	    // 1,1 = x16 other values are reserved
	    zone->bit.XSIZE = cfg->XSIZE;
	    EDIS;
}
void InitZone7(void)
{
#if(USE_XINTF_ZONE7)
	struct XTIMING_BITS cfg;
    // Zone 7------------------------------------
	// 杩炴帴鐨勬槸FLASH, SST39VF800A-70, 鍗�0ns
#if(SYSCLKOUT_MHZ == 150)
	//	F28335锛孲YSCLK=150Mhz -> 6.7ns
    // Zone write timing
	cfg.XWRLEAD 	= 1;	// one XTIMCLK cycle.
    cfg.XWRACTIVE 	= 1;	// two XTIMCLK cycle.
    cfg.XWRTRAIL 	= 0;
    // Zone read timing
    cfg.XRDLEAD 	= 1;
    cfg.XRDACTIVE 	= 1;
    cfg.XRDTRAIL 	= 0;

    // double all Zone read/write lead/active/trail timing
    cfg.X2TIMING 	= 0;

    // Zone will sample XREADY signal
    cfg.USEREADY 	= 0;
    cfg.READYMODE 	= 1;  // sample asynchronous

    // Size must be either:
    // 0,1 = x32 or
    // 1,1 = x16 other values are reserved
    cfg.XSIZE 		= 3;

#elif(SYSCLKOUT_MHZ == 90)
	// SYSCLK=90Mhz -> 11.11ns
    // Zone write timing
	cfg.XWRLEAD 	= 1;	// one XTIMCLK cycle.
    cfg.XWRACTIVE 	= 2;	// two XTIMCLK cycle.
    cfg.XWRTRAIL 	= 0;
    // Zone read timing
    cfg.XRDLEAD 	= 1;
    cfg.XRDACTIVE 	= 2;
    cfg.XRDTRAIL 	= 0;

    // double all Zone read/write lead/active/trail timing
    cfg.X2TIMING 	= 0;

    // Zone will sample XREADY signal
    cfg.USEREADY 	= 0;
    cfg.READYMODE 	= 1;  // sample asynchronous

    // Size must be either:
    // 0,1 = x32 or
    // 1,1 = x16 other values are reserved
    cfg.XSIZE 		= 3;
#endif
    ConfigureXTIMING(&XintfRegs.XTIMING7, &cfg);

    // Bank switching
    // Assume Zone 7 is slow, so add additional BCYC cycles
    // when ever switching from Zone 6 to another Zone.
    // This will help avoid bus contention.
//    EALLOW;
//    XintfRegs.XBANK.bit.BANK = 7;
//    XintfRegs.XBANK.bit.BCYC = 7;
//    EDIS;
   //Force a pipeline flush to ensure that the write to
   //the last register configured occurs before returning.
#endif
}
void InitZone6(void)
{
#if(USE_XINTF_ZONE6)
	struct XTIMING_BITS cfg;
    // Zone 6------------------------------------
#if(SYSCLKOUT_MHZ == 150)
	// 寮�彂鏉跨殑zone7杩炵殑鏄疪AM鑺墖锛欼S61LV256(512)16-10, 10ns
	// 寮�彂鏉跨殑鐢ㄧ殑F28335锛孲YSCLK=150Mhz -> 6.7ns
    // Zone write timing
	cfg.XWRLEAD 	= 1;	// one XTIMCLK cycle.
    cfg.XWRACTIVE 	= 7;	// two XTIMCLK cycle.
    cfg.XWRTRAIL 	= 1;
    // Zone read timing
    cfg.XRDLEAD 	= 1;
    cfg.XRDACTIVE 	= 7;
    cfg.XRDTRAIL 	= 1;

    // double all Zone read/write lead/active/trail timing
    cfg.X2TIMING 	= 1;

    // Zone will sample XREADY signal
    cfg.USEREADY 	= 0;
    cfg.READYMODE 	= 1;  // sample asynchronous

    // Size must be either:
    // 0,1 = x32 or
    // 1,1 = x16 other values are reserved
    cfg.XSIZE 		= 3;
#elif(SYSCLKOUT_MHZ == 90)
	// SYSCLK=90Mhz -> 11.11ns
    // Zone write timing
	cfg.XWRLEAD 	= 1;	// one XTIMCLK cycle.
    cfg.XWRACTIVE 	= 1;	// two XTIMCLK cycle.
    cfg.XWRTRAIL 	= 0;
    // Zone read timing
    cfg.XRDLEAD 	= 1;
    cfg.XRDACTIVE 	= 1;
    cfg.XRDTRAIL 	= 0;

    // double all Zone read/write lead/active/trail timing
    cfg.X2TIMING 	= 0;

    // Zone will sample XREADY signal
    cfg.USEREADY 	= 0;
    cfg.READYMODE 	= 1;  // sample asynchronous

    // Size must be either:
    // 0,1 = x32 or
    // 1,1 = x16 other values are reserved
    cfg.XSIZE 		= 3;
#endif

    // Zone 6------------------------------------
        ConfigureXTIMING(&XintfRegs.XTIMING6, &cfg);
        

    // Bank switching
    // Assume Zone 6 is slow, so add additional BCYC cycles
    // when ever switching from Zone 6 to another Zone.
    // This will help avoid bus contention.
//        EALLOW;
//    XintfRegs.XBANK.bit.BANK = 6;
//    XintfRegs.XBANK.bit.BCYC = 6;
//        EDIS;
#endif
}


void InitZone0(void)
{
#if(USE_XINTF_ZONE0)
	struct XTIMING_BITS cfg;
    // Zone 0------------------------------------
#if(SYSCLKOUT_MHZ == 150)
	// 寮�彂鏉跨殑zone7杩炵殑鏄疪AM鑺墖锛欼S01LV256(512)16-10, 10ns
	// 寮�彂鏉跨殑鐢ㄧ殑F28335锛孲YSCLK=150Mhz -> 6.7ns
    // Zone write timing
	cfg.XWRLEAD 	= 1;	// one XTIMCLK cycle.
    cfg.XWRACTIVE 	= 7;	// two XTIMCLK cycle.
    cfg.XWRTRAIL 	= 1;
    // Zone read timing
    cfg.XRDLEAD 	= 1;
    cfg.XRDACTIVE 	= 7;
    cfg.XRDTRAIL 	= 1;

    // double all Zone read/write lead/active/trail timing
    cfg.X2TIMING 	= 1;

    // Zone will sample XREADY signal
    cfg.USEREADY 	= 0;
    cfg.READYMODE 	= 1;  // sample asynchronous

    // Size must be either:
    // 0,1 = x32 or
    // 1,1 = x16 other values are reserved
    cfg.XSIZE 		= 3;
#elif(SYSCLKOUT_MHZ == 90)
	// SYSCLK=90Mhz -> 11.11ns
    // Zone write timing
	cfg.XWRLEAD 	= 1;	// one XTIMCLK cycle.
    cfg.XWRACTIVE 	= 2;	// two XTIMCLK cycle.
    cfg.XWRTRAIL 	= 0;
    // Zone read timing
    cfg.XRDLEAD 	= 1;
    cfg.XRDACTIVE 	= 2;
    cfg.XRDTRAIL 	= 0;

    // double all Zone read/write lead/active/trail timing
    cfg.X2TIMING 	= 0;

    // Zone will sample XREADY signal
    cfg.USEREADY 	= 0;
    cfg.READYMODE 	= 1;  // sample asynchronous

    // Size must be either:
    // 0,1 = x32 or
    // 1,1 = x16 other values are reserved
    cfg.XSIZE 		= 3;
#endif
    
    // Zone 0------------------------------------
    ConfigureXTIMING(&XintfRegs.XTIMING0, &cfg);


    // Bank switching
    // Assume Zone 0 is slow, so add additional BCYC cycles
    // when ever switching from Zone 0 to another Zone.
    // This will help avoid bus contention.
//        EALLOW;
//    XintfRegs.XBANK.bit.BANK = 0;
//    XintfRegs.XBANK.bit.BCYC = 0;
//        EDIS;
#endif
}



void InitXintf(void)
{
	// -------------------------------------------------------
    // Step 2. Make sure the XINTF clock is enabled
    //
	EALLOW;
	SysCtrlRegs.PCLKCR3.bit.XINTFENCLK = 1;

	// ------------------------------------------------------
    // Step 3. Configure the XTIMCLK and XCLKOUT
	//
//	XTIMCLK:
//	000 XTIMCLK = SYSCLKOUT/1
//	001 XTIMCLK = SYSCLKOUT/2 (default)
    XintfRegs.XINTCNF2.bit.XTIMCLK = 0;    // 璁剧疆鎵�湁澶栭儴鍖哄煙 XTIMCLK = SYSCLKOUT
    // 3涓啓缂撳啿
    XintfRegs.XINTCNF2.bit.WRBUFF = 3;
    // XCLKOUT琚娇鑳�
    XintfRegs.XINTCNF2.bit.CLKOFF = 0;
//    CLKMODE:
//    0 XCLKOUT is equal to XTIMCLK
//    1 XCLKOUT is a divide by 2 of XTIMCLK (default)
    XintfRegs.XINTCNF2.bit.CLKMODE = 0;       // XCLKOUT = XTIMCLK
	EDIS;

	// ------------------------------------------------------
    // Step 4. Initialize the specific zone.
	//
	InitZone0();		// 运动卡的FPGA用的zone0，
	InitZone6();		// 运动卡的EXTRAM用的zone6，
	InitZone7();		// EXTFLASH用的zone7， 不用XINTF。


   InitXintf16Gpio();
// InitXintf32Gpio();

   asm(" RPT #7 || NOP");

}

void InitXintf32Gpio()
{
     EALLOW;
     GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 3;  // XD31
     GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 3;  // XD30
     GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 3;  // XD29
     GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 3;  // XD28
     GpioCtrlRegs.GPBMUX2.bit.GPIO52 = 3;  // XD27
     GpioCtrlRegs.GPBMUX2.bit.GPIO53 = 3;  // XD26
     GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 3;  // XD25
     GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 3;  // XD24
     GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 3;  // XD23
     GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 3;  // XD22
     GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 3;  // XD21
     GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 3;  // XD20
     GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 3;  // XD19
     GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 3;  // XD18
     GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 3;  // XD17
     GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 3;  // XD16

     GpioCtrlRegs.GPBQSEL2.bit.GPIO48 = 3;  // XD31 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO49 = 3;  // XD30 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO50 = 3;  // XD29 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO51 = 3;  // XD28 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO52 = 3;  // XD27 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO53 = 3;  // XD26 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 3;  // XD25 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO55 = 3;  // XD24 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 3;  // XD23 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO57 = 3;  // XD22 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO58 = 3;  // XD21 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO59 = 3;  // XD20 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO60 = 3;  // XD19 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO61 = 3;  // XD18 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO62 = 3;  // XD17 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO63 = 3;  // XD16 asynchronous input


     InitXintf16Gpio();
}

void InitXintf16Gpio()
{
     EALLOW;
     GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 3;  // XD15
     GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 3;  // XD14
     GpioCtrlRegs.GPCMUX1.bit.GPIO66 = 3;  // XD13
     GpioCtrlRegs.GPCMUX1.bit.GPIO67 = 3;  // XD12
     GpioCtrlRegs.GPCMUX1.bit.GPIO68 = 3;  // XD11
     GpioCtrlRegs.GPCMUX1.bit.GPIO69 = 3;  // XD10
     GpioCtrlRegs.GPCMUX1.bit.GPIO70 = 3;  // XD19
     GpioCtrlRegs.GPCMUX1.bit.GPIO71 = 3;  // XD8
     GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 3;  // XD7
     GpioCtrlRegs.GPCMUX1.bit.GPIO73 = 3;  // XD6
     GpioCtrlRegs.GPCMUX1.bit.GPIO74 = 3;  // XD5
     GpioCtrlRegs.GPCMUX1.bit.GPIO75 = 3;  // XD4
     GpioCtrlRegs.GPCMUX1.bit.GPIO76 = 3;  // XD3
     GpioCtrlRegs.GPCMUX1.bit.GPIO77 = 3;  // XD2
     GpioCtrlRegs.GPCMUX1.bit.GPIO78 = 3;  // XD1
     GpioCtrlRegs.GPCMUX1.bit.GPIO79 = 3;  // XD0

     GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 3;  // XA0/XWE1n
     GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 3;  // XA1
     GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 3;  // XA2
     GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 3;  // XA3
     GpioCtrlRegs.GPBMUX1.bit.GPIO44 = 3;  // XA4
     GpioCtrlRegs.GPBMUX1.bit.GPIO45 = 3;  // XA5
     GpioCtrlRegs.GPBMUX1.bit.GPIO46 = 3;  // XA6
     GpioCtrlRegs.GPBMUX1.bit.GPIO47 = 3;  // XA7

     GpioCtrlRegs.GPCMUX2.bit.GPIO80 = 3;  // XA8
     GpioCtrlRegs.GPCMUX2.bit.GPIO81 = 3;  // XA9
     GpioCtrlRegs.GPCMUX2.bit.GPIO82 = 3;  // XA10
     GpioCtrlRegs.GPCMUX2.bit.GPIO83 = 3;  // XA11
     GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 3;  // XA12
     GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 3;  // XA13
     GpioCtrlRegs.GPCMUX2.bit.GPIO86 = 3;  // XA14
     GpioCtrlRegs.GPCMUX2.bit.GPIO87 = 3;  // XA15
     GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 3;  // XA16
     GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 3;  // XA17
     GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 3;  // XA18
     GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 3;  // XA19

     GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 3;  // XREADY
	 GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 3;  // XRNW
     GpioCtrlRegs.GPBMUX1.bit.GPIO38 = 3;  // XWE0

     GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 3;  // XZCS0
     GpioCtrlRegs.GPBMUX1.bit.GPIO37 = 3;  // XZCS7
     GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 3;  // XZCS6
     EDIS;
}


//
//        SRC/DST_ADDR    // The value written into the shadow register
//                        // is the start address of the first location where data is read or written to.
//
//        SRC/DST_BEG_ADDR// On a wrap condition, the active register will be incremented by the signed value in the
//                        // appropriate SRC/DST_WRAP_STEP register prior to being loaded into the active SRC/DST_ADDR register.
//
//        BURST_SIZE  	// 16 or 32, the smallest amount of data that can be transferred at one time
//                        // This specifies the number of words to be transferred in a burst.
//                        // This value is loaded into the BURST_COUNT register at the beginning of each burst.
//
//        TRANSFER_SIZE 	// How many bursts are performed in the entire transfer.
//                        // This specifies the number of bursts to be transferred before per CPU interrupt (if enabled).
//                        // This value is loaded into the TRANSFER_COUNT register at the beginning of each transfer.
//        // DATASIZE
//                        NOTE: The value written to the SIZE registers is one less than the intended size. So, to transfer
//                        three 16-bit words, the value 2 should be placed in the SIZE register.
//                        Regardless of the state of the DATASIZE bit, the value specified in the SIZE registers are for
//                        16-bit addresses. So, to transfer three 32-bit words, the value 5 should be placed in the SIZE
//                        register.
//
//        MODE.CHx[CONTINUOUS]
//        MODE.CHx[ONESHOT]   // DMA transfers one burst of data each time
//        MODE.CHx[CHINTMODE] // DMA interrupt
//                        Whether this interrupt is generated at the beginning or the end of the transfer is defined in the
//                        CHINTMODE bit in the MODE register. Whether the channel remains enabled or not after the
//                        transfer is completed is defined by the CONTINUOUS bit in the MODE register.
//
//        SRC/DST_WRAP_SIZE   // This specifies the number of bursts to be transferred before the current address pointer wraps around to the beginning.
//                            // To disable the wrap function, assign the value of these registers to be larger than the TRANSFER_SIZE.
//
//
//        For each source/destination pointer, the address changes can be controlled with the following step values:
//        SRC/DST_BURST_STEP // Within each burst transfer, the address source and destination step sizes are specified by these registers.
//                            This value is a signed 2's compliment number. If no increment is desired,
//                            such as when accessing the McBSP data, the value of these registers should be set to zero.
//
//
//        SRC/DST_TRANSFER_STEP   // This specifies the address offset to start the next burst transfer after completing the current burst transfer.
//
//        SRC/DST_WRAP_STEP       // When the wrap counter reaches zero, this value specifies the number of words to add/subtract
//                                // from the BEG_ADDR pointer and hence sets the new start address.
//
//
//
//        The appropriate active SRC/DST_BEG_ADDR register is incremented by the signed value contained
//           in the SRC/DST_WRAP_STEP register, then
//
//        鈥�The new active SRC/DST_BEG_ADDR register is loaded into the active SRC/DST_ADDR register.
//
//        SRC/DST_BEG_ADDR
//        SRC/DST_TRANSFER_STEP
        
//===========================================================================
// No more.
//===========================================================================
