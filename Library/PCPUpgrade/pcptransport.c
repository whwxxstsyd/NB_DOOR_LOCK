/**
  *********************************************************************************************************
  * @file    pcptransport.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-07-16
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "pcptransport.h"
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "delay.h"
#include "usart.h"

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Transport_Write(PCP_CoAPNetTransportTypeDef* NetCoAP, const char *buf, u16 sendlen)
 @Description			PCP_Transport_Write		: CoAP����һ����������
 @Input				NetCoAP				: CoAPNet Struct*
					buf					: ��������
					sendlen 				: ���ݳ���
 @Return				PCP_StatusTypeDef		: PCP����״̬
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Transport_Write(PCP_CoAPNetTransportTypeDef* NetCoAP, const char *buf, u16 sendlen)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_ERROR;
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	
	memcpy(NetCoAP->NBIotStack->Sendbuf, buf, sendlen);
	NetCoAP->NBIotStack->Sendlen = sendlen;
	
	NBStatus = NBIOT_Neul_NBxx_SendCOAPPayloadFlag(NetCoAP->NBIotStack, "0x0100");
	if (NBStatus != NBIOT_OK) {
		PCPStatus = (PCP_StatusTypeDef)NBStatus;
		goto exit;
	}
	else {
		PCPStatus = PCP_OK;
	}
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Transport_Read(PCP_CoAPNetTransportTypeDef* NetCoAP, char *buf, u16* recvlen)
 @Description			PCP_Transport_Read		: CoAP��ȡһ����������
 @Input				NetCoAP				: CoAPNet Struct*
					buf					: ��ȡ���ݻ���
					recvlen				: ��ȡ���ݳ��ȵ�ַ
 @Return				PCP_StatusTypeDef		: PCP����״̬
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Transport_Read(PCP_CoAPNetTransportTypeDef* NetCoAP, char *buf, u16* recvlen)
{
	NBIOT_StatusTypeDef NBStatus = NBIOT_ERROR;
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	
	NBStatus = NBIOT_Neul_NBxx_ReadCOAPPayload(NetCoAP->NBIotStack);
	if (NBStatus != NBIOT_OK) {
		PCPStatus = (PCP_StatusTypeDef)NBStatus;
		goto exit;
	}
	else {
		PCPStatus = PCP_OK;
	}
	
	memcpy(buf, NetCoAP->NBIotStack->Recvbuf, NetCoAP->NBIotStack->Recvlen);
	*recvlen = NetCoAP->NBIotStack->Recvlen;
	
exit:
	return PCPStatus;
}

/**********************************************************************************************************
 @Function			PCP_StatusTypeDef PCP_Transport_Init(PCP_CoAPNetTransportTypeDef* NetCoAP, NBIOT_ClientsTypeDef* NBIotStack)
 @Description			PCP_Transport_Init		: Initiative��ʼ��PCP���ݴ���ӿ�
 @Input				NetCoAP				: CoAPNet Struct*
					NBIotStack			: NBIotЭ��ջ
 @Return				PCP_StatusTypeDef		: PCP����״̬
**********************************************************************************************************/
PCP_StatusTypeDef PCP_Transport_Init(PCP_CoAPNetTransportTypeDef* NetCoAP, NBIOT_ClientsTypeDef* NBIotStack)
{
	PCP_StatusTypeDef PCPStatus = PCP_OK;
	
	NetCoAP->NBIotStack = NBIotStack;
	NetCoAP->Write = PCP_Transport_Write;
	NetCoAP->Read = PCP_Transport_Read;
	
	return PCPStatus;
}

/********************************************** END OF FLEE **********************************************/

