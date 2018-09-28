#ifndef __SYS_H
#define   __SYS_H

#include "bsp.h"	


typedef struct
{		
	u32	xTicksToWait;
	u32	xTimeOut;
}Stm8_CalculagraphTypeDef;													//��ʱ��

typedef struct
{	
	bool bEventRunningState;
	u16	xEventRunningStartTime;
	u16	xEventRunningEndTime;
}Stm8_EventRunningTimeTypeDef;												//�¼�����ʱ����

typedef struct
{
	u16	seconds;												//��
	u16	tenseconds;												//10��
	u16	minutes;												//��	
	u16	fifteenMinutes;										//15����
	u16	fortyMinutes;											//40����
	u16	hours;													//ʱ
	u16	twoHours;												//2Сʱ
	u16	fourHours;												//4Сʱ
	u16	days;													//��
}Stm8_SystemRunningTime;													//ϵͳ����ʱ��
	
extern __IO u32 SystemRunningTimesSecond;										//ϵͳ����ʱ��(S)
extern __IO u32 SystemSoftResetTime;											//ϵͳ���г�ʱ�����λʱ��(MS)
extern Stm8_SystemRunningTime	SystemRunningTime;								//ϵͳ����ʱ��(�ṹ��)



void Stm8_IncSecondTick(void);												//����ʱ���1��
u32  Stm8_GetSecondTick(void);												//��ȡ��������

void Stm8_Calculagraph_Init(Stm8_CalculagraphTypeDef* timer);						//��ʼ����ʱ��
void Stm8_Calculagraph_CountdownMS(Stm8_CalculagraphTypeDef* timer, u32 timeout_ms);	//���ü�ʱ������ʱ��(MS)
bool Stm8_Calculagraph_IsExpiredMS(Stm8_CalculagraphTypeDef* timer);				//��ѯ�Ƿ񵽴��ʱ����ʱʱ��(MS)
void Stm8_Calculagraph_CountdownSec(Stm8_CalculagraphTypeDef* timer, u32 timeout_sec);	//���ü�ʱ������ʱ��(S)
bool Stm8_Calculagraph_IsExpiredSec(Stm8_CalculagraphTypeDef* timer);				//��ѯ�Ƿ񵽴��ʱ����ʱʱ��(S)

void Stm8_EventRunningTime_Init(Stm8_EventRunningTimeTypeDef* timer);				//��ʼ���¼�����ʱ����
void Stm8_EventRunningTime_StartMS(Stm8_EventRunningTimeTypeDef* timer);				//�����¼�����ʱ����(MS)
u16 Stm8_EventRunningTime_EndMS(Stm8_EventRunningTimeTypeDef* timer);		//�����¼�����ʱ����(MS)
void Stm8_EventRunningTime_StartSec(Stm8_EventRunningTimeTypeDef* timer);			//�����¼�����ʱ����(S)
u16 Stm8_EventRunningTime_EndSec(Stm8_EventRunningTimeTypeDef* timer);		//�����¼�����ʱ����(S)


#endif

