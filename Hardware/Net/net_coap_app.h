#ifndef __NET_COAP_APP_H
#define   __NET_COAP_APP_H

#include "include.h"
#include "nbiotconfig.h"
#include "nbiotfunc.h"
#include "nbiottransport.h"
#include "net_coap_message_operate.h"
#include "net_pcp_message_operate.h"
#include "platform_map.h"
	
#define COAP_DEBUG_LOG_RF_PRINT													//���忪��RF���DEBUG��Ϣ
	
#define COAPCDPADDR_CHINA_TELECOM_FORMAL		"117.60.157.137"					//�������� : "117.60.157.137"		ip753c9d89:5683
#define COAPCDPADDR_CHINA_TELECOM_TEST		"180.101.147.115"					//���Ų��� : "180.101.147.115"		ipb4659373:5683
#define COAPCDPADDR								COAPCDPADDR_CHINA_TELECOM_FORMAL
#define COAPCDPPORT								5683	
	
#define COAP_NBIOT_BAND1					NBand_2100MHz
#define COAP_NBIOT_BAND3					NBand_1800MHz
#define COAP_NBIOT_BAND5					NBand_850MHz
#define COAP_NBIOT_BAND8					NBand_900MHz
#define COAP_NBIOT_BAND20					NBand_800MHz
#define COAP_NBIOT_BAND28					NBand_700MHz	
#define COAP_NBIOT_BAND_TYPE				COAP_NBIOT_BAND5

void NET_COAP_APP_PollExecution(NBIOT_ClientsTypeDef* pClient);					//COAP�߼�����
void NET_COAP_NBIOT_Event_StopMode(NBIOT_ClientsTypeDef* pClient);				//ֹͣģʽ
void NET_COAP_NBIOT_Event_HardwareReboot(NBIOT_ClientsTypeDef* pClient);			//Ӳ������
void NET_COAP_NBIOT_Event_ReportError(NBIOT_ClientsTypeDef* pClient);				//���������
void NET_COAP_NBIOT_Event_ModuleCheck(NBIOT_ClientsTypeDef* pClient);				//ģ����
void NET_COAP_NBIOT_Event_ParameterConfig(NBIOT_ClientsTypeDef* pClient);			//��������
void NET_COAP_NBIOT_Event_SimICCIDCheck(NBIOT_ClientsTypeDef* pClient);			//Sim�����
void NET_COAP_NBIOT_Event_FullFunctionality(NBIOT_ClientsTypeDef* pClient);		//��������
void NET_COAP_NBIOT_Event_MinimumFunctionality(NBIOT_ClientsTypeDef* pClient);		//��С����
void NET_COAP_NBIOT_Event_CDPServerCheck(NBIOT_ClientsTypeDef* pClient);			//CDP��������ѯ
void NET_COAP_NBIOT_Event_CDPServerConfig(NBIOT_ClientsTypeDef* pClient);			//CDP����������
void NET_COAP_NBIOT_Event_NbandModeCheck(NBIOT_ClientsTypeDef* pClient);			//NBANDģʽ��ѯ
void NET_COAP_NBIOT_Event_NbandModeConfig(NBIOT_ClientsTypeDef* pClient);			//NBANDģʽ����
void NET_COAP_NBIOT_Event_MiscEquipConfig(NBIOT_ClientsTypeDef* pClient);			//��������
void NET_COAP_NBIOT_Event_AttachCheck(NBIOT_ClientsTypeDef* pClient);				//ע�����
void NET_COAP_NBIOT_Event_AttachExecute(NBIOT_ClientsTypeDef* pClient);			//ע������
void NET_COAP_NBIOT_Event_AttachInquire(NBIOT_ClientsTypeDef* pClient);			//ע����ѯ
void NET_COAP_NBIOT_Event_ParameterCheckOut(NBIOT_ClientsTypeDef* pClient);		//�������
void NET_COAP_NBIOT_Event_SendData(NBIOT_ClientsTypeDef* pClient);				//��������
void NET_COAP_NBIOT_Event_RecvData(NBIOT_ClientsTypeDef* pClient);				//��������
void NET_COAP_NBIOT_Event_SendDataRANormal(NBIOT_ClientsTypeDef* pClient);			//��������RANormal
void NET_COAP_NBIOT_Event_RecvDataRANormal(NBIOT_ClientsTypeDef* pClient);			//��������RANormal
void NET_COAP_NBIOT_Event_ExecutDownlinkData(NBIOT_ClientsTypeDef* pClient);		//�������ݴ���

void NET_COAP_Listen_PollExecution(NBIOT_ClientsTypeDef* pClient);				//COAP����������
#if NBCOAP_LISTEN_IDLE_TYPE == NBCOAP_LISTEN_IDLE_ENABLE
void NET_COAP_NBIOT_Listen_Enable_EnterIdleMode(NBIOT_ClientsTypeDef* pClient);		//ʹ��(����IDLEģʽ)����
void NET_COAP_NBIOT_Listen_Event_EnterIdleMode(NBIOT_ClientsTypeDef* pClient);		//�¼�(����IDLEģʽ)����
#endif
#if NBCOAP_LISTEN_PARAMETER_TYPE == NBCOAP_LISTEN_PARAMETER_ENABLE
void NET_COAP_NBIOT_Listen_Enable_EnterParameter(NBIOT_ClientsTypeDef* pClient);	//ʹ��(����NBIOT������Ϣ)����
void NET_COAP_NBIOT_Listen_Event_EnterParameter(NBIOT_ClientsTypeDef* pClient);		//�¼�(����NBIOT������Ϣ)����
#endif

#endif /* __NET_COAP_APP_H */

