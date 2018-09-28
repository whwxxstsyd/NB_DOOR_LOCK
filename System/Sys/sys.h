#ifndef __SYS_H
#define   __SYS_H

#include "bsp.h"	


typedef struct
{		
	u32	xTicksToWait;
	u32	xTimeOut;
}Stm8_CalculagraphTypeDef;													//计时器

typedef struct
{	
	bool bEventRunningState;
	u16	xEventRunningStartTime;
	u16	xEventRunningEndTime;
}Stm8_EventRunningTimeTypeDef;												//事件运行时间器

typedef struct
{
	u16	seconds;												//秒
	u16	tenseconds;												//10秒
	u16	minutes;												//分	
	u16	fifteenMinutes;										//15分钟
	u16	fortyMinutes;											//40分钟
	u16	hours;													//时
	u16	twoHours;												//2小时
	u16	fourHours;												//4小时
	u16	days;													//天
}Stm8_SystemRunningTime;													//系统运行时间
	
extern __IO u32 SystemRunningTimesSecond;										//系统运行时间(S)
extern __IO u32 SystemSoftResetTime;											//系统运行超时软件复位时间(MS)
extern Stm8_SystemRunningTime	SystemRunningTime;								//系统运行时间(结构体)



void Stm8_IncSecondTick(void);												//运行时间加1秒
u32  Stm8_GetSecondTick(void);												//获取运行秒数

void Stm8_Calculagraph_Init(Stm8_CalculagraphTypeDef* timer);						//初始化计时器
void Stm8_Calculagraph_CountdownMS(Stm8_CalculagraphTypeDef* timer, u32 timeout_ms);	//配置计时器倒数时间(MS)
bool Stm8_Calculagraph_IsExpiredMS(Stm8_CalculagraphTypeDef* timer);				//查询是否到达计时器计时时间(MS)
void Stm8_Calculagraph_CountdownSec(Stm8_CalculagraphTypeDef* timer, u32 timeout_sec);	//配置计时器倒数时间(S)
bool Stm8_Calculagraph_IsExpiredSec(Stm8_CalculagraphTypeDef* timer);				//查询是否到达计时器计时时间(S)

void Stm8_EventRunningTime_Init(Stm8_EventRunningTimeTypeDef* timer);				//初始化事件运行时间器
void Stm8_EventRunningTime_StartMS(Stm8_EventRunningTimeTypeDef* timer);				//开启事件运行时间器(MS)
u16 Stm8_EventRunningTime_EndMS(Stm8_EventRunningTimeTypeDef* timer);		//结束事件运行时间器(MS)
void Stm8_EventRunningTime_StartSec(Stm8_EventRunningTimeTypeDef* timer);			//开启事件运行时间器(S)
u16 Stm8_EventRunningTime_EndSec(Stm8_EventRunningTimeTypeDef* timer);		//结束事件运行时间器(S)


#endif

