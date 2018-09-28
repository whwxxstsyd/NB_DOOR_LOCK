/*
*********************************************************************************************************
*
*	ģ������ : �����ж�+FIFO����ģ��
*	�ļ����� : bsp_uart_fifo.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_USART_FIFO_H_
#define _BSP_USART_FIFO_H_


#define	UART1_FIFO_EN	0					
#define	UART2_FIFO_EN	1
#define	UART3_FIFO_EN	1			

/* RS485оƬ����ʹ��GPIO, PB2 */
#define PORT_RS485_TXEN  	GPIOC
#define PIN_RS485_TXEN	 	GPIO_Pin_4
	
#define RS485_RX_EN()	GPIO_ResetBits(PORT_RS485_TXEN,PIN_RS485_TXEN)	
#define RS485_TX_EN()	GPIO_SetBits(PORT_RS485_TXEN,PIN_RS485_TXEN)
	

/* ����˿ں� */
typedef enum
{
	COM1 = 0,	/* USART1  PA9, PA10 ��  PB6, PB7*/
	COM2 = 1,	/* USART2, PD5,PD6 �� PA2, PA3 */
	COM3 = 2,	/* USART3, PB10, PB11 */
}COM_PORT_E;
	

#define UART_BUF_SIZE_MAX 	512														


/* ���崮�ڲ����ʺ�FIFO��������С����Ϊ���ͻ������ͽ��ջ�����, ֧��ȫ˫�� */
/* ���崮�ڲ����ʺ�FIFO��������С����Ϊ���ͻ������ͽ��ջ�����, ֧��ȫ˫�� */
#if UART1_FIFO_EN == 1	
	#define UART1_BAUD			9600
	#define UART1_TX_BUF_SIZE	512		
	#define UART1_RX_BUF_SIZE	512		
#endif

#if UART2_FIFO_EN == 1
	#define UART2_BAUD			9600
	#define UART2_TX_BUF_SIZE	512			
	#define UART2_RX_BUF_SIZE	512		
#endif

#if UART3_FIFO_EN == 1			
	#define UART3_BAUD			9600	
	#define UART3_TX_BUF_SIZE	200
	#define UART3_RX_BUF_SIZE	200						
#endif



/* �����豸�ṹ�� */
typedef struct
{
	USART_TypeDef *uart;		/* STM32�ڲ������豸ָ�� */
	uint8_t *pTxBuf;			/* ���ͻ����� */
	uint8_t *pRxBuf;			/* ���ջ����� */
	uint16_t usTxBufSize;		/* ���ͻ�������С */
	uint16_t usRxBufSize;		/* ���ջ�������С */
	uint16_t usTxWrite;			/* ���ͻ�����дָ�� */
	uint16_t usTxRead;			/* ���ͻ�������ָ�� */
	uint16_t usTxCount;			/* �ȴ����͵����ݸ��� */

	uint16_t usRxWrite;			/* ���ջ�����дָ�� */
	uint16_t usRxRead;			/* ���ջ�������ָ�� */
	uint16_t usRxCount;			/* ��δ��ȡ�������ݸ��� */
	
	void (*SendBefor)(void); 	/* ��ʼ����֮ǰ�Ļص�����ָ�루��Ҫ����RS485�л�������ģʽ�� */
	void (*SendOver)(void); 	/* ������ϵĻص�����ָ�루��Ҫ����RS485������ģʽ�л�Ϊ����ģʽ�� */
	void (*ReciveNew)(uint8_t _byte);	/* �����յ����ݵĻص�����ָ�� */
	void (*ReciveFinish)(USART_TypeDef *_ucPort);	/* �����������ݵĻص�����ָ�� */	
}UART_T;



/* ���崮�ڲ����ʺ�FIFO��������С����Ϊ���ͻ������ͽ��ջ�����, ֧��ȫ˫�� */
#if UART1_FIFO_EN == 1
	extern UART_T g_tUart1;
#endif

#if UART2_FIFO_EN == 1
	extern UART_T g_tUart2;
#endif

#if UART3_FIFO_EN == 1	
	extern UART_T g_tUart3;		
#endif



void bsp_InitUart(void);
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen);
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte);
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte);

void comClearTxFifo(COM_PORT_E _ucPort);
void comClearRxFifo(COM_PORT_E _ucPort);	




/*����1�йصĺ���*/
void USART1_IRQ_SEND(void);
void USART1_IRQ_RECV(void);


	
/*����2�йصĺ���*/
void USART2_IRQ_SEND(void);
void USART2_IRQ_RECV(void);

	

/*����3�йصĺ���*/
void USART3_IRQ_SEND(void);
void USART3_IRQ_RECV(void);



u8 uart_recv_finish_get(COM_PORT_E _ucPort);	
void uart_recv_finish_clr(COM_PORT_E _ucPort);






#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
