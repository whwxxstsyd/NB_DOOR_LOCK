#ifndef __ONENET_FUNC_H
#define   __ONENET_FUNC_H

#include "include.h"
#include "onenet_config.h"	

ONENET_StatusTypeDef NBIOT_OneNET_Related_AccessConfig_LwM2MServer(ONENET_ClientsTypeDef* pClient, u8 mode, sc8* host, u16 port);											//����LwM2M������
ONENET_StatusTypeDef NBIOT_OneNET_Related_AccessConfig_AckTimeout(ONENET_ClientsTypeDef* pClient, u8 rspTimeout);														//����Ӧ��ʱʱ��
ONENET_StatusTypeDef NBIOT_OneNET_Related_AccessConfig_ObserveAutoack(ONENET_ClientsTypeDef* pClient, u8 obsAutoack);													//����ģ���Զ�Ӧ��

ONENET_StatusTypeDef NBIOT_OneNET_Related_Create_SuiteInstance(ONENET_ClientsTypeDef* pClient, s32* refer);															//����ͨ���׼�ʵ��
ONENET_StatusTypeDef NBIOT_OneNET_Related_Delete_SuiteInstance(ONENET_ClientsTypeDef* pClient, s32 refer);															//ɾ��ͨ���׼�ʵ��
ONENET_StatusTypeDef NBIOT_OneNET_Related_CheckRead_SuiteVersion(ONENET_ClientsTypeDef* pClient);																	//���ͨ���׼��汾

ONENET_StatusTypeDef NBIOT_OneNET_Related_Add_LwM2MObject(ONENET_ClientsTypeDef* pClient, s32 refer, u32 objId, u16 insCount, sc8* insBitmap, u16 attrCount, u16 actCount);		//����LwM2M����
ONENET_StatusTypeDef NBIOT_OneNET_Related_Del_LwM2MObject(ONENET_ClientsTypeDef* pClient, s32 refer, u32 objId);														//ɾ��LwM2M����

ONENET_StatusTypeDef NBIOT_OneNET_Related_Send_RegisterRequest(ONENET_ClientsTypeDef* pClient, s32 refer, u32 lifetime, u32 timeout);										//����ע������
ONENET_StatusTypeDef NBIOT_OneNET_Related_Send_DeregisterRequest(ONENET_ClientsTypeDef* pClient, s32 refer);															//����ע������

ONENET_StatusTypeDef NBIOT_OneNET_Related_Respond_DiscoverRequest(ONENET_ClientsTypeDef* pClient, s32 refer, u32 msgId, u8 result, u16 length, sc8* valuestring, sc8* raiMode);	//��ӦDiscover����
ONENET_StatusTypeDef NBIOT_OneNET_Related_Respond_ObserveRequest(ONENET_ClientsTypeDef* pClient, s32 refer, u32 msgId, u8 result, sc8* raiMode);								//��ӦObserve����
	












#endif /* __ONENET_FUNC_H */

