/*
*********************************************************************************************************
*
*	模块名称 : 串口中断+FIFO驱动模块
*	文件名称 : bsp_uart_fifo.c
*	版    本 : V1.0
*	说    明 : 采用串口中断+FIFO模式实现多个串口的同时访问
*	修改记录 :
*		版本号  日期       作者    说明
*		V1.0    2013-02-01 armfly  正式发布
*		V1.1    2013-06-09 armfly  FiFo结构增加TxCount成员变量，方便判断缓冲区满; 增加 清FiFo的函数
*		V1.2	2014-09-29 armfly  增加RS485 MODBUS接口。接收到新字节后，直接执行回调函数。
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"	

		
/* 定义每个串口结构体变量 */
#if UART1_FIFO_EN == 1
	UART_T g_tUart1;	
	static uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART2_FIFO_EN == 1
	UART_T g_tUart2;
	static uint8_t g_TxBuf2[UART2_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf2[UART2_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART3_FIFO_EN == 1
	UART_T g_tUart3;
	static uint8_t g_TxBuf3[UART3_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf3[UART3_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif



	
static void UartVarInit(void);

static void InitHardUart(void);
static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen);
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte);
static void UartIRQ(UART_T *_pUart);

static void uart_recv_finish_set(USART_TypeDef *_ucPort);

void RS485_InitTXE(void);				

/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart
*	功能说明: 初始化串口硬件，并对全局变量赋初值.
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitUart(void)
{
	UartVarInit();		/* 必须先初始化全局变量,再配置硬件 */
		
	InitHardUart();	/* 配置串口的硬件参数(波特率等) */

	RS485_InitTXE();
}

/*
*********************************************************************************************************
*	函 数 名: ComToUart
*	功能说明: 将COM端口号转换为UART指针
*	形    参: _ucPort: 端口号(COM1 - COM6)
*	返 回 值: uart指针
*********************************************************************************************************
*/
UART_T *ComToUart(COM_PORT_E _ucPort)
{
	if (_ucPort == COM1)
	{
		#if UART1_FIFO_EN == 1
			return &g_tUart1;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM2)
	{
		#if UART2_FIFO_EN == 1
			return &g_tUart2;
		#else
			return 0;	
		#endif
	}
	else if (_ucPort == COM3)
	{
		#if UART3_FIFO_EN == 1
			return &g_tUart3;
		#else
			return 0;
		#endif
	}
	else
	{
		/* 不做任何处理 */
		return 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: comSendBuf
*	功能说明: 向串口发送一组数据。数据放到发送缓冲区后立即返回，由中断服务程序在后台完成发送
*	形    参: _ucPort: 端口号(COM1 - COM6)
*			  _ucaBuf: 待发送的数据缓冲区
*			  _usLen : 数据长度
*	返 回 值: 无
*********************************************************************************************************
*/
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return;
	}

	if (pUart->SendBefor != 0)
	{
		pUart->SendBefor();		/* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
	}

	UartSend(pUart, _ucaBuf, _usLen);
}

/*
*********************************************************************************************************
*	函 数 名: comSendChar
*	功能说明: 向串口发送1个字节。数据放到发送缓冲区后立即返回，由中断服务程序在后台完成发送
*	形    参: _ucPort: 端口号(COM1 - COM6)
*			  _ucByte: 待发送的数据
*	返 回 值: 无
*********************************************************************************************************
*/
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte)
{ 
	comSendBuf(_ucPort, &_ucByte, 1);
}
 
/*
*********************************************************************************************************
*	函 数 名: comGetChar
*	功能说明: 从串口缓冲区读取1字节，非阻塞。无论有无数据均立即返回
*	形    参: _ucPort: 端口号(COM1 - COM6)
*			  _pByte: 接收到的数据存放在这个地址
*	返 回 值: 0 表示无数据, 1 表示读取到有效字节
*********************************************************************************************************
*/
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte)
{
	UART_T *pUart;
	
	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{	
		return 0;
	}

	return UartGetChar(pUart, _pByte);
}

/*
*********************************************************************************************************
*	函 数 名: comClearTxFifo
*	功能说明: 清零串口发送缓冲区
*	形    参: _ucPort: 端口号(COM1 - COM6)
*	返 回 值: 无
*********************************************************************************************************
*/
void comClearTxFifo(COM_PORT_E _ucPort)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return;
	}

	pUart->usTxWrite = 0;
	pUart->usTxRead = 0;
	pUart->usTxCount = 0;
}

/*
*********************************************************************************************************
*	函 数 名: comClearRxFifo
*	功能说明: 清零串口接收缓冲区
*	形    参: _ucPort: 端口号(COM1 - COM6)
*	返 回 值: 无
*********************************************************************************************************
*/
void comClearRxFifo(COM_PORT_E _ucPort)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return;
	}

	pUart->usRxWrite = 0;
	pUart->usRxRead = 0;
	pUart->usRxCount = 0;

	memset(pUart->pRxBuf, 0, pUart->usRxBufSize);				
}









/* 如果是RS485通信，请按如下格式编写函数， 我们仅举了 USART3作为RS485的例子 */

/*
*********************************************************************************************************
*	函 数 名: RS485_InitTXE
*	功能说明: 配置RS485发送使能口线 TXE
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RS485_InitTXE(void)
{			
    GPIO_Init(PORT_RS485_TXEN, PIN_RS485_TXEN, GPIO_Mode_Out_PP_Low_Fast);
}	

/*
*********************************************************************************************************
*	函 数 名: bsp_Set485Baud
*	功能说明: 修改UART2波特率
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Set485Baud(uint32_t _baud)
{
	
}

/*
*********************************************************************************************************
*	函 数 名: RS485_SendBefor
*	功能说明: 发送数据前的准备工作。对于RS485通信，请设置RS485芯片为发送状态，
*			  并修改 UartVarInit()中的函数指针等于本函数名，比如 g_tUart2.SendBefor = RS485_SendBefor
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RS485_SendBefor(void)
{
	RS485_TX_EN();	/* 切换RS485收发芯片为发送模式 */
}

/*
*********************************************************************************************************
*	函 数 名: RS485_SendOver
*	功能说明: 发送一串数据结束后的善后处理。对于RS485通信，请设置RS485芯片为接收状态，
*			  并修改 UartVarInit()中的函数指针等于本函数名，比如 g_tUart2.SendOver = RS485_SendOver
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RS485_SendOver(void)
{
	RS485_RX_EN();	/* 切换RS485收发芯片为接收模式 */
}


/*
*********************************************************************************************************
*	函 数 名: RS485_SendBuf
*	功能说明: 通过RS485芯片发送一串数据。注意，本函数不等待发送完毕。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RS485_SendBuf(uint8_t *_ucaBuf, uint16_t _usLen)
{
	comSendBuf(COM1, _ucaBuf, _usLen);
}		


/*
*********************************************************************************************************
*	函 数 名: RS485_SendStr
*	功能说明: 向485总线发送一个字符串
*	形    参: _pBuf 数据缓冲区
*			 _ucLen 数据长度
*	返 回 值: 无
*********************************************************************************************************
*/
void RS485_SendStr(char *_pBuf)
{		
	RS485_SendBuf((uint8_t *)_pBuf, strlen(_pBuf));
}

/*
*********************************************************************************************************
*	函 数 名: RS485_ReciveNew
*	功能说明: 接收到新的数据
*	形    参: _byte 接收到的新数据
*	返 回 值: 无
*********************************************************************************************************
*/
void RS485_ReciveNew(uint8_t _byte)
{	
	
}










/*
*********************************************************************************************************
*	函 数 名: InitHardUart
*	功能说明: 配置串口的硬件参数（波特率，数据位，停止位，起始位，校验位，中断使能）适合于STM32-F4开发板
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitHardUart(void)
{
#if UART1_FIFO_EN == 1		

	/* 第1步： 配置GPIO */
    GPIO_ExternalPullUpConfig(GPIOC, GPIO_Pin_3, ENABLE);
    GPIO_ExternalPullUpConfig(GPIOC, GPIO_Pin_2, DISABLE);	

	/* 第2步： 配置串口硬件参数 */
	USART_DeInit(USART1);	

	USART_Init(USART1, UART1_BAUD,	
		        USART_WordLength_8b,	
		        USART_StopBits_1,
		        USART_Parity_No,
		        (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));

	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);	/* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(USART1, ENABLE);		/* 使能串口 */

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART1, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
	
#endif

#if UART2_FIFO_EN == 1		

	/* 第1步： 配置GPIO */
    GPIO_ExternalPullUpConfig(GPIOE, GPIO_Pin_4, ENABLE);
    GPIO_ExternalPullUpConfig(GPIOE, GPIO_Pin_3, DISABLE);
	
	/* 第2步： 配置串口硬件参数 */
	USART_DeInit(USART2);
	
	USART_Init(USART2, UART2_BAUD,
		         USART_WordLength_8b,	
		         USART_StopBits_1,
		         USART_Parity_No,
		         (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);	/* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(USART2, ENABLE);		/* 使能串口 */

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART2, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
#endif

#if UART3_FIFO_EN == 1		

	/* 第1步： 配置GPIO */
    GPIO_ExternalPullUpConfig(GPIOG, GPIO_Pin_1, ENABLE);	
    GPIO_ExternalPullUpConfig(GPIOG, GPIO_Pin_0, DISABLE);	

	/* 第2步： 配置串口硬件参数 */

	USART_DeInit(USART3);

	/* USART configuration */
	USART_Init(USART3, UART3_BAUD,
	         USART_WordLength_8b,	
	         USART_StopBits_1,
	         USART_Parity_No,
	         (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));

		
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);	/* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(USART3, ENABLE);		/* 使能串口 */

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART3, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
#endif

}









/*
*********************************************************************************************************
*	函 数 名: UartVarInit
*	功能说明: 初始化串口相关的变量
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void UartVarInit(void)
{			
#if UART1_FIFO_EN == 1	
	g_tUart1.uart = USART1;						/* STM32 串口设备 */
	g_tUart1.pTxBuf = g_TxBuf1;					/* 发送缓冲区指针 */
	g_tUart1.pRxBuf = g_RxBuf1;					/* 接收缓冲区指针 */
	g_tUart1.usTxBufSize = UART1_TX_BUF_SIZE;		/* 发送缓冲区大小 */
	g_tUart1.usRxBufSize = UART1_RX_BUF_SIZE;		/* 接收缓冲区大小 */
	g_tUart1.usTxWrite = 0;						/* 发送FIFO写索引 */
	g_tUart1.usTxRead = 0;							/* 发送FIFO读索引 */
	g_tUart1.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart1.usRxRead = 0;							/* 接收FIFO读索引 */
	g_tUart1.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart1.usTxCount = 0;						/* 待发送的数据个数 */
	g_tUart1.SendBefor = RS485_SendBefor;			/* 发送数据前的回调函数 */
	g_tUart1.SendOver = RS485_SendOver;			/* 发送完毕后的回调函数 */
	g_tUart1.ReciveNew = RS485_ReciveNew;			/* 接收到新数据后的回调函数 */
	g_tUart1.ReciveFinish = uart_recv_finish_set;	/* 完全接收到新数据后的回调函数 */
#endif		
	
#if UART2_FIFO_EN == 1
	g_tUart2.uart = USART2;						/* STM32 串口设备 */
	g_tUart2.pTxBuf = g_TxBuf2;					/* 发送缓冲区指针 */
	g_tUart2.pRxBuf = g_RxBuf2;					/* 接收缓冲区指针 */
	g_tUart2.usTxBufSize = UART2_TX_BUF_SIZE;		/* 发送缓冲区大小 */
	g_tUart2.usRxBufSize = UART2_RX_BUF_SIZE;		/* 接收缓冲区大小 */
	g_tUart2.usTxWrite = 0;						/* 发送FIFO写索引 */
	g_tUart2.usTxRead = 0;							/* 发送FIFO读索引 */
	g_tUart2.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart2.usRxRead = 0;							/* 接收FIFO读索引 */
	g_tUart2.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart2.usTxCount = 0;						/* 待发送的数据个数 */
	g_tUart2.SendBefor = 0;						/* 发送数据前的回调函数 */
	g_tUart2.SendOver = 0;							/* 发送完毕后的回调函数 */
	g_tUart2.ReciveNew = 0;						/* 接收到新数据后的回调函数 */
	g_tUart2.ReciveFinish = uart_recv_finish_set;	/* 完全接收到新数据后的回调函数 */
#endif

#if UART3_FIFO_EN == 1
	g_tUart3.uart = USART3;						/* STM32 串口设备 */
	g_tUart3.pTxBuf = g_TxBuf3;					/* 发送缓冲区指针 */
	g_tUart3.pRxBuf = g_RxBuf3;					/* 接收缓冲区指针 */
	g_tUart3.usTxBufSize = UART3_TX_BUF_SIZE;		/* 发送缓冲区大小 */
	g_tUart3.usRxBufSize = UART3_RX_BUF_SIZE;		/* 接收缓冲区大小 */
	g_tUart3.usTxWrite = 0;						/* 发送FIFO写索引 */
	g_tUart3.usTxRead = 0;							/* 发送FIFO读索引 */
	g_tUart3.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart3.usRxRead = 0;							/* 接收FIFO读索引 */
	g_tUart3.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart3.usTxCount = 0;						/* 待发送的数据个数 */
	g_tUart3.SendBefor = 0;						/* 发送数据前的回调函数 */
	g_tUart3.SendOver = 0;							/* 发送完毕后的回调函数 */
	g_tUart3.ReciveNew = 0;						/* 接收到新数据后的回调函数 */
	g_tUart3.ReciveFinish = uart_recv_finish_set;	/* 完全接收到新数据后的回调函数 */
#endif	
	
}

/*
*********************************************************************************************************
*	函 数 名: UartSend
*	功能说明: 填写数据到UART发送缓冲区,并启动发送中断。中断处理函数发送完毕后，自动关闭发送中断
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen)
{
	uint16_t i;

	for (i = 0; i < _usLen; i++)
	{
		/* 如果发送缓冲区已经满了，则等待缓冲区空 */
	#if 0
		/*
			在调试GPRS例程时，下面的代码出现死机，while 死循环
			原因： 发送第1个字节时 _pUart->usTxWrite = 1；_pUart->usTxRead = 0;
			将导致while(1) 无法退出
		*/
		while (1)
		{
			uint16_t usRead;

			DISABLE_INT();
			usRead = _pUart->usTxRead;
			ENABLE_INT();

			if (++usRead >= _pUart->usTxBufSize)
			{
				usRead = 0;
			}

			if (usRead != _pUart->usTxWrite)
			{
				break;
			}
		}
	#else
		/* 当 _pUart->usTxBufSize == 1 时, 下面的函数会死掉(待完善) */
		while (1)
		{
			uint16_t usCount;

			disableInterrupts();
			usCount = _pUart->usTxCount;
			enableInterrupts();

			if (usCount < _pUart->usTxBufSize)
			{
				break;
			}
		}
	#endif

		/* 将新数据填入发送缓冲区 */
		_pUart->pTxBuf[_pUart->usTxWrite] = _ucaBuf[i];
		
		disableInterrupts();
		if (++_pUart->usTxWrite >= _pUart->usTxBufSize)
		{
			_pUart->usTxWrite = 0;
		}
		_pUart->usTxCount++;
		enableInterrupts();	
	}

	USART_ITConfig(_pUart->uart, USART_IT_TXE, ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: UartGetChar
*	功能说明: 从串口接收缓冲区读取1字节数据 （用于主程序调用）
*	形    参: _pUart : 串口设备
*			  _pByte : 存放读取数据的指针
*	返 回 值: 0 表示无数据  1表示读取到数据
*********************************************************************************************************
*/
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte)
{
	uint16_t usCount;

	/* usRxWrite 变量在中断函数中被改写，主程序读取该变量时，必须进行临界区保护 */
	disableInterrupts();
	usCount = _pUart->usRxCount;
	enableInterrupts();

	/* 如果读和写索引相同，则返回0 */
	//if (_pUart->usRxRead == usRxWrite)
	if (usCount == 0)	/* 已经没有数据 */
	{
		return 0;
	}
	else
	{
		*_pByte = _pUart->pRxBuf[_pUart->usRxRead];		/* 从串口接收FIFO取1个数据 */

		/* 改写FIFO读索引 */
		disableInterrupts();	
		if (++_pUart->usRxRead >= _pUart->usRxBufSize)
		{	
			_pUart->usRxRead = 0;
		}
		_pUart->usRxCount--;
		enableInterrupts();
		return 1;
	}
}

/*
*********************************************************************************************************
*	函 数 名: UartIRQ
*	功能说明: 供中断服务程序调用，通用串口中断处理函数
*	形    参: _pUart : 串口设备
*	返 回 值: 无
*********************************************************************************************************
*/
static void UartIRQ_Recv(UART_T *_pUart)
{
	/* 处理接收中断  */
	if (USART_GetITStatus(_pUart->uart, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(_pUart->uart,USART_IT_RXNE);		

		/* 从串口接收数据寄存器读取数据存放到接收FIFO */
		uint8_t ch;

		ch = USART_ReceiveData8(_pUart->uart);
		_pUart->pRxBuf[_pUart->usRxWrite] = ch;
		if (++_pUart->usRxWrite >= _pUart->usRxBufSize)
		{
			_pUart->usRxWrite = 0;
		}
		if (_pUart->usRxCount < _pUart->usRxBufSize)
		{
			_pUart->usRxCount++;
		}

		/* 回调函数,通知应用程序收到新数据,一般是发送1个消息或者设置一个标记 */
		//if (_pUart->usRxWrite == _pUart->usRxRead)
		//if (_pUart->usRxCount == 1)
		{
			if (_pUart->ReciveNew)
			{	
				_pUart->ReciveNew(ch);
			}
		}
	}
	else if(USART_GetITStatus(_pUart->uart, USART_IT_IDLE) != RESET)
	{
		USART_ClearITPendingBit(_pUart->uart,USART_IT_IDLE);	
		USART_ReceiveData8(_pUart->uart);                  // idle读SR和DR清标志

		if (_pUart->ReciveFinish)
		{					
			_pUart->ReciveFinish(_pUart->uart);
		}	
	}
}
	

static void UartIRQ_Send(UART_T *_pUart)
{
	/* 处理发送缓冲区空中断 */
	if (USART_GetITStatus(_pUart->uart, USART_IT_TXE) != RESET)
	{
		//if (_pUart->usTxRead == _pUart->usTxWrite)
		if (_pUart->usTxCount == 0)
		{
			/* 发送缓冲区的数据已取完时， 禁止发送缓冲区空中断 （注意：此时最后1个数据还未真正发送完毕）*/
			USART_ITConfig(_pUart->uart, USART_IT_TXE, DISABLE);

			/* 使能数据发送完毕中断 */
			USART_ITConfig(_pUart->uart, USART_IT_TC, ENABLE);
		}
		else
		{
			/* 从发送FIFO取1个字节写入串口发送数据寄存器 */
			USART_SendData8(_pUart->uart, _pUart->pTxBuf[_pUart->usTxRead]);
			if (++_pUart->usTxRead >= _pUart->usTxBufSize)
			{
				_pUart->usTxRead = 0;
			}
			_pUart->usTxCount--;
		}	
	}
	/* 数据bit位全部发送完毕的中断 */
	else if (USART_GetITStatus(_pUart->uart, USART_IT_TC) != RESET)
	{
		//if (_pUart->usTxRead == _pUart->usTxWrite)
		if (_pUart->usTxCount == 0)
		{
			/* 如果发送FIFO的数据全部发送完毕，禁止数据发送完毕中断 */
			USART_ITConfig(_pUart->uart, USART_IT_TC, DISABLE);

			/* 回调函数, 一般用来处理RS485通信，将RS485芯片设置为接收模式，避免抢占总线 */
			if (_pUart->SendOver)
			{
				_pUart->SendOver();
			}
		}
		else
		{
			/* 正常情况下，不会进入此分支 */
			
			/* 如果发送FIFO的数据还未完毕，则从发送FIFO取1个数据写入发送数据寄存器 */
			USART_SendData8(_pUart->uart, _pUart->pTxBuf[_pUart->usTxRead]);
			if (++_pUart->usTxRead >= _pUart->usTxBufSize)
			{
				_pUart->usTxRead = 0;
			}
			_pUart->usTxCount--;
		}
	}
}


/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler  USART2_IRQHandler USART3_IRQHandler UART4_IRQHandler UART5_IRQHandler
*	功能说明: USART中断服务程序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
#if UART1_FIFO_EN == 1

void USART1_IRQ_SEND(void)
{
	UartIRQ_Send(&g_tUart1);
}

void USART1_IRQ_RECV(void)
{
	UartIRQ_Recv(&g_tUart1);
}

#else

void USART1_IRQ_SEND(void)
{	
}
	
void USART1_IRQ_RECV(void)
{
}


#endif

#if UART2_FIFO_EN == 1

void USART2_IRQ_SEND(void)	
{
	UartIRQ_Send(&g_tUart2);
}

void USART2_IRQ_RECV(void)
{
	UartIRQ_Recv(&g_tUart2);	
}

#else

void USART2_IRQ_SEND(void)	
{
}

void USART2_IRQ_RECV(void)
{
}


#endif

#if UART3_FIFO_EN == 1

void USART3_IRQ_SEND(void)
{
	UartIRQ_Send(&g_tUart3);		
}

void USART3_IRQ_RECV(void)
{
	UartIRQ_Recv(&g_tUart3);	
}

#else

void USART3_IRQ_SEND(void)
{
}

void USART3_IRQ_RECV(void)
{
}


#endif




static u8 uart1_recv_finish = 0;
static u8 uart2_recv_finish = 0;
static u8 uart3_recv_finish = 0;



static void uart_recv_finish_set(USART_TypeDef *_ucPort)	
{
	if (_ucPort == USART1)
	{
		#if UART1_FIFO_EN == 1
			uart1_recv_finish = 1;
		#endif
	}
	else if (_ucPort == USART2)
	{
		#if UART2_FIFO_EN == 1
			uart2_recv_finish = 1;
		#endif
	}
	else if (_ucPort == USART3)
	{
		#if UART3_FIFO_EN == 1
			uart3_recv_finish = 1;
		#endif
	}
}





u8 uart_recv_finish_get(COM_PORT_E _ucPort)	
{
	if (_ucPort == COM1)
	{
		#if UART1_FIFO_EN == 1
			return uart1_recv_finish;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM2)
	{
		#if UART2_FIFO_EN == 1
			return uart2_recv_finish;
		#else
			return 0;	
		#endif
	}
	else if (_ucPort == COM3)
	{
		#if UART3_FIFO_EN == 1
			return uart3_recv_finish;		
		#else
			return 0;
		#endif
	}
	else
	{
		/* 不做任何处理 */
		return 0;
	}
}

void uart_recv_finish_clr(COM_PORT_E _ucPort)
{	
	if (_ucPort == COM1)
	{
	#if UART1_FIFO_EN == 1
		uart1_recv_finish = 0;
		comClearRxFifo(COM1);			
	#endif
	}
	else if (_ucPort == COM2)
	{
	#if UART2_FIFO_EN == 1
		uart2_recv_finish = 0;
		comClearRxFifo(COM2);	
	#endif
	}
	else if (_ucPort == COM3)
	{
	#if UART3_FIFO_EN == 1
		uart3_recv_finish = 0;	
		comClearRxFifo(COM3);	
	#endif
	}	
}


	





/*
*********************************************************************************************************
*	函 数 名: fputc
*	功能说明: 重定义putc函数，这样可以使用printf函数从串口1打印输出
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/	
int fputc(int ch, FILE *f)
{				
	comSendChar(DEBUG_COM, ch);	
			
	return ch;
	
}

/*
*********************************************************************************************************
*	函 数 名: fgetc
*	功能说明: 重定义getc函数，这样可以使用getchar函数从串口1输入数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fgetc(FILE *f)
{
	uint8_t ucData;
				
	while(comGetChar(DEBUG_COM, &ucData) == 0);

	return ucData;

}





