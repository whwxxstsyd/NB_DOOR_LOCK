#ifndef __PCP_TRANSPORT_H
#define   __PCP_TRANSPORT_H

#include "sys.h"
#include "pcpconfig.h"

PCP_StatusTypeDef PCP_Transport_Write(PCP_CoAPNetTransportTypeDef* NetCoAP, const char *buf, u16 sendlen);			//CoAP发送一条负载数据
PCP_StatusTypeDef PCP_Transport_Read(PCP_CoAPNetTransportTypeDef* NetCoAP, char *buf, u16* recvlen);				//CoAP读取一条负载数据

PCP_StatusTypeDef PCP_Transport_Init(PCP_CoAPNetTransportTypeDef* NetCoAP, NBIOT_ClientsTypeDef* NBIotStack);			//初始化PCP数据传输接口

#endif /* __PCP_TRANSPORT_H */
