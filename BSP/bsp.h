/*
*********************************************************************************************************
*
*	ģ������ : BSPģ��
*	�ļ����� : bsp.h
*	˵    �� : ���ǵײ�����ģ�����е�h�ļ��Ļ����ļ��� Ӧ�ó���ֻ�� #include bsp.h ���ɣ�
*			  ����Ҫ#include ÿ��ģ��� h �ļ�
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_H
#define _BSP_H

#include "include.h"


/* ����ȫ���жϵĺ� */	
#define ENABLE_INT()	enableInterrupts();	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	disableInterrupts()	/* ��ֹȫ���ж� */

/* ���������ڵ��Խ׶��Ŵ� */
#define BSP_Printf		printf	
		
#define DEV_COM 	COM1	/*�豸ͨ�Ŵ���*/
#define NB_COM 		COM2	/*NBģ��ʹ�õĴ���*/	
#define DEBUG_COM 	COM3	/*���Դ���*/						

#define SYSTEM_TICKS_PER_SEC	1000				

#ifndef TRUE	
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

	
#define CDP_IP_ADDRESS		"180.101.147.115"					//���Ų��� : "180.101.147.115"		ipb4659373:5683
#define CDP_IP_PORT			"5683"	



/*
	EXTI9_5_IRQHandler ���жϷ�������ɢ�ڼ��������� bsp�ļ��С�
	��Ҫ���ϵ� stm32f4xx_it.c �С�

	���������б�ʾEXTI9_5_IRQHandler��ں������зŵ� stm32f4xx_it.c��
*/
#define EXTI9_5_ISR_MOVE_OUT

#define DEBUG_GPS_TO_COM1	/* ��ӡGPS���ݵ�����1 */




/* ͨ��ȡ��ע�ͻ������ע�͵ķ�ʽ�����Ƿ�����ײ�����ģ�� */
#include "bsp_uart_fifo.h"
#include "bsp_timer.h"
#include "bsp_led.h"	


/*�Լ���Ӳ�����*/			
#include "basic_fun.h"		
#include "shell.h"	


/*NBģ����ص�*/
	

/* �ṩ������C�ļ����õĺ��� */
void bsp_Init(void);
void bsp_Idle(void);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
