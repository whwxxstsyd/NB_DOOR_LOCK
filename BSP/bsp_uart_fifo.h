/*
*********************************************************************************************************
*
*	模块名称 : 串口中断+FIFO驱动模块
*	文件名称 : bsp_uart_fifo.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_USART_FIFO_H_
#define _BSP_USART_FIFO_H_


#define	UART1_FIFO_EN	0					
#define	UART2_FIFO_EN	1
#define	UART3_FIFO_EN	1			

/* RS485芯片发送使能GPIO, PB2 */
#define PORT_RS485_TXEN  	GPIOC
#define PIN_RS485_TXEN	 	GPIO_Pin_4
	
#define RS485_RX_EN()	GPIO_ResetBits(PORT_RS485_TXEN,PIN_RS485_TXEN)	
#define RS485_TX_EN()	GPIO_SetBits(PORT_RS485_TXEN,PIN_RS485_TXEN)
	

/* 定义端口号 */
typedef enum
{
	COM1 = 0,	/* USART1  PA9, PA10 或  PB6, PB7*/
	COM2 = 1,	/* USART2, PD5,PD6 或 PA2, PA3 */
	COM3 = 2,	/* USART3, PB10, PB11 */
}COM_PORT_E;
	

#define UART_BUF_SIZE_MAX 	512														


/* 定义串口波特率和FIFO缓冲区大小，分为发送缓冲区和接收缓冲区, 支持全双工 */
/* 定义串口波特率和FIFO缓冲区大小，分为发送缓冲区和接收缓冲区, 支持全双工 */
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



/* 串口设备结构体 */
typedef struct
{
	USART_TypeDef *uart;		/* STM32内部串口设备指针 */
	uint8_t *pTxBuf;			/* 发送缓冲区 */
	uint8_t *pRxBuf;			/* 接收缓冲区 */
	uint16_t usTxBufSize;		/* 发送缓冲区大小 */
	uint16_t usRxBufSize;		/* 接收缓冲区大小 */
	uint16_t usTxWrite;			/* 发送缓冲区写指针 */
	uint16_t usTxRead;			/* 发送缓冲区读指针 */
	uint16_t usTxCount;			/* 等待发送的数据个数 */

	uint16_t usRxWrite;			/* 接收缓冲区写指针 */
	uint16_t usRxRead;			/* 接收缓冲区读指针 */
	uint16_t usRxCount;			/* 还未读取的新数据个数 */
	
	void (*SendBefor)(void); 	/* 开始发送之前的回调函数指针（主要用于RS485切换到发送模式） */
	void (*SendOver)(void); 	/* 发送完毕的回调函数指针（主要用于RS485将发送模式切换为接收模式） */
	void (*ReciveNew)(uint8_t _byte);	/* 串口收到数据的回调函数指针 */
	void (*ReciveFinish)(USART_TypeDef *_ucPort);	/* 串口收完数据的回调函数指针 */	
}UART_T;



/* 定义串口波特率和FIFO缓冲区大小，分为发送缓冲区和接收缓冲区, 支持全双工 */
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




/*串口1有关的函数*/
void USART1_IRQ_SEND(void);
void USART1_IRQ_RECV(void);


	
/*串口2有关的函数*/
void USART2_IRQ_SEND(void);
void USART2_IRQ_RECV(void);

	

/*串口3有关的函数*/
void USART3_IRQ_SEND(void);
void USART3_IRQ_RECV(void);



u8 uart_recv_finish_get(COM_PORT_E _ucPort);	
void uart_recv_finish_clr(COM_PORT_E _ucPort);






#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
