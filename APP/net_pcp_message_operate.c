/**
  *********************************************************************************************************
  * @file    net_pcp_message_operate.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-07-18
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "net_pcp_message_operate.h"	
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "string.h"

PCP_SwapSendDataTypeDef		NETPcpMessageSendPark;
PCP_SwapRecvDataTypeDef		NETPcpMessageRecvPark;

/**********************************************************************************************************
 @Function			static bool NET_PCP_Message_SendDataisFull(void)
 @Description			NET_PCP_Message_SendDataisFull	: ��鷢�Ͷ����Ƿ�����
 @Input				void
 @Return				true							: ����
					false						: δ��
**********************************************************************************************************/
bool NET_PCP_Message_SendDataisFull(void)
{
	bool MessageState;
	
	if ((NETPcpMessageSendPark.Rear + 1) % PCP_SEND_PACK_NUM == NETPcpMessageSendPark.Front) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			static bool NET_PCP_Message_RecvDataisFull(void)
 @Description			NET_PCP_Message_RecvDataisFull	: �����ն����Ƿ�����
 @Input				void
 @Return				true							: ����
					false						: δ��
**********************************************************************************************************/
bool NET_PCP_Message_RecvDataisFull(void)
{
	bool MessageState;
	
	if ((NETPcpMessageRecvPark.Rear + 1) % PCP_RECV_PACK_NUM == NETPcpMessageRecvPark.Front) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			static bool NET_PCP_Message_SendDataisEmpty(void)
 @Description			NET_PCP_Message_SendDataisEmpty	: ��鷢�Ͷ����Ƿ��ѿ�
 @Input				void
 @Return				true							: �ѿ�
					false						: δ��
**********************************************************************************************************/
bool NET_PCP_Message_SendDataisEmpty(void)
{
	bool MessageState;
	
	if (NETPcpMessageSendPark.Front == NETPcpMessageSendPark.Rear) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			static bool NET_PCP_Message_RecvDataisEmpty(void)
 @Description			NET_PCP_Message_RecvDataisEmpty	: �����ն����Ƿ��ѿ�
 @Input				void
 @Return				true							: �ѿ�
					false						: δ��
**********************************************************************************************************/
bool NET_PCP_Message_RecvDataisEmpty(void)
{
	bool MessageState;
	
	if (NETPcpMessageRecvPark.Front == NETPcpMessageRecvPark.Rear) {
		MessageState = true;
	}
	else {
		MessageState = false;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			void NET_PCP_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
 @Description			NET_PCP_Message_SendDataEnqueue	: ��������д�����
 @Input				dataBuf	 		 			: ��д������
					dataLength					: ��д�����ݳ���
 @Return				void
**********************************************************************************************************/
void NET_PCP_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
{
	if ((dataBuf == NULL) || (dataLength > PCP_SEND_BUFFER_SIZE)) {
		return;
	}
	
	if (NET_PCP_Message_SendDataisFull() == true) {													//��������
		NETPcpMessageSendPark.Rear = (NETPcpMessageSendPark.Rear + 1) % PCP_SEND_PACK_NUM;					//��βƫ��1
		memset((u8 *)&NETPcpMessageSendPark.Park[NETPcpMessageSendPark.Rear], 0x0, sizeof(NETPcpMessageSendPark.Park[NETPcpMessageSendPark.Rear]));
		memcpy(NETPcpMessageSendPark.Park[NETPcpMessageSendPark.Rear].Buffer, dataBuf, dataLength);
		NETPcpMessageSendPark.Park[NETPcpMessageSendPark.Rear].Length = dataLength;
		NETPcpMessageSendPark.Front = (NETPcpMessageSendPark.Front + 1) % PCP_SEND_PACK_NUM;					//��ͷƫ��1
	}
	else {																					//����δ��
		NETPcpMessageSendPark.Rear = (NETPcpMessageSendPark.Rear + 1) % PCP_SEND_PACK_NUM;					//��βƫ��1
		memset((u8 *)&NETPcpMessageSendPark.Park[NETPcpMessageSendPark.Rear], 0x0, sizeof(NETPcpMessageSendPark.Park[NETPcpMessageSendPark.Rear]));
		memcpy(NETPcpMessageSendPark.Park[NETPcpMessageSendPark.Rear].Buffer, dataBuf, dataLength);
		NETPcpMessageSendPark.Park[NETPcpMessageSendPark.Rear].Length = dataLength;
	}
}

/**********************************************************************************************************
 @Function			void NET_PCP_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
 @Description			NET_PCP_Message_RecvDataEnqueue	: ��������д�����
 @Input				dataBuf	 		 			: ��д������
					dataLength					: ��д�����ݳ���
 @Return				void
**********************************************************************************************************/
void NET_PCP_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
{
	if ((dataBuf == NULL) || (dataLength > COAP_RECV_BUFFER_SIZE)) {
		return;
	}
	
	if (NET_PCP_Message_RecvDataisFull() == true) {													//��������
		NETPcpMessageRecvPark.Rear = (NETPcpMessageRecvPark.Rear + 1) % PCP_RECV_PACK_NUM;					//��βƫ��1
		memset((u8 *)&NETPcpMessageRecvPark.Park[NETPcpMessageRecvPark.Rear], 0x0, sizeof(NETPcpMessageRecvPark.Park[NETPcpMessageRecvPark.Rear]));
		memcpy(NETPcpMessageRecvPark.Park[NETPcpMessageRecvPark.Rear].Buffer, dataBuf, dataLength);
		NETPcpMessageRecvPark.Park[NETPcpMessageRecvPark.Rear].Length = dataLength;
		NETPcpMessageRecvPark.Front = (NETPcpMessageRecvPark.Front + 1) % PCP_RECV_PACK_NUM;					//��ͷƫ��1
	}
	else {																					//����δ��
		NETPcpMessageRecvPark.Rear = (NETPcpMessageRecvPark.Rear + 1) % PCP_RECV_PACK_NUM;					//��βƫ��1
		memset((u8 *)&NETPcpMessageRecvPark.Park[NETPcpMessageRecvPark.Rear], 0x0, sizeof(NETPcpMessageRecvPark.Park[NETPcpMessageRecvPark.Rear]));
		memcpy(NETPcpMessageRecvPark.Park[NETPcpMessageRecvPark.Rear].Buffer, dataBuf, dataLength);
		NETPcpMessageRecvPark.Park[NETPcpMessageRecvPark.Rear].Length = dataLength;
	}
}

/**********************************************************************************************************
 @Function			bool NET_PCP_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
 @Description			NET_PCP_Message_SendDataDequeue	: �������ݶ�������
 @Input				dataBuf	 		 			: ��������ݵ�ַ
					dataLength					: ��������ݳ��ȵ�ַ
 @Return				true							: δ��
					false						: �ѿ�
**********************************************************************************************************/
bool NET_PCP_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
{
	bool MessageState;
	unsigned char front;
	
	if (NET_PCP_Message_SendDataisEmpty() == true) {													//�����ѿ�
		MessageState = false;
	}
	else {																					//����δ��
		front = (NETPcpMessageSendPark.Front + 1) % PCP_SEND_PACK_NUM;									//��ͷƫ��1
		memcpy(dataBuf, NETPcpMessageSendPark.Park[front].Buffer, NETPcpMessageSendPark.Park[front].Length);
		*dataLength = NETPcpMessageSendPark.Park[front].Length;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_PCP_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
 @Description			NET_PCP_Message_RecvDataDequeue	: �������ݶ�������
 @Input				dataBuf	 		 			: ��������ݵ�ַ
					dataLength					: ��������ݳ��ȵ�ַ
 @Return				true							: δ��
					false						: �ѿ�
**********************************************************************************************************/
bool NET_PCP_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
{
	bool MessageState;
	unsigned char front;
	
	if (NET_PCP_Message_RecvDataisEmpty() == true) {													//�����ѿ�
		MessageState = false;
	}
	else {																					//����δ��
		front = (NETPcpMessageRecvPark.Front + 1) % PCP_RECV_PACK_NUM;									//��ͷƫ��1
		memcpy(dataBuf, NETPcpMessageRecvPark.Park[front].Buffer, NETPcpMessageRecvPark.Park[front].Length);
		*dataLength = NETPcpMessageRecvPark.Park[front].Length;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_PCP_Message_SendDataOffSet(void)
 @Description			NET_PCP_Message_SendDataOffSet	: �������ݶ���(����ͷƫ��1)
 @Input				void
 @Return				true							: δ��
					false						: �ѿ�
**********************************************************************************************************/
bool NET_PCP_Message_SendDataOffSet(void)
{
	bool MessageState;
	
	if (NET_PCP_Message_SendDataisEmpty() == true) {													//�����ѿ�
		MessageState = false;
	}
	else {																					//����δ��
		NETPcpMessageSendPark.Front = (NETPcpMessageSendPark.Front + 1) % PCP_SEND_PACK_NUM;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_PCP_Message_RecvDataOffSet(void)
 @Description			NET_PCP_Message_RecvDataOffSet	: �������ݶ���(����ͷƫ��1)
 @Input				void
 @Return				true							: δ��
					false						: �ѿ�
**********************************************************************************************************/
bool NET_PCP_Message_RecvDataOffSet(void)
{
	bool MessageState;
	
	if (NET_PCP_Message_RecvDataisEmpty() == true) {													//�����ѿ�
		MessageState = false;
	}
	else {																					//����δ��
		NETPcpMessageRecvPark.Front = (NETPcpMessageRecvPark.Front + 1) % PCP_RECV_PACK_NUM;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			unsigned char NET_PCP_Message_SendDataRear(void)
 @Description			NET_PCP_Message_SendDataRear		: �������ݶ�βֵ
 @Input				void
 @Return				�������ݶ�βֵ
**********************************************************************************************************/
unsigned char NET_PCP_Message_SendDataRear(void)
{
	return NETPcpMessageSendPark.Rear;
}

/**********************************************************************************************************
 @Function			unsigned char NET_PCP_Message_RecvDataRear(void)
 @Description			NET_PCP_Message_RecvDataRear		: �������ݶ�βֵ
 @Input				void
 @Return				�������ݶ�βֵ
**********************************************************************************************************/
unsigned char NET_PCP_Message_RecvDataRear(void)
{
	return NETPcpMessageRecvPark.Rear;
}

/********************************************** END OF FLEE **********************************************/

