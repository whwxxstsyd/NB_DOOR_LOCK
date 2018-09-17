#include "net_nbiot_task.h"


	
static NET_NBIOT_ClientsTypeDef NetNbiotClientHandler;							//NET NBIOT Clinet Handler



static void NET_NBIOT_Client_Init(NET_NBIOT_ClientsTypeDef* pClient)
{
#if NETPROTOCAL == NETCOAP

	pClient->PollExecution			= NET_POLL_EXECUTION_COAP;

#elif NETPROTOCAL == NETMQTTSN

	pClient->PollExecution			= NET_POLL_EXECUTION_DNS;

#elif NETPROTOCAL == NETONENET
	
	pClient->PollExecution			= NET_POLL_EXECUTION_ONENET;

#endif
}

	

static void Net_NBIoT_DataProcessing(NET_NBIOT_ClientsTypeDef* pClient)
{

}


		
static void Net_NBIoT_TaskProcessing(NET_NBIOT_ClientsTypeDef* pClient)
{

}





void NET_NBIOT_Initialization(void)
{
	/* NET NBIOT客户端初始化 */
	NET_NBIOT_Client_Init(&NetNbiotClientHandler);

	/* NBIOT数据传输接口初始化 */
	NBIOT_Transport_Init(&NbiotATCmdHandler);
	
	/* NBIOT客户端初始化 */
	NBIOT_Client_Init(&NbiotClientHandler, &NbiotATCmdHandler, &NetNbiotClientHandler);
	
#ifndef NETPROTOCAL
	#error No Define NETPROTOCAL!
#else

#if (NETPROTOCAL == NETCOAP)
	
	/* PCP数据传输接口初始化 */
	PCP_Transport_Init(&PCPCoAPNetHandler, &NbiotClientHandler);

	/* PCP客户端初始化 */
	PCP_Client_Init(&PCPClientHandler, &PCPCoAPNetHandler, &NetNbiotClientHandler);
	
#elif (NETPROTOCAL == NETMQTTSN)
	

#elif (NETPROTOCAL == NETONENET)
	

#else
	#error NETPROTOCAL Define Error
#endif
	
#endif
}





void Net_NBIoT_Task(void)				
{
	Net_NBIoT_DataProcessing(&NetNbiotClientHandler);								
		
	Net_NBIoT_TaskProcessing(&NetNbiotClientHandler);						
}

