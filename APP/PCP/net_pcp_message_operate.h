#ifndef __NET_PCP_MESSAGE_OPERATE_H
#define   __NET_PCP_MESSAGE_OPERATE_H

#include "sys.h"

#define PCP_SEND_BUFFER_SIZE				128
#define PCP_RECV_BUFFER_SIZE				512
#define PCP_SEND_PACK_NUM				5
#define PCP_RECV_PACK_NUM				5

/* 发送数据队列 */
typedef struct
{
	unsigned char						Front;
	unsigned char						Rear;
	
	struct PCPSendParkTypeDef
	{
		unsigned char					Buffer[PCP_SEND_BUFFER_SIZE];
		unsigned short					Length;
	}Park[PCP_SEND_PACK_NUM];
}PCP_SwapSendDataTypeDef;

/* 接收数据队列 */
typedef struct
{
	unsigned char						Front;
	unsigned char						Rear;
	
	struct PCPRecvParkTypeDef
	{
		unsigned char					Buffer[PCP_RECV_BUFFER_SIZE];
		unsigned short					Length;
	}Park[PCP_RECV_PACK_NUM];
}PCP_SwapRecvDataTypeDef;


bool NET_PCP_Message_SendDataisFull(void);																//检查发送队列是否已满
bool NET_PCP_Message_RecvDataisFull(void);																//检查接收队列是否已满
bool NET_PCP_Message_SendDataisEmpty(void);																//检查发送队列是否已空
bool NET_PCP_Message_RecvDataisEmpty(void);																//检查接收队列是否已空
void NET_PCP_Message_SendDataEnqueue(unsigned char* dataBuf, unsigned short dataLength);							//发送数据写入队列
void NET_PCP_Message_RecvDataEnqueue(unsigned char* dataBuf, unsigned short dataLength);							//接收数据写入队列
bool NET_PCP_Message_SendDataDequeue(unsigned char* dataBuf, unsigned short* dataLength);							//发送数据读出队列
bool NET_PCP_Message_RecvDataDequeue(unsigned char* dataBuf, unsigned short* dataLength);							//接收数据读出队列
bool NET_PCP_Message_SendDataOffSet(void);																//发送数据队列(队列头偏移1)
bool NET_PCP_Message_RecvDataOffSet(void);																//接收数据队列(队列头偏移1)
unsigned char NET_PCP_Message_SendDataRear(void);															//发送数据队尾值
unsigned char NET_PCP_Message_RecvDataRear(void);															//接收数据队尾值

#endif /* __NET_PCP_MESSAGE_OPERATE_H */
