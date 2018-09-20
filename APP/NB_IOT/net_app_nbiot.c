#include "net_app_nbiot.h"
#include "net_app_coap.h"
//#include "net_app_onenet.h"	
//#include "net_app_pcp.h"				

#include "net_platform_config.h"	

	
//当前的使用的协议类型
static NET_NBIOT_ClientsTypeDef NetNbiotClientHandler;			//NET NBIOT Clinet Handler



static void NET_NBIOT_Client_Init(NET_NBIOT_ClientsTypeDef* pClientType)
{
#if NETPROTOCAL == NETCOAP	

	pClientType->PollExecution	= NET_POLL_EXECUTION_COAP;

#elif NETPROTOCAL == NETMQTTSN	

	pClientType->PollExecution	= NET_POLL_EXECUTION_DNS;

#elif NETPROTOCAL == NETONENET

	pClientType->PollExecution	= NET_POLL_EXECUTION_ONENET;

#endif
}

	

static void Net_NBIoT_DataProcessing(NET_NBIOT_ClientsTypeDef* pClient)
{
	
}


		
static void Net_NBIoT_TaskProcessing(NET_NBIOT_ClientsTypeDef* pClientType)
{	
	/* NBIOT PollExecution */
#if NETPROTOCAL == NETCOAP
	
	switch (pClientType->PollExecution)
	{
		case NET_POLL_EXECUTION_COAP:	
			NET_APP_COAP_PollExecution(&NbiotClientHandler);
			break;
		
		case NET_POLL_EXECUTION_DNS:
			pClientType->PollExecution = NET_POLL_EXECUTION_COAP;
			break;
		
		case NET_POLL_EXECUTION_MQTTSN:
			pClientType->PollExecution = NET_POLL_EXECUTION_COAP;
			break;
		
		case NET_POLL_EXECUTION_PCP:	
			pClientType->PollExecution = NET_POLL_EXECUTION_COAP;
			//NET_APP_PCP_PollExecution(&PCPClientHandler);		
			break;	
		
		case NET_POLL_EXECUTION_ONENET:
			pClientType->PollExecution = NET_POLL_EXECUTION_COAP;
			break;
	}
	
#elif NETPROTOCAL == NETMQTTSN
	

	
#elif NETPROTOCAL == NETONENET
	
	switch (pClientType->PollExecution)
	{	
		case NET_POLL_EXECUTION_COAP:
			pClientType->PollExecution = NET_POLL_EXECUTION_ONENET;
			break;
		
		case NET_POLL_EXECUTION_DNS:
			pClientType->PollExecution = NET_POLL_EXECUTION_ONENET;
			break;
		
		case NET_POLL_EXECUTION_MQTTSN:
			pClientType->PollExecution = NET_POLL_EXECUTION_ONENET;
			break;
		
		case NET_POLL_EXECUTION_PCP:
			pClientType->PollExecution = NET_POLL_EXECUTION_ONENET;
			break;	
		
		case NET_POLL_EXECUTION_ONENET:	
			NET_APP_ONENET_PollExecution(&OneNETClientHandler);
			break;
	}
	
#endif
}






void NET_NBIOT_Initialization(void)
{
	/* NET NBIOT客户端初始化 */
	//NET_NBIOT_Client_Init(&NetNbiotClientHandler);
		
	/* NBIOT数据传输接口初始化 */
	//NBIOT_Transport_Init(&NbiotATCmdHandler);
		
	/* NBIOT客户端初始化 */
	//NBIOT_Client_Init(&NbiotClientHandler, &NbiotATCmdHandler, &NetNbiotClientHandler);
	
#ifndef NETPROTOCAL
	#error No Define NETPROTOCAL!
#else

#if (NETPROTOCAL == NETCOAP)
	
	/* PCP数据传输接口初始化 */
	//PCP_Transport_Init(&PCPCoAPNetHandler, &NbiotClientHandler);
	/* PCP客户端初始化 */
	//PCP_Client_Init(&PCPClientHandler, &PCPCoAPNetHandler, &NetNbiotClientHandler);
	
#elif (NETPROTOCAL == NETMQTTSN)
	
	
#elif (NETPROTOCAL == NETONENET)
	
	/* ONENET数据传输接口初始化 */
	ONENET_Transport_Init(&OneNETLWM2MNetHandler, &NbiotClientHandler);
	/* ONENET客户端初始化 */
	OneNET_Client_Init(&OneNETClientHandler, &OneNETLWM2MNetHandler, &NetNbiotClientHandler);
	
#else
#error NETPROTOCAL Define Error
#endif

#endif
}







void NET_App_NBIOT_Task(void)					
{
	Net_NBIoT_DataProcessing(&NetNbiotClientHandler);								
		
	Net_NBIoT_TaskProcessing(&NetNbiotClientHandler);						
}



