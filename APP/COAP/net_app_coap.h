#ifndef __NET_APP_COAP_H
#define  __NET_APP_COAP_H

#include "nbiot_config.h"	

#define GPRS_DEBUG	DEBUG_PRINT

#define COAP_DEBUG_LOG_PRINT													//定义开启RF输出DEBUG信息

#define COAP_CDP_ADDR_CHINA_TELECOM_FORMAL	"117.60.157.137"					//电信生产 : "117.60.157.137"		ip753c9d89:5683
#define COAP_CDP_ADDR_CHINA_TELECOM_TEST		"180.101.147.115"					//电信测试 : "180.101.147.115"		ipb4659373:5683

#define COAP_CDP_ADDR							COAPCDPADDR_CHINA_TELECOM_FORMAL
#define COAP_CDP_PORT							5683	

#define NBIOT_COAP_BAND1					NBand_2100MHz
#define NBIOT_COAP_BAND3					NBand_1800MHz
#define NBIOT_COAP_BAND5					NBand_850MHz
#define NBIOT_COAP_BAND8					NBand_900MHz
#define NBIOT_COAP_BAND20					NBand_800MHz
#define NBIOT_COAP_BAND28					NBand_700MHz	

#define NBIOT_COAP_BAND_TYPE				NBIOT_COAP_BAND5




void NET_NBIOT_COAP_Event_HardwareReboot(NBIOT_ClientsTypeDef* pClient);

void NET_NBIOT_COAP_Event_ReportError(NBIOT_ClientsTypeDef* pClient);

void NET_NBIOT_COAP_Event_NbandModeCheck(NBIOT_ClientsTypeDef* pClient);

void NET_NBIOT_COAP_Event_NbandModeConfig(NBIOT_ClientsTypeDef* pClient);

void NET_NBIOT_COAP_Event_AttachExecute(NBIOT_ClientsTypeDef* pClient);

void NET_NBIOT_COAP_Event_AttachInquire(NBIOT_ClientsTypeDef* pClient);

void NET_NBIOT_COAP_Event_ParameterCheckOut(NBIOT_ClientsTypeDef* pClient);

void NET_NBIOT_COAP_Event_CDPServerCheck(NBIOT_ClientsTypeDef* pClient);

void NET_NBIOT_COAP_Event_SimICCIDCheck(NBIOT_ClientsTypeDef* pClient);

void NET_NBIOT_COAP_Event_CDPServerConfig(NBIOT_ClientsTypeDef* pClient);

void NET_NBIOT_COAP_Event_ModuleCheck(NBIOT_ClientsTypeDef* pClient);

void NET_NBIOT_COAP_Event_AttachCheck(NBIOT_ClientsTypeDef* pClient);

void NET_NBIOT_COAP_Event_MiscEquipConfig(NBIOT_ClientsTypeDef* pClient);

void NET_NBIOT_COAP_Event_ParameterConfig(NBIOT_ClientsTypeDef* pClient);

void NET_NBIOT_COAP_Event_SendData(NBIOT_ClientsTypeDef* pClient);

void NET_NBIOT_COAP_Event_RecvData(NBIOT_ClientsTypeDef* pClient);





#endif

