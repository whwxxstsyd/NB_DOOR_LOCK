/*
*********************************************************************************************************
*
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.c
*	��    �� : V1.0
*	˵    �� : ����LEDָʾ��
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-02-01 armfly  ��ʽ����
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"


/* �����ڶ�Ӧ��RCCʱ�� */
#define GPIO_PORT_LED0  	GPIOD			
#define GPIO_PIN_LED0		GPIO_Pin_7

#define GPIO_PORT_LED1  	GPIOE	
#define GPIO_PIN_LED1		GPIO_Pin_7

#define GPIO_PORT_LED2  	GPIOE	
#define GPIO_PIN_LED2		GPIO_Pin_6	
	

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitLed
*	����˵��: ����LEDָʾ����ص�GPIO,  �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitLed(void)
{	
    GPIO_Init(GPIO_PORT_LED0, GPIO_PIN_LED0, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIO_PORT_LED1, GPIO_PIN_LED1, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIO_PORT_LED2, GPIO_PIN_LED2, GPIO_Mode_Out_PP_Low_Fast);

	bsp_LedOff(NET_RUNNING_LED);	
	bsp_LedOff(SYSTEM_RUNNING_LED);	
	bsp_LedOff(DEVICE_WORKING_LED);				
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedOn
*	����˵��: ����ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_LedOn(uint8_t _no)
{
	if (_no == 0)
	{
		GPIO_SetBits(GPIO_PORT_LED0,GPIO_PIN_LED0);
	}
	else if(_no == 1)
	{
		GPIO_SetBits(GPIO_PORT_LED1,GPIO_PIN_LED1);
	}
	else if(_no == 2)
	{
		GPIO_SetBits(GPIO_PORT_LED2,GPIO_PIN_LED2);	
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedOff
*	����˵��: Ϩ��ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_LedOff(uint8_t _no)
{
	if (_no == 0)
	{	
		GPIO_ResetBits(GPIO_PORT_LED0,GPIO_PIN_LED0);
	}
	else if(_no == 1)
	{
		GPIO_ResetBits(GPIO_PORT_LED1,GPIO_PIN_LED1);
	}
	else if(_no == 2)
	{
		GPIO_ResetBits(GPIO_PORT_LED2,GPIO_PIN_LED2);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedToggle
*	����˵��: ��תָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
void bsp_LedToggle(uint8_t _no)
{
	if (_no == 0)
	{		
		GPIO_ToggleBits(GPIO_PORT_LED0, GPIO_PIN_LED0); 		
	}
	else if(_no == 1)
	{
		GPIO_ToggleBits(GPIO_PORT_LED1,GPIO_PIN_LED1);
	}
	else if(_no == 2)
	{	
		GPIO_ToggleBits(GPIO_PORT_LED2,GPIO_PIN_LED2);
	}

}

/*
*********************************************************************************************************
*	�� �� ��: bsp_IsLedOn
*	����˵��: �ж�LEDָʾ���Ƿ��Ѿ�������
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: 1��ʾ�Ѿ�������0��ʾδ����
*********************************************************************************************************
*/
uint8_t bsp_IsLedOn(uint8_t _no)
{
	if (_no == 0)
	{		
		if (GPIO_ReadOutputDataBit(GPIO_PORT_LED0,GPIO_PIN_LED0) == 0)
		{	
			return 1;
		}
		return 0;
	}
	else if (_no == 1)
	{		
		if (GPIO_ReadOutputDataBit(GPIO_PORT_LED1,GPIO_PIN_LED1) == 0)
		{	
			return 1;
		}
		return 0;
	}
	else if (_no == 2)	
	{				
		if (GPIO_ReadOutputDataBit(GPIO_PORT_LED2,GPIO_PIN_LED2) == 0)
		{	
			return 1;
		}
		return 0;
	}

	return 0;
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
