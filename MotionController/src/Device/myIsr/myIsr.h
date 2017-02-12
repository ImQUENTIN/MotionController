 /*
 *  Created on: 2016-12-2
 *      Author: QUENTIN
 *      E-mail: qiyuexin@yeah.net
 */
//###########################################################################
//
//  FILE:   myIsr.h
//
// TITLE:   我的中断服务函数 函数声明
//
//###########################################################################


#ifndef MYISR_H_
#define MYISR_H_

#ifdef __cplusplus
extern "C" {
#endif

/* cpu timer  */
interrupt void cpu_timer0_isr(void);        // cpu timer0
interrupt void cpu_timer1_isr(void);        // cpu timer1
interrupt void cpu_timer2_isr(void);        // cpu timer2
interrupt void scia_rx_isr(void);			// scia rx
interrupt void scia_tx_isr(void);			// scia tx
interrupt void scib_rx_isr(void);			// scib rx
interrupt void scib_tx_isr(void);			// scib tx
interrupt void scic_rx_isr(void);			// scic rx
interrupt void scic_tx_isr(void);			// scic tx
interrupt void spia_rx_isr(void);			// spia rx
interrupt void spia_tx_isr(void);			// spia tx
interrupt void dma_ch1_isr(void);			// dma ch1
interrupt void dma_ch2_isr(void);			// dma ch1
interrupt void dma_ch3_isr(void);			// dma ch1
interrupt void dma_ch4_isr(void);			// dma ch1
interrupt void dma_ch5_isr(void);			// dma ch1
interrupt void dma_ch6_isr(void);			// dma ch1


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* MYISR_H_ */
