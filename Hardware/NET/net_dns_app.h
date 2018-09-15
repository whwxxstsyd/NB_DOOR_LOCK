#ifndef __NET_DNS_APP_H
#define   __NET_DNS_APP_H

#include "sys.h"
#include "dnsconfig.h"
#include "dnsfunc.h"
#include "dnstransport.h"
#include "nbiotconfig.h"
#include "nbiotfunc.h"
#include "nbiottransport.h"
#include "mqttsnconfig.h"
#include "mqttsnfunc.h"
#include "mqttsntransport.h"
#include "net_mqttsn_message_operate.h"
#include "net_coap_message_operate.h"
#include "platform_map.h"
#include "platform_config.h"

#define DNS_DEBUG_LOG_RF_PRINT											//定义开启RF输出DEBUG信息

#define DNS_NBIOT_BAND1					NBand_2100MHz
#define DNS_NBIOT_BAND3					NBand_1800MHz
#define DNS_NBIOT_BAND5					NBand_850MHz
#define DNS_NBIOT_BAND8					NBand_900MHz
#define DNS_NBIOT_BAND20					NBand_800MHz
#define DNS_NBIOT_BAND28					NBand_700MHz
#define DNS_NBIOT_BAND_TYPE				DNS_NBIOT_BAND8

void NET_DNS_APP_PollExecution(DNS_ClientsTypeDef* pClient);					//DNS逻辑处理
void NET_DNS_NBIOT_Event_StopMode(DNS_ClientsTypeDef* pClient);					//停止模式
void NET_DNS_NBIOT_Event_HardwareReboot(DNS_ClientsTypeDef* pClient);				//硬件重启
void NET_DNS_NBIOT_Event_ReportError(DNS_ClientsTypeDef* pClient);				//错误码输出
void NET_DNS_NBIOT_Event_ModuleCheck(DNS_ClientsTypeDef* pClient);				//模块检测
void NET_DNS_NBIOT_Event_ParameterConfig(DNS_ClientsTypeDef* pClient);			//参数配置
void NET_DNS_NBIOT_Event_SimICCIDCheck(DNS_ClientsTypeDef* pClient);				//Sim卡检测
void NET_DNS_NBIOT_Event_FullFunctionality(DNS_ClientsTypeDef* pClient);			//完整功能
void NET_DNS_NBIOT_Event_MinimumFunctionality(DNS_ClientsTypeDef* pClient);		//最小功能
void NET_DNS_NBIOT_Event_NbandModeCheck(DNS_ClientsTypeDef* pClient);				//NBAND模式查询
void NET_DNS_NBIOT_Event_NbandModeConfig(DNS_ClientsTypeDef* pClient);			//NBAND模式配置
void NET_DNS_NBIOT_Event_AttachCheck(DNS_ClientsTypeDef* pClient);				//注网检查
void NET_DNS_NBIOT_Event_AttachExecute(DNS_ClientsTypeDef* pClient);				//注网进行
void NET_DNS_NBIOT_Event_AttachInquire(DNS_ClientsTypeDef* pClient);				//注网查询
void NET_DNS_NBIOT_Event_ParameterCheckOut(DNS_ClientsTypeDef* pClient);			//参数检出

void NET_DNS_APP_ProcessExecution(DNS_ClientsTypeDef* pClient);					//DNS协议逻辑处理
void NET_DNS_Event_CreatUDPSocket(DNS_ClientsTypeDef* pClient);					//CreatUDPSocket
void NET_DNS_Event_SendDnsStructData(DNS_ClientsTypeDef* pClient);				//SendDnsStructData
void NET_DNS_Event_RecvDnsStructData(DNS_ClientsTypeDef* pClient);				//RecvDnsStructData
void NET_DNS_Event_CloseUDPSocket(DNS_ClientsTypeDef* pClient);					//CloseUDPSocket
void NET_DNS_Event_OverDnsAnalysis(DNS_ClientsTypeDef* pClient);					//OverDnsAnalysis

#endif /* __NET_DNS_APP_H */
