/**
  *********************************************************************************************************
  * @file    sys.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2017-10-23
  * @brief   
  *********************************************************************************************************
  * @attention
  *			
  *
  *
  *********************************************************************************************************
  */

#include "sys.h"

__IO u32 SystemRunningTimesSecond = 0;										//ϵͳ����ʱ��(S)
__IO u32 SystemSoftResetTime = 0;												//ϵͳ���г�ʱ�����λʱ��(MS)
Stm8_SystemRunningTime		SystemRunningTime = {0, 0, 100, 0, 0, 0, 1, 0, 0};	//ϵͳ����ʱ��(�ṹ��)

	
/**********************************************************************************************************
 @Function			void Stm8_IncSecondTick(void)
 @Description			Stm8_IncSecondTick : ����ʱ���1��
 @Input				void
 @Return				void
**********************************************************************************************************/
void Stm8_IncSecondTick(void)
{	
	static u32 SecondTickPre;
	u32 SecondTickNow;
		
	SecondTickNow = bsp_GetRunTime();
	if ((SecondTickNow > SecondTickPre) && ((SecondTickNow - SecondTickPre) >= 1000)) {
		SystemRunningTimesSecond++;
		SecondTickPre += 1000;
	}
	else if ((0xFFFFFFFF - SecondTickPre + SecondTickNow + 1) >= 1000) {
		SystemRunningTimesSecond++;
		SecondTickPre += 1000;
	}
}

/**********************************************************************************************************
 @Function			u32 Stm8_GetSecondTick(void)
 @Description			Stm8_GetSecondTick 	: ��ȡ��������
 @Input				void
 @Return				SystemRunningTimesSecond : ��������
**********************************************************************************************************/
u32 Stm8_GetSecondTick(void)
{
	return SystemRunningTimesSecond;
}

/**********************************************************************************************************
 @Function			void Stm8_Calculagraph_Init(Stm8_CalculagraphTypeDef* timer)
 @Description			Stm8_Calculagraph_Init 			: ��ʼ����ʱ��
 @Input				Stm8_CalculagraphTypeDef*		: ��ʱ���ṹ��ָ��
 @Return				void
**********************************************************************************************************/
void Stm8_Calculagraph_Init(Stm8_CalculagraphTypeDef* timer)
{
	timer->xTicksToWait = 0;
	timer->xTimeOut = 0;
}

/**********************************************************************************************************
 @Function			void Stm8_Calculagraph_CountdownMS(Stm8_CalculagraphTypeDef* timer, u32 timeout_ms)
 @Description			Stm8_Calculagraph_CountdownMS	: ���ü�ʱ������ʱ��(MS)
 @Input				Stm8_CalculagraphTypeDef*		: ��ʱ���ṹ��ָ��
					timeout_ms					: ����ʱʱ��(MS)
 @Return				void
**********************************************************************************************************/
void Stm8_Calculagraph_CountdownMS(Stm8_CalculagraphTypeDef* timer, u32 timeout_ms)
{
	timer->xTicksToWait = timeout_ms;		
	timer->xTimeOut = bsp_GetRunTime() + timeout_ms;
}

/**********************************************************************************************************
 @Function			bool Stm8_Calculagraph_IsExpiredMS(Stm8_CalculagraphTypeDef* timer)
 @Description			Stm8_Calculagraph_IsExpiredMS	: ��ѯ�Ƿ񵽴��ʱ����ʱʱ��(MS)
 @Input				Stm8_CalculagraphTypeDef*		: ��ʱ���ṹ��ָ��
 @Return				TRUE							: ����
					FALSE						: δ����
**********************************************************************************************************/
bool Stm8_Calculagraph_IsExpiredMS(Stm8_CalculagraphTypeDef* timer)
{	
	u32 MilliSecondTickNow;
	
	MilliSecondTickNow = bsp_GetRunTime();
	
	if ((MilliSecondTickNow > (timer->xTimeOut - timer->xTicksToWait)) && ((MilliSecondTickNow - (timer->xTimeOut - timer->xTicksToWait)) >= timer->xTicksToWait)) {
		return TRUE;
	}
	else if ((0xFFFFFFFF - (timer->xTimeOut - timer->xTicksToWait) + MilliSecondTickNow + 1) >= timer->xTicksToWait) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

/**********************************************************************************************************
 @Function			void Stm8_Calculagraph_CountdownSec(Stm8_CalculagraphTypeDef* timer, u32 timeout_sec)
 @Description			Stm8_Calculagraph_CountdownSec	: ���ü�ʱ������ʱ��(S)
 @Input				Stm8_CalculagraphTypeDef*		: ��ʱ���ṹ��ָ��
					timeout_sec					: ����ʱʱ��(S)
 @Return				void
**********************************************************************************************************/
void Stm8_Calculagraph_CountdownSec(Stm8_CalculagraphTypeDef* timer, u32 timeout_sec)
{
	timer->xTicksToWait = timeout_sec;
	timer->xTimeOut = Stm8_GetSecondTick() + timeout_sec;
}

/**********************************************************************************************************
 @Function			bool Stm8_Calculagraph_IsExpiredSec(Stm8_CalculagraphTypeDef* timer)
 @Description			Stm8_Calculagraph_IsExpiredSec	: ��ѯ�Ƿ񵽴��ʱ����ʱʱ��(S)
 @Input				Stm8_CalculagraphTypeDef*		: ��ʱ���ṹ��ָ��
 @Return				TRUE							: ����
					FALSE						: δ����
**********************************************************************************************************/
bool Stm8_Calculagraph_IsExpiredSec(Stm8_CalculagraphTypeDef* timer)
{
	u32 SecondTickNow;
	
	SecondTickNow = Stm8_GetSecondTick();
		
	if ((SecondTickNow > (timer->xTimeOut - timer->xTicksToWait)) && ((SecondTickNow - (timer->xTimeOut - timer->xTicksToWait)) >= timer->xTicksToWait)) {
		return TRUE;
	}
	else if ((0xFFFFFFFF - (timer->xTimeOut - timer->xTicksToWait) + SecondTickNow + 1) >= timer->xTicksToWait) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

/**********************************************************************************************************
 @Function			void Stm8_EventRunningTime_Init(Stm8_EventRunningTimeTypeDef* timer)
 @Description			Stm8_EventRunningTime_Init 		: ��ʼ���¼�����ʱ����
 @Input				Stm8_CalculagraphTypeDef*		: �¼�����ʱ�����ṹ��ָ��
 @Return				void
**********************************************************************************************************/
void Stm8_EventRunningTime_Init(Stm8_EventRunningTimeTypeDef* timer)
{
	timer->bEventRunningState = FALSE;
	timer->xEventRunningStartTime = 0;
	timer->xEventRunningEndTime = 0;
}

/**********************************************************************************************************
 @Function			void Stm8_EventRunningTime_StartMS(Stm8_EventRunningTimeTypeDef* timer)
 @Description			Stm8_EventRunningTime_StartMS	: �����¼�����ʱ����(MS)
 @Input				Stm8_CalculagraphTypeDef*		: �¼�����ʱ�����ṹ��ָ��
 @Return				void
**********************************************************************************************************/
void Stm8_EventRunningTime_StartMS(Stm8_EventRunningTimeTypeDef* timer)
{
	if (timer->bEventRunningState == FALSE) {
		timer->bEventRunningState = TRUE;	
		timer->xEventRunningStartTime = Stm8_GetSecondTick();
	}
}

/**********************************************************************************************************
 @Function			u16 Stm8_EventRunningTime_EndMS(Stm8_EventRunningTimeTypeDef* timer)
 @Description			Stm8_EventRunningTime_StartMS	: �����¼�����ʱ����(MS)
 @Input				Stm8_CalculagraphTypeDef*		: �¼�����ʱ�����ṹ��ָ��
 @Return				u16					: �¼�����ʱ��(MS)
**********************************************************************************************************/
u16 Stm8_EventRunningTime_EndMS(Stm8_EventRunningTimeTypeDef* timer)
{	
	u16 ret = 0;
	
	if (timer->bEventRunningState == TRUE) {
		timer->bEventRunningState = FALSE;
		timer->xEventRunningEndTime = Stm8_GetSecondTick();
		ret = (timer->xEventRunningEndTime > timer->xEventRunningStartTime) ? \
			 (timer->xEventRunningEndTime - timer->xEventRunningStartTime) : \
			 (0xFFFFFFFF - timer->xEventRunningStartTime + timer->xEventRunningEndTime);
	}
	
	return ret;
}

/**********************************************************************************************************
 @Function			void Stm8_EventRunningTime_StartSec(Stm8_EventRunningTimeTypeDef* timer)
 @Description			Stm8_EventRunningTime_StartSec	: �����¼�����ʱ����(S)
 @Input				Stm8_CalculagraphTypeDef*		: �¼�����ʱ�����ṹ��ָ��
 @Return				void
**********************************************************************************************************/
void Stm8_EventRunningTime_StartSec(Stm8_EventRunningTimeTypeDef* timer)
{	
	if (timer->bEventRunningState == FALSE) {
		timer->bEventRunningState = TRUE;
		timer->xEventRunningStartTime = Stm8_GetSecondTick();
	}
}

/**********************************************************************************************************
 @Function			u16 Stm8_EventRunningTime_EndSec(Stm8_EventRunningTimeTypeDef* timer)
 @Description			Stm8_EventRunningTime_EndSec		: �����¼�����ʱ����(S)
 @Input				Stm8_CalculagraphTypeDef*		: �¼�����ʱ�����ṹ��ָ��
 @Return				u16					: �¼�����ʱ��(S)
**********************************************************************************************************/
u16 Stm8_EventRunningTime_EndSec(Stm8_EventRunningTimeTypeDef* timer)
{
	u16 ret = 0;
	
	if (timer->bEventRunningState == TRUE) {
		timer->bEventRunningState = FALSE;
		timer->xEventRunningEndTime = Stm8_GetSecondTick();
		ret = (timer->xEventRunningEndTime > timer->xEventRunningStartTime) ? \
			 (timer->xEventRunningEndTime - timer->xEventRunningStartTime) : \
			 (0xFFFFFFFF - timer->xEventRunningStartTime + timer->xEventRunningEndTime);
	}
	
	return ret;
}



/********************************************** END OF FLEE **********************************************/

