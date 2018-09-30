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
 @Description			NET_PCP_Message_SendDataisFull	: 检查发送队列是否已满
 @Input				void
 @Return				true							: 已满
					false						: 未满
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
 @Description			NET_PCP_Message_RecvDataisFull	: 检查接收队列是否已满
 @Input				void
 @Return				true							: 已满
					false						: 未满
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
 @Description			NET_PCP_Message_SendDataisEmpty	: 检查发送队列是否已空
 @Input				void
 @Return				true							: 已空
					false						: 未空
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
 @Description			NET_PCP_Message_RecvDataisEmpty	: 检查接收队列是否已空
 @Input				void
 @Return				true							: 已空
					false						: 未空
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
 @Description			NET_PCP_Message_SendDataEnqueue	: 发送数据写入队列
 @Input				dataBuf	 		 			: 需写入数据
					dataLength					: 需写入数据长度
 @Return				void
**********************************************************************************************************/
void NET_PCP_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
{
	if ((dataBuf == NULL) || (dataLength > PCP_SEND_BUFFER_SIZE)) {
		return;
	}
	
	NETPcpMessageSendPark.Rear = (NETPcpMessageSendPark.Rear + 1) % PCP_SEND_PACK_NUM;						//队尾偏移1
	memset((u8 *)&NETPcpMessageSendPark.Park[NETPcpMessageSendPark.Rear], 0x0, sizeof(NETPcpMessageSendPark.Park[NETPcpMessageSendPark.Rear]));
	memcpy(NETPcpMessageSendPark.Park[NETPcpMessageSendPark.Rear].Buffer, dataBuf, dataLength);
	NETPcpMessageSendPark.Park[NETPcpMessageSendPark.Rear].Length = dataLength;
	
	if (NET_PCP_Message_SendDataisFull() == true) {													//队列已满
		NETPcpMessageSendPark.Front = (NETPcpMessageSendPark.Front + 1) % PCP_SEND_PACK_NUM;					//队头偏移1
	}
}

/**********************************************************************************************************
 @Function			void NET_PCP_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
 @Description			NET_PCP_Message_RecvDataEnqueue	: 接收数据写入队列
 @Input				dataBuf	 		 			: 需写入数据
					dataLength					: 需写入数据长度
 @Return				void
**********************************************************************************************************/
void NET_PCP_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength)
{
	if ((dataBuf == NULL) || (dataLength > COAP_RECV_BUFFER_SIZE)) {
		return;
	}
	
	NETPcpMessageRecvPark.Rear = (NETPcpMessageRecvPark.Rear + 1) % PCP_RECV_PACK_NUM;						//队尾偏移1
	memset((u8 *)&NETPcpMessageRecvPark.Park[NETPcpMessageRecvPark.Rear], 0x0, sizeof(NETPcpMessageRecvPark.Park[NETPcpMessageRecvPark.Rear]));
	memcpy(NETPcpMessageRecvPark.Park[NETPcpMessageRecvPark.Rear].Buffer, dataBuf, dataLength);
	NETPcpMessageRecvPark.Park[NETPcpMessageRecvPark.Rear].Length = dataLength;
	
	if (NET_PCP_Message_RecvDataisFull() == true) {													//队列已满
		NETPcpMessageRecvPark.Front = (NETPcpMessageRecvPark.Front + 1) % PCP_RECV_PACK_NUM;					//队头偏移1
	}
}

/**********************************************************************************************************
 @Function			bool NET_PCP_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
 @Description			NET_PCP_Message_SendDataDequeue	: 发送数据读出队列
 @Input				dataBuf	 		 			: 需读出数据地址
					dataLength					: 需读出数据长度地址
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_PCP_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
{
	bool MessageState;
	unsigned char front;
	
	if (NET_PCP_Message_SendDataisEmpty() == true) {													//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		front = (NETPcpMessageSendPark.Front + 1) % PCP_SEND_PACK_NUM;									//队头偏移1
		memcpy(dataBuf, NETPcpMessageSendPark.Park[front].Buffer, NETPcpMessageSendPark.Park[front].Length);
		*dataLength = NETPcpMessageSendPark.Park[front].Length;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_PCP_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
 @Description			NET_PCP_Message_RecvDataDequeue	: 接收数据读出队列
 @Input				dataBuf	 		 			: 需读出数据地址
					dataLength					: 需读出数据长度地址
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_PCP_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength)
{
	bool MessageState;
	unsigned char front;
	
	if (NET_PCP_Message_RecvDataisEmpty() == true) {													//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		front = (NETPcpMessageRecvPark.Front + 1) % PCP_RECV_PACK_NUM;									//队头偏移1
		memcpy(dataBuf, NETPcpMessageRecvPark.Park[front].Buffer, NETPcpMessageRecvPark.Park[front].Length);
		*dataLength = NETPcpMessageRecvPark.Park[front].Length;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_PCP_Message_SendDataOffSet(void)
 @Description			NET_PCP_Message_SendDataOffSet	: 发送数据队列(队列头偏移1)
 @Input				void
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_PCP_Message_SendDataOffSet(void)
{
	bool MessageState;
	
	if (NET_PCP_Message_SendDataisEmpty() == true) {													//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		NETPcpMessageSendPark.Front = (NETPcpMessageSendPark.Front + 1) % PCP_SEND_PACK_NUM;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			bool NET_PCP_Message_RecvDataOffSet(void)
 @Description			NET_PCP_Message_RecvDataOffSet	: 接收数据队列(队列头偏移1)
 @Input				void
 @Return				true							: 未空
					false						: 已空
**********************************************************************************************************/
bool NET_PCP_Message_RecvDataOffSet(void)
{
	bool MessageState;
	
	if (NET_PCP_Message_RecvDataisEmpty() == true) {													//队列已空
		MessageState = false;
	}
	else {																					//队列未空
		NETPcpMessageRecvPark.Front = (NETPcpMessageRecvPark.Front + 1) % PCP_RECV_PACK_NUM;
		MessageState = true;
	}
	
	return MessageState;
}

/**********************************************************************************************************
 @Function			unsigned char NET_PCP_Message_SendDataRear(void)
 @Description			NET_PCP_Message_SendDataRear		: 发送数据队尾值
 @Input				void
 @Return				发送数据队尾值
**********************************************************************************************************/
unsigned char NET_PCP_Message_SendDataRear(void)
{
	return NETPcpMessageSendPark.Rear;
}

/**********************************************************************************************************
 @Function			unsigned char NET_PCP_Message_RecvDataRear(void)
 @Description			NET_PCP_Message_RecvDataRear		: 接收数据队尾值
 @Input				void
 @Return				接收数据队尾值
**********************************************************************************************************/
unsigned char NET_PCP_Message_RecvDataRear(void)
{
	return NETPcpMessageRecvPark.Rear;
}

/********************************************** END OF FLEE **********************************************/
