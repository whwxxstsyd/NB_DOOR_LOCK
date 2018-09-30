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



u8 USART1_RX_BUF[USART1_REC_LEN];    										//USART1���ջ���,���USART1_REC_LEN���ֽ�
u8 USART2_RX_BUF[USART2_REC_LEN];    										//USART2���ջ���,���USART2_REC_LEN���ֽ�

u8 USART1_RX_CACHE_BUF[USART1_CACHE_LEN];									//USART1���ջ���2,���USART1_CACHE_LEN���ֽ�
u8 USART2_RX_CACHE_BUF[USART2_CACHE_LEN];									//USART2���ջ���2,���USART2_CACHE_LEN���ֽ�

u16 USART1_RX_STA = 0;					//USART1����״̬��� bit15,������ɱ�־ bit14,���յ�0x0d(\n) bit13,�����б�־
u16 USART2_RX_STA = 0;					//USART2����״̬��� bit15,������ɱ�־ bit14,���յ�0x0d(\n) bit13,�����б�־

u16 USART1_RX_CACHE_STA = 0;			//USART1����״̬��� bit15,������ɱ�־ bit14,���յ�0x0d(\n) bit13,�����б�־
u16 USART2_RX_CACHE_STA = 0;			//USART2����״̬��� bit15,������ɱ�־ bit14,���յ�0x0d(\n) bit13,�����б�־



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
 @Description			NBIOT_Transport_SendATCmd 				: ����ATָ��ȴ�Ӧ��
 @Input				ATCmd		 						: ATָ��ṹ��
 @Return				NBIOT_StatusTypeDef						: NBIOT����״̬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Transport_SendATCmd(NBIOT_ATCmdTypeDef* ATCmd)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	Stm8_CalculagraphTypeDef Serial_timer_Ms;
	
	if (ATCmd->ATSendlen > ATCmd->ATSendbuf_size) {					//�������ݳ��ȼ��
		NBStatus = NBIOT_ERROR;
		goto exit;
	}
	
	Uart1_PortSerialEnable(ENABLE, DISABLE);															//���������ж�
	USART1_RX_STA |= 0x2000;																			//����״̬��λ
	memset((void *)USART1_RX_BUF, 0x0, sizeof(USART1_RX_BUF));												//��ջ���ռ�
	USART1_RX_CACHE_STA = 0;																			//����״̬��λ
	memset((void *)USART1_RX_CACHE_BUF, 0x0, sizeof(USART1_RX_CACHE_BUF));									//��ջ���ռ�
	
	if (HAL_OK != HAL_UART_Transmit(&UART1_Handler, ATCmd->ATSendbuf, ATCmd->ATSendlen, 0xFFFF)) {					//��������
		NBStatus = NBIOT_ERROR;
		Uart1_PortSerialEnable(DISABLE, DISABLE);
		goto exit;
	}
	
	if ((ATCmd->ATack && ATCmd->CmdWaitTime.xTicksToWait) || (ATCmd->ATNack && ATCmd->CmdWaitTime.xTicksToWait)) {	//��Ҫ�ȴ�Ӧ��
		
		Stm32_Calculagraph_CountdownMS(&Serial_timer_Ms, NBIOT_SERIAL_TIMEOUT_MSEC);
		ATCmd->SerialWaitTime = Serial_timer_Ms;
		
		while (Stm32_Calculagraph_IsExpiredMS(&ATCmd->SerialWaitTime) != true) {								//�ȴ�����ʱ
			if (USART1_RX_STA & 0x8000) {																//���յ��ڴ���Ӧ����
				memcpy(USART1_RX_CACHE_BUF + USART1_RX_CACHE_STA, USART1_RX_BUF, USART1_RX_STA & 0x1FFF);			//������ٻ���
				USART1_RX_CACHE_STA += (USART1_RX_STA & 0x1FFF);											//���ٻ��泤������
				USART1_RX_STA &= 0x2000;																//����״̬��0�ɼ�������
			}
			else if (USART1_RX_STA & 0x2000) {															//���ڽ�����
				Stm32_Calculagraph_CountdownMS(&Serial_timer_Ms, NBIOT_SERIAL_TIMEOUT_MSEC);
				ATCmd->SerialWaitTime = Serial_timer_Ms;
			}
			if (Stm32_Calculagraph_IsExpiredMS(&ATCmd->CmdWaitTime) == true) {								//��������ִ��ʱ��
				break;
			}
		}
		
		while (Stm32_Calculagraph_IsExpiredMS(&ATCmd->CmdWaitTime) != true) {									//�ȴ�����ʱ
			if (USART1_RX_CACHE_STA != 0) {															//���յ��ڴ���Ӧ����
				
				USART1_RX_CACHE_BUF[USART1_RX_CACHE_STA] = 0;											//��ӽ�����
				NBIOT_ClearStringHead(USART1_RX_CACHE_BUF, &USART1_RX_CACHE_STA);							//ȥ��ǰ��'\0'
				
				if (ATCmd->ATack && (strstr((const char*)USART1_RX_CACHE_BUF, (const char*)ATCmd->ATack) != NULL)) {			//Found! OK
					if (ATCmd->ATRecvbuf) {															//��ȡӦ������
						if (USART1_RX_CACHE_STA > ATCmd->ATRecvbuf_size) {								//����AT���ݳ��ȼ��
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
					if (ATCmd->ATRecvbuf) {															//��ȡӦ������
						if (USART1_RX_CACHE_STA > ATCmd->ATRecvbuf_size) {								//����AT���ݳ��ȼ��
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
 @Description			NBIOT_Transport_RecvATCmd 				: ����ATָ��Ӧ��
 @Input				ATCmd		 						: ATָ��ṹ��
 @Return				NBIOT_StatusTypeDef						: NBIOT����״̬
**********************************************************************************************************/
NBIOT_StatusTypeDef NBIOT_Transport_RecvATCmd(NBIOT_ATCmdTypeDef* ATCmd)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_OK;
	char *str = NULL;
	u16 ReceiveLength = 0;
	
	Uart1_PortSerialEnable(ENABLE, DISABLE);															//���������ж�
	USART1_RX_STA = 0;																				//����״̬��λ
	memset((void *)USART1_RX_BUF, 0x0, sizeof(USART1_RX_BUF));												//��ջ���ռ�
	
	if ((ATCmd->ATack && ATCmd->CmdWaitTime.xTicksToWait) || (ATCmd->ATNack && ATCmd->CmdWaitTime.xTicksToWait)) {	//��Ҫ�ȴ�Ӧ��
		while (Stm32_Calculagraph_IsExpiredMS(&ATCmd->CmdWaitTime) != true) {									//�ȴ�����ʱ
			if (USART1_RX_STA & 0x8000) {																//���յ��ڴ���Ӧ����
				
				/* NBIOT�����ʼ��� */
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
				
				USART1_RX_BUF[USART1_RX_STA & 0x1FFF] = 0;												//��ӽ�����
				if (ATCmd->ATack && ((str = strstr((const char*)USART1_RX_BUF, (const char*)ATCmd->ATack)) != NULL)) {			//Found! OK
					if (ATCmd->ATRecvbuf) {															//��ȡӦ������
						if ((ReceiveLength + (USART1_RX_STA & 0x1FFF)) >= ATCmd->ATRecvbuf_size) {				//����AT���ݳ��ȼ��
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
					if (ATCmd->ATRecvbuf) {															//��ȡӦ������
						if ((ReceiveLength + (USART1_RX_STA & 0x1FFF)) >= ATCmd->ATRecvbuf_size) {				//����AT���ݳ��ȼ��
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
					if ((ReceiveLength + (USART1_RX_STA & 0x1FFF)) >= ATCmd->ATRecvbuf_size) {					//����AT���ݳ��ȼ��
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
 @Description			NBIOT_Transport_Init					: Initiative��ʼ��NBIOT���ݴ���ӿ�
 @Input				TCmd		 							: ATָ��ṹ��
 @Return				NBIOT_StatusTypeDef						: NBIOT����״̬
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

