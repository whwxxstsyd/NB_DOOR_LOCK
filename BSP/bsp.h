/*
*********************************************************************************************************
*
*	模块名称 : BSP模块
*	文件名称 : bsp.h
*	说    明 : 这是底层驱动模块所有的h文件的汇总文件。 应用程序只需 #include bsp.h 即可，
*			  不需要#include 每个模块的 h 文件
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_H
#define _BSP_H

#include "include.h"


/* 开关全局中断的宏 */	
#define ENABLE_INT()	enableInterrupts();	/* 使能全局中断 */
#define DISABLE_INT()	disableInterrupts()	/* 禁止全局中断 */

/* 这个宏仅用于调试阶段排错 */
#define BSP_Printf		printf	
		
#define DEV_COM 	COM1	/*设备通信串口*/
#define NB_COM 		COM2	/*NB模块使用的串口*/	
#define DEBUG_COM 	COM3	/*调试串口*/						

#define SYSTEM_TICKS_PER_SEC	1000				
 
#ifndef TRUE	
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

/*
	EXTI9_5_IRQHandler 的中断服务程序分散在几个独立的 bsp文件中。
	需要整合到 stm32f4xx_it.c 中。

	定义下面行表示EXTI9_5_IRQHandler入口函数集中放到 stm32f4xx_it.c。
*/
#define EXTI9_5_ISR_MOVE_OUT

#define DEBUG_GPS_TO_COM1	/* 打印GPS数据到串口1 */

/* 通过取消注释或者添加注释的方式控制是否包含底层驱动模块 */
#include "bsp_uart_fifo.h"
#include "bsp_timer.h"
#include "bsp_led.h"	


/*自己的硬件相关*/
#include "gprs.h"			
#include "basic_fun.h"		
#include "shell.h"	

/* 提供给其他C文件调用的函数 */
void bsp_Init(void);
void bsp_Idle(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
