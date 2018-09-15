#ifndef __NET_PCP_APP_H
#define   __NET_PCP_APP_H

#include "sys.h"
#include "pcpconfig.h"
#include "pcpupgrade.h"
#include "pcpfunc.h"
#include "pcptransport.h"
#include "pcpcrccheck.h"
#include "pcpsock.h"
#include "nbiotconfig.h"
#include "nbiotfunc.h"
#include "nbiottransport.h"
#include "net_pcp_message_operate.h"
#include "net_coap_message_operate.h"
#include "platform_map.h"

void NET_PCP_APP_PollExecution(PCP_ClientsTypeDef* pClient);					//PCP逻辑处理
PCP_StatusTypeDef NET_PCP_NBIOT_Event_StopMode(PCP_ClientsTypeDef* pClient);		//停止模式
PCP_StatusTypeDef NET_PCP_NBIOT_Event_Initialized(PCP_ClientsTypeDef* pClient);		//初始化PCP
PCP_StatusTypeDef NET_PCP_NBIOT_Event_Ready(PCP_ClientsTypeDef* pClient);			//PCP准备中
PCP_StatusTypeDef NET_PCP_NBIOT_Event_Recv(PCP_ClientsTypeDef* pClient);			//PCP接收数据
PCP_StatusTypeDef NET_PCP_NBIOT_Event_Send(PCP_ClientsTypeDef* pClient);			//PCP发送数据
PCP_StatusTypeDef NET_PCP_NBIOT_Event_Execute(PCP_ClientsTypeDef* pClient);		//PCP数据处理
PCP_StatusTypeDef NET_PCP_NBIOT_Event_ActiveUpload(PCP_ClientsTypeDef* pClient);	//PCP主动上传数据

#endif /* __NET_PCP_APP_H */
