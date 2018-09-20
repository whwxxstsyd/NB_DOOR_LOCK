#ifndef __NET_ONENET_APP_H
#define __NET_ONENET_APP_H
	
#include "sys.h"
#include "onenetconfig.h"
#include "onenetfunc.h"
#include "onenettransport.h"
#include "nbiotconfig.h"
#include "nbiotfunc.h"
#include "nbiottransport.h"
#include "net_onenet_message_operate.h"
#include "platform_map.h"

#define ONENET_DEBUG_LOG_RF_PRINT											//���忪��RF���DEBUG��Ϣ
	


#define ONENET_NBIOT_BAND1				NBand_2100MHz
#define ONENET_NBIOT_BAND3				NBand_1800MHz
#define ONENET_NBIOT_BAND5				NBand_850MHz
#define ONENET_NBIOT_BAND8				NBand_900MHz
#define ONENET_NBIOT_BAND20				NBand_800MHz
#define ONENET_NBIOT_BAND28				NBand_700MHz
#define ONENET_NBIOT_BAND_TYPE			ONENET_NBIOT_BAND8






void NET_ONENET_APP_PollExecution(ONENET_ClientsTypeDef* pClient);				//OneNET�߼�����
void NET_ONENET_NBIOT_Event_StopMode(ONENET_ClientsTypeDef* pClient);				//ֹͣģʽ
void NET_ONENET_NBIOT_Event_HardwareReboot(ONENET_ClientsTypeDef* pClient);		//Ӳ������
void NET_ONENET_NBIOT_Event_ReportError(ONENET_ClientsTypeDef* pClient);			//���������
void NET_ONENET_NBIOT_Event_ModuleCheck(ONENET_ClientsTypeDef* pClient);			//ģ����
void NET_ONENET_NBIOT_Event_ParameterConfig(ONENET_ClientsTypeDef* pClient);		//��������
void NET_ONENET_NBIOT_Event_SimICCIDCheck(ONENET_ClientsTypeDef* pClient);			//Sim�����
void NET_ONENET_NBIOT_Event_FullFunctionality(ONENET_ClientsTypeDef* pClient);		//��������
void NET_ONENET_NBIOT_Event_MinimumFunctionality(ONENET_ClientsTypeDef* pClient);	//��С����
void NET_ONENET_NBIOT_Event_NbandModeCheck(ONENET_ClientsTypeDef* pClient);		//NBANDģʽ��ѯ
void NET_ONENET_NBIOT_Event_NbandModeConfig(ONENET_ClientsTypeDef* pClient);		//NBANDģʽ����
void NET_ONENET_NBIOT_Event_AttachCheck(ONENET_ClientsTypeDef* pClient);			//ע�����
void NET_ONENET_NBIOT_Event_AttachExecute(ONENET_ClientsTypeDef* pClient);			//ע������
void NET_ONENET_NBIOT_Event_AttachInquire(ONENET_ClientsTypeDef* pClient);			//ע����ѯ
void NET_ONENET_NBIOT_Event_ParameterCheckOut(ONENET_ClientsTypeDef* pClient);		//�������

void NET_ONENET_APP_ProcessExecution(ONENET_ClientsTypeDef* pClient);				//OneNETЭ���߼�����
void NET_ONENET_Event_Init(ONENET_ClientsTypeDef* pClient);						//INIT











#endif /* __NET_ONENET_APP_H */

