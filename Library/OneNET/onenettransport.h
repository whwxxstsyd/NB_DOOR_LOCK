#ifndef __ONENET_TRANSPORT_H
#define   __ONENET_TRANSPORT_H

#include "sys.h"
#include "onenetconfig.h"











ONENET_StatusTypeDef ONENET_Transport_Init(ONENET_LWM2MTransportTypeDef* NetLWM2M, NBIOT_ClientsTypeDef* NBIotStack);	//初始化PCP数据传输接口

#endif /* __ONENET_TRANSPORT_H */
