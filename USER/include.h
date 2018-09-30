/*********************************************************************************************************
//������
//���ߣ����ƮԾ    ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************************************/
#ifndef __INCLUDE_H__
#define __INCLUDE_H__
/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>			

#include "stm8l15x.h" 
#include "stm8l15x_clk.h"
#include "stm8l15x_rtc.h"
#include "stm8l15x_itc.h"
#include "stm8l15x_pwr.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_lcd.h"
#include "stm8l15x_exti.h"
#include "stm8l15x_tim1.h"
#include "stm8l15x_tim2.h"
#include "stm8l15x_tim3.h"
#include "stm8l15x_tim4.h"
#include "stm8l15x_adc.h"
#include "stm8l15x_flash.h"			

/*ԭ����������Ҫ�õ���*/
#include "main.h"
#include "mcu_init.h"
#include "Pulse_Acquire.h"
#include "save.h"
#include "rtc.h"
#include "uart.h"
#include "protocol.h"
#include "CJT-188-2004.h"
#include "BC95.h"
#include "Sys_timer.h" 
#include "string.h"
#include "Globaldata.h"
#include "Temperature.h"	
	

/*********************************************************************************************************
�궨����
*********************************************************************************************************/

#define __packed 	


/**********************�˿�����**************************/

/***************�͹�����������****************/
#define GPIO_LOW_POWER_MODE  GPIO_Mode_In_FL_No_IT

#define Cai1_PORT          GPIOD                                 //�������� cai1
#define Cai1_Pin           GPIO_Pin_0
#define Cai1_Mode          GPIO_Mode_In_FL_IT
#define Cai1               GPIO_ReadInputDataBit (Cai1_PORT,Cai1_Pin)

#define Cai2_PORT          GPIOD                                 //�������� cai2
#define Cai2_Pin           GPIO_Pin_1
#define Cai2_Mode          GPIO_Mode_In_FL_IT
#define Cai2               GPIO_ReadInputDataBit (Cai2_PORT,Cai2_Pin)

#define Weak_Up_PORT       GPIOF                                 //���ѻ���
#define Weak_Up_Pin        GPIO_Pin_0
#define Weak_Up_Mode       GPIO_Mode_In_FL_IT
#define Weak_Up            GPIO_ReadInputDataBit (Weak_Up_PORT,Weak_Up_Pin)

//
///*******************led��������*****************/
//#define Led_Green_PORT     GPIOD                               //��ɫLED
//#define Led_Green_Pin      GPIO_Pin_6
//#define Led_Green_Mode     GPIO_Mode_Out_PP_Low_Slow
//#define Led_Green_SET      GPIO_SetBits (Led_Green_PORT,Led_Green_Pin)
//#define Led_Green_RESET    GPIO_ResetBits (Led_Green_PORT, Led_Green_Pin)
//
//#define Led_Yellow_PORT    GPIOD                             //��ɫLED
//#define Led_Yellow_Pin     GPIO_Pin_7
//#define Led_Yellow_Mode    GPIO_Mode_Out_PP_Low_Slow
//#define Led_Yellow_SET     GPIO_SetBits (Led_Yellow_PORT,Led_Yellow_Pin)
//#define Led_Yellow_RESET   GPIO_ResetBits (Led_Yellow_PORT, Led_Yellow_Pin)
//
///*******************ADC���Ŷ���************************/
//
//#define NTC_Temp_PORT      GPIOA                                 //NTC adc���������
//#define NTC_Temp_Pin       GPIO_Pin_5
//#define NTC_Temp_Mode      GPIO_Mode_In_PU_No_IT
//#define NTC_Temp           GPIO_ReadInputDataBit (NTC_Temp_PORT,NTC_Temp_Pin)
//#define NTC_Temp_Channel   ADC_Channel_1
//
///*******************ģ����������********************/
//
//#define _RI_PORT       GPIOE                                 //������������
//#define _RI_Pin        GPIO_Pin_0
//#define _RI_Mode       GPIO_Mode_In_PU_No_IT
//#define _RI            GPIO_ReadInputDataBit (_RI_PORT,_RI_Pin)
//
//#define _RESET_PORT    GPIOE                               //��λ�ź����
//#define _RESET_Pin     GPIO_Pin_1
//#define _RESET_Mode    GPIO_Mode_Out_PP_Low_Slow
//#define _RESET_SET     GPIO_SetBits (_RESET_PORT,_RESET_Pin)
//#define _RESET_RESET   GPIO_ResetBits (_RESET_PORT, _RESET_Pin)
//
//#define _PWR_EN_PORT   GPIOE                             //
//#define _PWR_EN_Pin    GPIO_Pin_2
//#define _PWR_EN_Mode   GPIO_Mode_Out_PP_Low_Slow
//#define _PWR_EN_SET    GPIO_SetBits (_PWR_EN_PORT,_PWR_EN_Pin)
//#define _PWR_EN_RESET  GPIO_ResetBits (_PWR_EN_PORT, _PWR_EN_Pin)
//
///**********************������������**********************/
//#define UART2_TX_PORT      GPIOE                                 //����2TXD
//#define UART2_TX_Pin       GPIO_Pin_4
//#define UART2_TX_Mode      GPIO_Mode_Out_PP_High_Fast
//#define UART2_TX_SET       GPIO_SetBits (UART2_TX_PORT,UART2_TX_Pin)
//#define UART2_TX_RESET     GPIO_ResetBits (UART2_TX_PORT, UART2_TX_Pin)
//
//#define UART2_RX_PORT      GPIOE                                 //����2RXD
//#define UART2_RX_Pin       GPIO_Pin_3
//#define UART2_RX_Mode      GPIO_Mode_In_PU_No_IT
//#define UART2_RX           GPIO_ReadInputDataBit (UART2_RX_PORT,UART2_RX_Pin)
//
//
//#define UART3_TX_PORT      GPIOE                                 //����3TXD
//#define UART3_TX_Pin       GPIO_Pin_6
//#define UART3_TX_Mode      GPIO_Mode_Out_PP_High_Fast
//#define UART3_TX_SET       GPIO_SetBits (UART3_TX_PORT,UART3_TX_Pin)
//#define UART3_TX_RESET     GPIO_ResetBits (UART3_TX_PORT, UART3_TX_Pin)
//
//#define UART3_RX_PORT      GPIOE                                 //����3RXD
//#define UART3_RX_Pin       GPIO_Pin_7
//#define UART3_RX_Mode      GPIO_Mode_In_PU_No_IT
//#define UART3_RX           GPIO_ReadInputDataBit (UART3_RX_PORT,UART3_RX_Pin)


///**********************�����ú궨��**************************/
//#define HARDWARE  10     //Ӳ���汾1.0
//
//#define LOW_POWER 1           //�����Ƿ����͹���ģʽ 0�� 1��
//
///**********************����״̬�궨��**************************/
//#define RUN_MODE  0         //����ģʽ
//#define SLEEP_MODE 1         //˯��ģʽ
//#define MOTOR_MODE 2         //�������ģʽ
//#define DE_SLEEP_MODE 3      //��ʱ˯��ģʽ
//#define UART_MODE 4          //���ڻ�Ծģʽ
///**********************���״̬�궨��**************************/
//#define OPEN_MODE 1          //���Ŵ�
//#define CLOSE_MODE 2         //���Źر�
//#define HALF_MODE 3          //���Ű뿪
///**********************ˢ����״̬**************************/
//#define CARD_WATE 0          //�ȴ�����
//#define CARD_BEGIN 1         //��ʼ����
//#define CARD_OVER 2          //��������
/**********************EEPROM�궨��**************************/
//#define EEPROM_ADD           0X00001000     //EEPROM ��ʼ��ַ����
//#define USER_DATA_ADD        EEPROM_ADD  //�û���������ַ
//#define OPEN_USER_ADD        EEPROM_ADD  //������־�洢��ַ
//#define CLEAR_FLAG_ADD       (EEPROM_ADD+1)//�����־��ַ
//#define USER_NUM_ADD         (EEPROM_ADD+2)//�û��ŵ�ַ
//
//#define SURPLUS_FLOW_ADD     (EEPROM_ADD+9)//ʣ��������ַ
//#define BUY_COUNT_ADD        (EEPROM_ADD+13)//���������ַ
//#define LIMIT_FLOW_ADD       (EEPROM_ADD+14)//�޹�������ַ
//#define DIS_ALARM_ADD        (EEPROM_ADD+16)//��ʾ����
//#define CLO_ALARM_ADD        (EEPROM_ADD+17)//�ط�����
//#define SUBLIST_ADD          (EEPROM_ADD+18)//�ӱ�ŵ�ַ
//#define MINUS_FLAG_ADD       (EEPROM_ADD+19)//������־
//#define ADD_BUY_ADD          (EEPROM_ADD+20)//�ۻ������ַ
//
//#define SYS_DATA_ADD         (EEPROM_ADD+251)//ϵͳ����ʹ��
//#define PASSWORD_MAIN_1_ADD  (EEPROM_ADD+251)//������1ʹ��
//#define PASSWORD_MAIN_2_ADD  (EEPROM_ADD+252)//������2ʹ��
//#define PASSWORD_SLAVE_1_ADD (EEPROM_ADD+253)//��ѡ����2ʹ��
//#define PASSWORD_SLAVE_2_ADD (EEPROM_ADD+254)//��ѡ����2ʹ��
//#define HARDWARE_ADD         (EEPROM_ADD+255)//Ӳ���汾
/***********************EEPROMӳ���ַ����********************************/
//#define USER_DATA    ((union User_Data_Uni *)(USER_DATA_ADD))  //�û��������ܽṹ��
//#define OPEN_USER    ((bool *)(OPEN_USER_ADD))                 //������־
//#define CLEAR_FLAG   ((bool *)(CLEAR_FLAG_ADD))                //�����־
//#define USER_NUM     ((unsigned char *)(USER_NUM_ADD))       //�����û���
//#define ADD_FLOW     ((union flow_union *)(ADD_FLOW_ADD))      //�ۻ�����
//#define SURPLUS_FLOW ((union flow_union *)(SURPLUS_FLOW_ADD))  //ʣ������
//#define BUY_COUNT    ((unsigned char *)(BUY_COUNT_ADD))       //�������
//#define LIMIT_FLOW   ((unsigned short *)(LIMIT_FLOW_ADD))     //�޹���
//#define DIS_ALARM    ((unsigned char *)(DIS_ALARM_ADD))       //��ʾ����
//#define CLO_ALARM    ((unsigned char *)(CLO_ALARM_ADD))       //�ط�����
//#define SUBLIST      ((unsigned char *)(SUBLIST_ADD))         //�ӱ��
//#define MINUS_FLAG   ((bool *)(MINUS_FLAG_ADD))                //������־
//#define ADD_BUY      ((union flow_union *)(ADD_BUY_ADD))        //�ۻ�����   
//
//#define SYS_DATA     ((struct Sys_Data_Str *)(SYS_DATA_ADD))  //ϵͳ�������ܽṹ��
//
//#define PASSWORD_MAIN_1  ((FunctionalState *)(PASSWORD_MAIN_1_ADD))   //������1ʹ��
//#define PASSWORD_MAIN_2  ((FunctionalState *)(PASSWORD_MAIN_2_ADD))   //������2ʹ��
//#define PASSWORD_SLAVE_1 ((FunctionalState *)(PASSWORD_SLAVE_1_ADD))   //�Ӽ���2ʹ��
//#define PASSWORD_SLAVE_2 ((FunctionalState *)(PASSWORD_SLAVE_2_ADD))   //�Ӽ���2ʹ��
//#define HARDWARE_DATA    ((unsigned char *)(HARDWARE_ADD))          //Ӳ���汾˵��

/*********************************************************************************************************
�������Ͷ���
*********************************************************************************************************/
//union card_union
//{                                                           //���ڴ洢��Ƶ�������ݹ�����
//	 unsigned long int data32[7];
//	 unsigned char data8[28];
//}; 
//union user_id                                            //�û�ID��Ϣ������
//{
//	 unsigned int id16;
//	 unsigned char id8[2];
//};
//union add_buy                                               //�������������ƹ�������
//{
//	 unsigned int buy16;
//	 unsigned char buy8[2];
//};
//union buy_num                                               //�������������
//{
//	 unsigned int num16;
//	 unsigned char num8[2];
//};
//union Error_Data
//{
//  unsigned char Error_Data8;
//  struct
//  {
//    unsigned char Close_Timeout:1;//�ط���ʱ
//    unsigned char Close_Overcurrent:1;//�ط�����
//    unsigned char Valve_Leak:1;     //����©ˮ
//    unsigned char Magnetism:1;      //�Ÿ���
//    unsigned char Low_Power:1;      //Ƿѹ
//    unsigned char bad_Switch:1;     //�ɻɹ���
//  }Error_Bit;
//};
//union User_Data_Uni
//{
//  unsigned char Data8[20];
//  struct
//  {
//    bool Open_Falg;//������־
//    bool Clear_Falg;//�����־
//    unsigned char User_Num[3];//�û���
//    union flow_union Add_Flow;//�ۻ�����
//    union flow_union Surplus_Flow;//ʣ������
//    unsigned char Buy_Count;//�����������
//    unsigned short Limit_Flow;//�޹�����
//    unsigned char Dis_Alarm;//��ʾ������
//    unsigned char Clo_Alarm;//�ط�������
//    unsigned char Sublist;//�ӱ��
//    bool Minus_Falg;//������־λ
//  }Str;
//};
//struct Sys_Data_Str
//{
//  FunctionalState Password_Main_1;//������1ʹ��
//  FunctionalState Password_Main_2;//������2ʹ��
//  FunctionalState Slave_Main_1;   //��ѡ����1ʹ��
//  FunctionalState Slave_Main_2;   //��ѡ����1ʹ��
//  unsigned char Hardware;      //Ӳ���汾˵��
//};

/*********************************************************************************************************
�ⲿ����������
*********************************************************************************************************/
/*********************************************************************************************************
����������
*********************************************************************************************************/

/********************************************************************************************************/
#endif

/******************************************END********************************************************/
