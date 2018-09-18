/**
  *********************************************************************************************************
  * @file    onenettransport.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-08-31
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "onenettransport.h"
#include "platform_config.h"
#include "platform_map.h"
#include "stm32l1xx_config.h"
#include "nbiotfunc.h"
#include "string.h"













/**********************************************************************************************************
 @Function			ONENET_StatusTypeDef ONENET_Transport_Init(ONENET_LWM2MTransportTypeDef* NetLWM2M, NBIOT_ClientsTypeDef* NBIotStack)
 @Description			ONENET_Transport_Init	: Initiative初始化ONENET数据传输接口
 @Input				NetLWM2M				: LWM2M Struct*
					NBIotStack			: NBIot协议栈
 @Return				ONENET_StatusTypeDef	: ONENET处理状态
**********************************************************************************************************/
ONENET_StatusTypeDef ONENET_Transport_Init(ONENET_LWM2MTransportTypeDef* NetLWM2M, NBIOT_ClientsTypeDef* NBIotStack)
{
	ONENET_StatusTypeDef OneNETStatus = ONENET_OK;
	
	NetLWM2M->NBIotStack = NBIotStack;
	
	
	
	
	return OneNETStatus;
}

/********************************************** END OF FLEE **********************************************/
