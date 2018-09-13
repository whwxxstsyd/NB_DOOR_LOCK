/*
*********************************************************************************************************
*
*	模块名称 : LED指示灯驱动模块
*	文件名称 : bsp_led.c
*	版    本 : V1.0
*	说    明 : 驱动LED指示灯
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2013-02-01 armfly  正式发布
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"


/* 按键口对应的RCC时钟 */
#define GPIO_PORT_LED0  	GPIOD			
#define GPIO_PIN_LED0		GPIO_Pin_7

#define GPIO_PORT_LED1  	GPIOE	
#define GPIO_PIN_LED1		GPIO_Pin_7

#define GPIO_PORT_LED2  	GPIOE	
#define GPIO_PIN_LED2		GPIO_Pin_6	
	

/*
*********************************************************************************************************
*	函 数 名: bsp_InitLed
*	功能说明: 配置LED指示灯相关的GPIO,  该函数被 bsp_Init() 调用。
*	形    参:  无
*	返 回 值: 无
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
*	函 数 名: bsp_LedOn
*	功能说明: 点亮指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 无
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
*	函 数 名: bsp_LedOff
*	功能说明: 熄灭指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 无
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
*	函 数 名: bsp_LedToggle
*	功能说明: 翻转指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 按键代码
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
*	函 数 名: bsp_IsLedOn
*	功能说明: 判断LED指示灯是否已经点亮。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 1表示已经点亮，0表示未点亮
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

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
