#ifndef __NBIOT_TRANSPORT_H
#define   __NBIOT_TRANSPORT_H

#include "sys.h"
#include "nbiotconfig.h"

NBIOT_StatusTypeDef NBIOT_Transport_SendATCmd(NBIOT_ATCmdTypeDef* ATCmd);											//����ATָ��ȴ�Ӧ��
NBIOT_StatusTypeDef NBIOT_Transport_RecvATCmd(NBIOT_ATCmdTypeDef* ATCmd);											//����ATָ��Ӧ��
NBIOT_StatusTypeDef NBIOT_Transport_Init(NBIOT_ATCmdTypeDef* ATCmd);												//��ʼ��NBIOT���ݴ���ӿ�

#endif /* __NBIOT_TRANSPORT_H */

