/*
 * testMyPeripherals.c
 *
 *  Created on: 2016-12-6
 *      Author: Administrator
 */
#include "my_project.h"

#if( MY_TEST_DEMO == TEST_SCIA || MY_TEST_DEMO == TEST_SCIB || MY_TEST_DEMO == TEST_SCIC )
void TestSci(void)
{

#if(USE_SCIA)
#define Scix					Scia
#define Scix_gets(a)			Scia_gets(a)
#define Scix_getchar(a) 		Scia_getchar(a)
#define Scix_puts(a)			Scia_puts(a)
#define Scix_puts(a)			Scia_puts(a)

#elif(USE_SCIB)
#define Scix					Scib
#define Scix_gets(a)			Scib_gets(a)
#define Scix_getchar(a) 		Scib_getchar(a)
#define Scix_puts(a)			Scib_puts(a)
#define Scix_puts(a)			Scib_puts(a)

#elif(USE_SCIC)
#define Scix					Scic
#define Scix_gets(a)			Scic_gets(a)
#define Scix_getchar(a) 		Scic_getchar(a)
#define Scix_puts(a)			Scic_puts(a)
#define Scix_puts(a)			Scic_puts(a)
#endif

#if( USE_SCIA || USE_SCIB || USE_SCIC )
#if(TEST_SCI_CHAR)

// ===============================================================================
// 下面测试Scix_putchar 及 Scix_getchar
// ===============================================================================
	static uint8_t receivedData=0;
	for(;;) {
//			receivedData[1] = 0;

		if(!Scix_getchar(&receivedData)){

			Scix_puts("\r\nHi,You enter :");
			Scix_putchar(receivedData);	// show me.
			//Scia.RegsAddr->SCIFFTX.bit.TXFFINTCLR = 1;	// clear INT flag.
			//while(!Sci_IsFifoTxEmpty(&Scia));	// 等待清空
		}


		}

#else
	// ===============================================================================
	// 下面测试Scix_puts 及 Scix_gets
	// ===============================================================================

	char tmp[100],overPos=0;

	overPos = Scix_puts("0123456789abcdef0123456789abcdef0123456789");
	if(overPos){
		// 溢出检测到
		while(!Sci_IsFifoTxEmpty(&Scix));	// 等待清空
		Scix_puts("\r\nswFIFO overflow detected.\r\n");
		while(!Sci_IsFifoTxEmpty(&Scix));	// 等待清空
		Scix_puts("overfolwed Info:'");
		while(!Sci_IsFifoTxEmpty(&Scix));	// 等待清空
		Scix_puts(overPos + "0123456789abcdef0123456789abcdef0123456789");
	}
	while(!Sci_IsFifoTxEmpty(&Scix));	// 等待清空
	Scix_puts("'\r\nThis is Sic a: hello,world.\r\n");

	for(;;) {
		DELAY_US(1000000);

		if( !Scix_gets(tmp)) {
			Scix_puts("\r\ngets:");
			overPos = Scix_puts(tmp);
			if(overPos){
				// 溢出检测到
				while(!Sci_IsFifoTxEmpty(&Scix));	// 等待清空
				Scix_puts(overPos + tmp);
			}
		} else {
			Scix_puts("\r\nno char dected.");
		}

	}
#endif
#endif

}

#endif // ( MY_TEST_DEMO == TEST_SCIA || MY_TEST_DEMO == TEST_SCIB || MY_TEST_DEMO == TEST_SCIC )


#if( MY_TEST_DEMO == TEST_SPIA && USE_SPIA )
void TestSpi(void)
{
	// 说明：绿色的运动控制卡 使用的GPIO19作为SPIA_STE
	//       所以在使用金子舒用的开发板进行调试时需要把“USE_GPIO19_AS_SPISTEA” 注释掉
	//       该宏定义在 my_demo_select.h @line 116

	int i = 17+SPIA_SWFFRXDEEP;
	static char tmp[17+SPIA_SWFFRXDEEP]={0};
	static char tx[] = "hello\n";

	for(;i>0;i--){
		tmp[i] = 0;
		if(SpiaRegs.SPIFFTX.bit.TXFFST < 15)
		{
			SpiaRegs.SPITXBUF = i;
		}
	}

	for(;;) {

		//DELAY_US(100000);
		//Spia_puts("nothing is received.\n");
//		if( !SpiaRegs.SPISTS.bit.BUFFULL_FLAG)
//			SpiaRegs.SPITXBUF = 0x17;

			//SpiaRegs.SPITXBUF = 0x13;
//		if( !Spia_gets(tmp)) {
//			i =0 ;
//			while(tmp[i++] != 0){
//				if( tmp[i-1] == '#'){
//					SpiaRegs.SPITXBUF = 'h';
//					SpiaRegs.SPITXBUF = 'e';
//					SpiaRegs.SPITXBUF = 'l';
//					SpiaRegs.SPITXBUF = 'l';
//					SpiaRegs.SPITXBUF = '0';
//					SpiaRegs.SPIDAT   = 0x11;
//				}
//			}
//			//ESTOP0;	// stop here.
//		}
	}
}
#endif //( MY_TEST_DEMO == TEST_SPIA )



#if( MY_TEST_DEMO == TEST_XINTF )
void TestXintf(void)
{
	extern volatile Uint16 EXTRAM[0x8000];		// 片外RAM

	EXTRAM[0] = (signed int)0x0009;
	EXTRAM[1] = 0x0007;
	EXTRAM[2] = 0x08;
	EXTRAM[3] = 0x09;

}

void EXTFPGA_Test(void)
{
	volatile int i,tmp;

	testMymotor();

}
#endif //( MY_TEST_DEMO == TEST_XINTF )












