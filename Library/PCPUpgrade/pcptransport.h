#ifndef __PCP_TRANSPORT_H
#define   __PCP_TRANSPORT_H

#include "include.h"
#include "pcpconfig.h"

PCP_StatusTypeDef PCP_Transport_Write(PCP_CoAPNetTransportTypeDef* NetCoAP, const char *buf, u16 sendlen);			//CoAP����һ����������
PCP_StatusTypeDef PCP_Transport_Read(PCP_CoAPNetTransportTypeDef* NetCoAP, char *buf, u16* recvlen);				//CoAP��ȡһ����������

PCP_StatusTypeDef PCP_Transport_Init(PCP_CoAPNetTransportTypeDef* NetCoAP, NBIOT_ClientsTypeDef* NBIotStack);			//��ʼ��PCP���ݴ���ӿ�

#endif /* __PCP_TRANSPORT_H */

