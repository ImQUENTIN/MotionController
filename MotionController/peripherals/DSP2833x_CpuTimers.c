// TI File $Revision: /main/3 $
// Checkin $Date: March 16, 2007   08:37:30 $
//###########################################################################
//
// FILE:	DSP2833x_CpuTimers.c
//
// TITLE:	CPU 32-bit Timers Initialization & Support Functions.
//
// NOTES:   CpuTimer1 and CpuTimer2 are reserved for use with DSP BIOS and
//          other realtime operating systems.
//
//          Do not use these two timers in your application if you ever plan
//          on integrating DSP-BIOS or another realtime OS.
//
//
//###########################################################################
// $TI Release: DSP2833x Header Files V1.01 $
// $Release Date: September 26, 2007 $
//###########################################################################

#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File


#if(USE_CPU_TIMER0)
struct CPUTIMER_VARS CpuTimer0;
#endif
// CpuTimer 1 and CpuTimer2 are used by DSP BIOS & other RTOS. Comment out if using DSP BIOS or other RTOS.
#if(USE_CPU_TIMER1)
struct CPUTIMER_VARS CpuTimer1;
#endif
#if(USE_CPU_TIMER2)
struct CPUTIMER_VARS CpuTimer2;
#endif


//---------------------------------------------------------------------------
// InitCpuTimers:
//---------------------------------------------------------------------------
// This function initializes all three CPU timers to a known state.
//
void InitCpuTimer0(void)
{

	// CPU Timer 0
#if(USE_CPU_TIMER0)
	// Initialize address pointers to respective timer registers:
	CpuTimer0.RegsAddr = &CpuTimer0Regs;
	// Reset interrupt counters:
	CpuTimer0.InterruptCount = 0;

	// 配置Timer参数：Id，Frequency，time
	ConfigCpuTimer(&CpuTimer0, SYSCLK_FRQ_MHZ, CPU_TIMER0_PRD);  	// 150Mhz，300ms


//#if (USE_DMA_CH1)
//	CpuTimer0.RegsAddr->TCR.bit.TRB = 0;      // 1 = reload timer
//#endif

	// 使用定时器中断
	//	CpuTimer0Regs.TCR.bit.TIE	= 	1;      // 1. 外设中断打开。 execute @ function:ConfigCpuTimer();
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;  		// 2. PIE中断打开。Timer0的中断在PIE的第1组，第7个,使能它。
	IER |= M_INT1;  	                		// 3. CPU核中断打开。Timer0是第一组。
	//										    // 4. 开启总中断，INTM。在最后统一打开，这里不开启。

	// 	修改中断向量表：
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.TINT0 = &cpu_timer0_isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers

	// 开启定时器
	StartCpuTimer0();
#endif
}

// CpuTimer 1 and CpuTimer2 are reserved for DSP BIOS & other RTOS
// Do not use these two timers if you ever plan on integrating
// DSP-BIOS or another realtime OS.
//
void InitCpuTimer1(void)
{

#if(USE_CPU_TIMER1)
	// Initialize address pointers to respective timer registers:
		CpuTimer1.RegsAddr = &CpuTimer1Regs;
		// Reset interrupt counters:
		CpuTimer1.InterruptCount = 0;

ConfigCpuTimer(&CpuTimer1, SYSCLK_FRQ_MHZ, CPU_TIMER1_PRD);	// 150Mhz，300ms
//  CpuTimer1Regs.TCR.bit.TIE   =   1;      // 1. 外设中断打开。execute @ function:ConfigCpuTimer();
//										    // 2. Timer1和Timer2不使用PIE,直接送入CPU核。
IER |= M_INT13;						        // 3. CPU核中断打开。Timer1是第13组。
//										    // 4. 开启总中断，INTM。在最后统一打开，这里不开启。

// 	修改中断向量表：
EALLOW;  // This is needed to write to EALLOW protected registers
PieVectTable.XINT13 = &cpu_timer1_isr;
EDIS;    // This is needed to disable write to EALLOW protected registers

// 开启定时器
StartCpuTimer1();
#endif
}

void InitCpuTimer2(void)
{

#if(USE_CPU_TIMER2)
// Initialize address pointers to respective timer registers:
CpuTimer2.RegsAddr = &CpuTimer2Regs;
// Reset interrupt counters:
CpuTimer2.InterruptCount = 0;
/* Cpu Timer 2 预留给DSP的实时操作系统BIOS，不使用BIOS可以用。 */
ConfigCpuTimer(&CpuTimer2, SYSCLK_FRQ_MHZ, CPU_TIMER2_PRD);	//150Mhz，300ms
//  CpuTimer2Regs.TCR.bit.TIE   =   1;	    // 1. 外设中断打开。execute @ function:ConfigCpuTimer();
//										    // 2. Timer1和Timer2不使用PIE,直接送入CPU核。
IER |= M_INT14;							    // 3. CPU核中断打开。Timer2是第14组。
//										    // 4. 开启总中断，INTM。在最后统一打开，这里不开启。
// 	修改中断向量表：
EALLOW;  // This is needed to write to EALLOW protected registers
PieVectTable.TINT2 = &cpu_timer2_isr;
EDIS;    // This is needed to disable write to EALLOW protected registers

// 开启定时器
StartCpuTimer2();
#endif

}

void InitCpuTimers(void){
	InitCpuTimer0();
	InitCpuTimer1();
	InitCpuTimer2();
}
//---------------------------------------------------------------------------
// ConfigCpuTimer:
//---------------------------------------------------------------------------
// This function initializes the selected timer to the period specified
// by the "Freq" and "Period" parameters. The "Freq" is entered as "MHz"
// and the period in "uSeconds". The timer is held in the stopped state
// after configuration.
//
void ConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period)
{
	Uint32 	temp;

	// Initialize timer period:
	Timer->CPUFreqInMHz = Freq;
	Timer->PeriodInUSec = Period;
	temp = (long) (Freq * Period);
	Timer->RegsAddr->PRD.all = temp;

	// Set pre-scale counter to divide by 1 (SYSCLKOUT):
	Timer->RegsAddr->TPR.all  = 0;
	Timer->RegsAddr->TPRH.all  = 0;

	// Initialize timer control register:
	Timer->RegsAddr->TCR.bit.TSS = 1;      // 1 = Stop timer, 0 = Start/Restart Timer
	Timer->RegsAddr->TCR.bit.TRB = 1;      // 1 = reload timer
	Timer->RegsAddr->TCR.bit.SOFT = 0;	   // 0： Not allow to free run even if halted
	Timer->RegsAddr->TCR.bit.FREE = 0;     // Timer Free Run Disabled
	Timer->RegsAddr->TCR.bit.TIE = 1;      // 0 = Disable/ 1 = Enable Timer Interrupt

	// Reset interrupt counter:
	Timer->InterruptCount = 0;

}

//===========================================================================
// End of file.
//===========================================================================
