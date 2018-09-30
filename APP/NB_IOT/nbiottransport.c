/**
  *********************************************************************************************************
  * @file    nbiottransport.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-03-03
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "nbiottransport.h"
#include "usart.h"



u8 USART1_RX_BUF[USART1_REC_LEN];    										//USART1接收缓冲,最大USART1_REC_LEN个字节
u8 USART2_RX_BUF[USART2_REC_LEN];    										//USART2接收缓冲,最大USART2_REC_LEN个字节

u8 USART1_RX_CACHE_BUF[USART1_CACHE_LEN];									//USART1接收缓存2,最大USART1_CACHE_LEN个字节
u8 USART2_RX_CACHE_BUF[USART2_CACHE_LEN];									//USART2接收缓存2,最大USART2_CACHE_LEN个字节

u16 USART1_RX_STA = 0;					//USART1接收状态标记 bit15,接收完成标志 bit14,接收到0x0d(\n) bit13,接收中标志
u16 USART2_RX_STA = 0;					//USART2接收状态标记 bit15,接收完成标志 bit14,接收到0x0d(\n) bit13,接收中标志

u16 USART1_RX_CACHE_STA = 0;			//USART1接收状态标记 bit15,接收完成标志 bit14,接收到0x0d(\n) bit13,接收中标志
u16 USART2_RX_CACHE_STA = 0;			//USART2接收状态标记 bit15,接收完成标志 bit14,接收到0x0d(\n) bit13,接收中标志



static void NBIOT_ClearStringHead(unsigned char* buf, unsigned short* len)
{
	unsigned int indexlen = 0;
	
	for (indexlen = 0; indexlen < *len; indexlen++) {
		if (buf[indexlen] != '\0') break;
	}
	
	if (indexlen > 0) {
		for (int indexcp = 0; indexcp < (*len - indexlen); indexcp++) {
			buf[indexcp] = buf[indexcp + indexlen];
		}
		*len -= indexlen;
	}
}


/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Transport_SendATCmd(NBIOT_ATCmdTypeDef* ATCmd)
 @Description			NBIOT_Transport_SendATCmd 				: 发送AT指令等待应答
 @Input				ATCmd		 						: AT指令结构体
 @Return				NBIOT_StatusTypeDef						: NBIOT处理状态
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Transport_SendATCmd(NBIOT_ATCmdTypeDef* ATCmd)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	Stm8_CalculagraphTypeDef Serial_timer_Ms;
	
	if (ATCmd->ATSendlen > ATCmd->ATSendbuf_size) {					//发送数据长度检测
		NBStatus = NBIOT_ERROR;
		goto exit;
	}
	
	Uart1_PortSerialEnable(ENABLE, DISABLE);															//开启接收中断
	USART1_RX_STA |= 0x2000;																			//接收状态复位
	memset((void *)USART1_RX_BUF, 0x0, sizeof(USART1_RX_BUF));												//清空缓存空间
	USART1_RX_CACHE_STA = 0;																			//接收状态复位
	memset((void *)USART1_RX_CACHE_BUF, 0x0, sizeof(USART1_RX_CACHE_BUF));									//清空缓存空间
	
	if (HAL_OK != HAL_UART_Transmit(&UART1_Handler, ATCmd->ATSendbuf, ATCmd->ATSendlen, 0xFFFF)) {					//发送命令
		NBStatus = NBIOT_ERROR;
		Uart1_PortSerialEnable(DISABLE, DISABLE);
		goto exit;
	}
	
	if ((ATCmd->ATack && ATCmd->CmdWaitTime.xTicksToWait) || (ATCmd->ATNack && ATCmd->CmdWaitTime.xTicksToWait)) {	//需要等待应答
		
		Stm32_Calculagraph_CountdownMS(&Serial_timer_Ms, NBIOT_SERIAL_TIMEOUT_MSEC);
		ATCmd->SerialWaitTime = Serial_timer_Ms;
		
		while (Stm32_Calculagraph_IsExpiredMS(&ATCmd->SerialWaitTime) != true) {								//等待倒计时
			if (USART1_RX_STA & 0x8000) {																//接收到期待的应答结果
				memcpy(USART1_RX_CACHE_BUF + USART1_RX_CACHE_STA, USART1_RX_BUF, USART1_RX_STA & 0x1FFF);			//载入高速缓存
				USART1_RX_CACHE_STA += (USART1_RX_STA & 0x1FFF);											//高速缓存长度增加
				USART1_RX_STA &= 0x2000;																//缓存状态清0可继续接收
			}
			else if (USART1_RX_STA & 0x2000) {															//正在接收中
				Stm32_Calculagraph_CountdownMS(&Serial_timer_Ms, NBIOT_SERIAL_TIMEOUT_MSEC);
				ATCmd->SerialWaitTime = Serial_timer_Ms;
			}
			if (Stm32_Calculagraph_IsExpiredMS(&ATCmd->CmdWaitTime) == true) {								//超过命令执行时间
				break;
			}
		}
		
		while (Stm32_Calculagraph_IsExpiredMS(&ATCmd->CmdWaitTime) != true) {									//等待倒计时
			if (USART1_RX_CACHE_STA != 0) {															//接收到期待的应答结果
				
				USART1_RX_CACHE_BUF[USART1_RX_CACHE_STA] = 0;											//添加结束符
				NBIOT_ClearStringHead(USART1_RX_CACHE_BUF, &USART1_RX_CACHE_STA);							//去除前导'\0'
				
				if (ATCmd->ATack && (strstr((const char*)USART1_RX_CACHE_BUF, (const char*)ATCmd->ATack) != NULL)) {			//Found! OK
					if (ATCmd->ATRecvbuf) {															//获取应答数据
						if (USART1_RX_CACHE_STA > ATCmd->ATRecvbuf_size) {								//接收AT数据长度检测
							NBStatus = NBIOT_ERROR;
							goto exit;
						}
						strncpy((char *)ATCmd->ATRecvbuf, (char*)USART1_RX_CACHE_BUF, USART1_RX_CACHE_STA);
						ATCmd->ATRecvbuf[USART1_RX_CACHE_STA] = '\0';
						ATCmd->ATRecvlen = USART1_RX_CACHE_STA;
					}
					NBStatus = NBIOT_OK;
					break;
				}
				else if (ATCmd->ATNack && (strstr((const char*)USART1_RX_CACHE_BUF, (const char*)ATCmd->ATNack) != NULL)) {		//Found! Err
					if (ATCmd->ATRecvbuf) {															//获取应答数据
						if (USART1_RX_CACHE_STA > ATCmd->ATRecvbuf_size) {								//接收AT数据长度检测
							NBStatus = NBIOT_ERROR;
							goto exit;
						}
						strncpy((char *)ATCmd->ATRecvbuf, (char*)USART1_RX_CACHE_BUF, USART1_RX_CACHE_STA);
						ATCmd->ATRecvbuf[USART1_RX_CACHE_STA] = '\0';
						ATCmd->ATRecvlen = USART1_RX_CACHE_STA;
					}
					NBStatus = NBIOT_ERROR;
					break;
				}
				else {																						//NotFound!
					NBStatus = NBIOT_CMD_TIMEOUT;
					goto exit;
				}
			}
			else {
				NBStatus = NBIOT_CMD_TIMEOUT;
				goto exit;
			}
		}
		
		if (Stm32_Calculagraph_IsExpiredMS(&ATCmd->CmdWaitTime) == true) {
			ATCmd->ATRecvlen = 0;
			NBStatus = NBIOT_CMD_TIMEOUT;
		}
	}
	
exit:
	Uart1_PortSerialEnable(DISABLE, DISABLE);
	
	return NBStatus;
}

/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Transport_RecvATCmd(NBIOT_ATCmdTypeDef* ATCmd)
 @Description			NBIOT_Transport_RecvATCmd 				: 接收AT指令应答
 @Input				ATCmd		 						: AT指令结构体
 @Return				NBIOT_StatusTypeDef						: NBIOT处理状态
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Transport_RecvATCmd(NBIOT_ATCmdTypeDef* ATCmd)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	char *str = NULL;
	u16 ReceiveLength = 0;
	
	Uart1_PortSerialEnable(ENABLE, DISABLE);															//开启接收中断
	USART1_RX_STA = 0;																				//接收状态复位
	memset((void *)USART1_RX_BUF, 0x0, sizeof(USART1_RX_BUF));												//清空缓存空间
	
	if ((ATCmd->ATack && ATCmd->CmdWaitTime.xTicksToWait) || (ATCmd->ATNack && ATCmd->CmdWaitTime.xTicksToWait)) {	//需要等待应答
		while (Stm32_Calculagraph_IsExpiredMS(&ATCmd->CmdWaitTime) != true) {									//等待倒计时
			if (USART1_RX_STA & 0x8000) {																//接收到期待的应答结果
				
				/* NBIOT波特率计算 */
				if (NBIOTBaudRate.EnBaudRateState != false) {
					if ((USART1_RX_STA & 0x1FFF) > 30) {
						NBIOTBaudRate.NBIOTBaudRateCal.StartMs = NBIOTBaudRate.NBIOTBaudRateNow.MiddleMs;
						NBIOTBaudRate.NBIOTBaudRateCal.StartClock = NBIOTBaudRate.NBIOTBaudRateNow.MiddleClock;
						NBIOTBaudRate.NBIOTBaudRateCal.EndMs = NBIOTBaudRate.NBIOTBaudRateNow.EndMs;
						NBIOTBaudRate.NBIOTBaudRateCal.EndClock = NBIOTBaudRate.NBIOTBaudRateNow.EndClock;
						NBIOTBaudRate.NBIOTBaudRateCal.TotalNum = (USART1_RX_STA & 0x1FFF) - NBIOTBaudRate.NBIOTBaudRateNow.MiddleNum - 1;
					}
					else if ((USART1_RX_STA & 0x1FFF) > 15) {
						NBIOTBaudRate.NBIOTBaudRateCal.StartMs = NBIOTBaudRate.NBIOTBaudRateNow.StartMs;
						NBIOTBaudRate.NBIOTBaudRateCal.StartClock = NBIOTBaudRate.NBIOTBaudRateNow.StartClock;
						NBIOTBaudRate.NBIOTBaudRateCal.EndMs = NBIOTBaudRate.NBIOTBaudRateNow.EndMs;
						NBIOTBaudRate.NBIOTBaudRateCal.EndClock = NBIOTBaudRate.NBIOTBaudRateNow.EndClock;
						NBIOTBaudRate.NBIOTBaudRateCal.TotalNum = (USART1_RX_STA & 0x1FFF) - 1;
					}
					memset((void*)&NBIOTBaudRate.NBIOTBaudRateNow, 0, sizeof(NBIOTBaudRate.NBIOTBaudRateNow));
				}
				
				USART1_RX_BUF[USART1_RX_STA & 0x1FFF] = 0;												//添加结束符
				if (ATCmd->ATack && ((str = strstr((const char*)USART1_RX_BUF, (const char*)ATCmd->ATack)) != NULL)) {			//Found! OK
					if (ATCmd->ATRecvbuf) {															//获取应答数据
						if ((ReceiveLength + (USART1_RX_STA & 0x1FFF)) >= ATCmd->ATRecvbuf_size) {				//接收AT数据长度检测
							NBStatus = NBIOT_ERROR;
							goto exit;
						}
						strncpy((char *)ATCmd->ATRecvbuf + ReceiveLength, str, strlen(str));
						ATCmd->ATRecvbuf[strlen(str) + ReceiveLength] = '\0';
						ReceiveLength += USART1_RX_STA & 0x1FFF;
						ATCmd->ATRecvlen = ReceiveLength;
					}
					NBStatus = NBIOT_OK;
					break;
				}
				else if (ATCmd->ATNack && ((str = strstr((const char*)USART1_RX_BUF, (const char*)ATCmd->ATNack)) != NULL)) {		//Found! Err
					if (ATCmd->ATRecvbuf) {															//获取应答数据
						if ((ReceiveLength + (USART1_RX_STA & 0x1FFF)) >= ATCmd->ATRecvbuf_size) {				//接收AT数据长度检测
							NBStatus = NBIOT_ERROR;
							goto exit;
						}
						strncpy((char *)ATCmd->ATRecvbuf + ReceiveLength, str, strlen(str));
						ATCmd->ATRecvbuf[strlen(str) + ReceiveLength] = '\0';
						ReceiveLength += USART1_RX_STA & 0x1FFF;
						ATCmd->ATRecvlen = ReceiveLength;
					}
					NBStatus = NBIOT_ERROR;
					break;
				}
				else if (ATCmd->ATRecvbuf) {																		//No Found!
					if ((ReceiveLength + (USART1_RX_STA & 0x1FFF)) >= ATCmd->ATRecvbuf_size) {					//接收AT数据长度检测
						NBStatus = NBIOT_ERROR;
						goto exit;
					}
					strncpy((char *)ATCmd->ATRecvbuf + ReceiveLength, (char *)USART1_RX_BUF, USART1_RX_STA & 0x1FFF);
					ReceiveLength += USART1_RX_STA & 0x1FFF;
				}
				
				USART1_RX_STA = 0;																	//NotFound!
			}
		}
		
		if (Stm32_Calculagraph_IsExpiredMS(&ATCmd->CmdWaitTime) == true) {
			ATCmd->ATRecvlen = 0;
			NBStatus = NBIOT_CMD_TIMEOUT;
		}
	}
	
exit:
	Uart1_PortSerialEnable(DISABLE, DISABLE);
	
	return NBStatus;
}







unsigned char NBIOT_ATSendBuf[NBIOT_ATBUFFER_SIZE];
unsigned char NBIOT_ATRecvBuf[NBIOT_ATBUFFER_SIZE];


/**********************************************************************************************************
 @Function			NBIOT_StatusTypeDef NBIOT_Transport_Init(NBIOT_ATCmdTypeDef* ATCmd)
 @Description			NBIOT_Transport_Init					: Initiative初始化NBIOT数据传输接口
 @Input				TCmd		 							: AT指令结构体
 @Return				NBIOT_StatusTypeDef						: NBIOT处理状态
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Transport_Init(NBIOT_ATCmdTypeDef* ATCmd)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	
	ATCmd->ATSendbuf			= NBIOT_ATSendBuf;
	ATCmd->ATRecvbuf			= NBIOT_ATRecvBuf;
	ATCmd->ATSendbuf_size		= sizeof(NBIOT_ATSendBuf);
	ATCmd->ATRecvbuf_size		= sizeof(NBIOT_ATRecvBuf);
	
	ATCmd->ATSendlen			= 0;	
	ATCmd->ATRecvlen			= 0;	
	
	ATCmd->Write				= NBIOT_Transport_SendATCmd;
	ATCmd->Read					= NBIOT_Transport_RecvATCmd;
	
	return NBStatus;
}


/********************************************** END OF FLEE **********************************************/

