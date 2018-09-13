/*********************************************************************************************************
//������
//���ߣ����ƮԾ    ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************************************/

#ifndef __MAIN_H__
#define __MAIN_H__
/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include "include.h"
/*********************************************************************************************************
�궨����
*********************************************************************************************************/
#define  SOFTWARE_VERSION       10                 //����汾V1.0

#define  EEPROM_ADD             0X00001000           //EEPROM ��ʼ��ַ����
#define  SOFTWARE_VERSION_ADD   (EEPROM_ADD)         //����汾��ַ
#define  SERVER_ADD1_EEPROM_ADD (EEPROM_ADD+10)      //IP��ַ1
#define  SERVER_ADD2_EEPROM_ADD (EEPROM_ADD+20)      //IP��ַ2
#define  ADD_FLOW_ADD           (EEPROM_ADD+30)      //�ۻ�������ַ
#define  HALL_ERROR_ADD         (EEPROM_ADD+35)      //����״̬
#define  CJT_188_ADD            (EEPROM_ADD+40)      //188��ַ�洢
#define  ACCOUNT_FLOW_ADD       (EEPROM_ADD+50)      //������������ַ
#define  HISTORY_FLOW_ADD       (EEPROM_ADD+55)      //��ʷ������ַ
#define  SETTLEMENT_DATE_ADD    (EEPROM_ADD+110)     //�������ڵ�ַ
#define  SAMPLE_FREQUENCY_ADDR  (EEPROM_ADD+111)     //����Ƶ�ʵ�ַ
#define  REPORT_FREQUENCY_ADDR  (EEPROM_ADD+114)     //�ϱ�Ƶ�ʵ�ַ
#define  BAT_ALARM_ADD          (EEPROM_ADD+116)     //��ر���ֵ��ַ
#define  BC95_ERROR_RECORD_ADD  (EEPROM_ADD+122)     //BC95���������¼��ַ

#define  SDCF1_ADDR             (EEPROM_ADD+126)      //��1�½�����������ַ
#define  SDCF2_ADDR             (EEPROM_ADD+130)      //��2�½�����������ַ
#define  SDCF3_ADDR             (EEPROM_ADD+134)      //��3�½�����������ַ
#define  SDCF4_ADDR             (EEPROM_ADD+138)      //��4�½�����������ַ
#define  SDCF5_ADDR             (EEPROM_ADD+142)      //��5�½�����������ַ
#define  SDCF6_ADDR             (EEPROM_ADD+146)      //��6�½�����������ַ
#define  SDCF7_ADDR             (EEPROM_ADD+150)      //��7�½�����������ַ
#define  SDCF8_ADDR             (EEPROM_ADD+154)      //��8�½�����������ַ
#define  SDCF9_ADDR             (EEPROM_ADD+158)      //��9�½�����������ַ
#define  SDCF10_ADDR            (EEPROM_ADD+162)      //��10�½�����������ַ
#define  SDCF11_ADDR            (EEPROM_ADD+166)      //��11�½�����������ַ
#define  SDCF12_ADDR            (EEPROM_ADD+170)      //��12�½�����������ַ
#define  SDCF13_ADDR            (EEPROM_ADD+174)      //��13�½�����������ַ

//#define  HISTORYL_READ_ADDR     (EEPROM_ADD+180)      //��ʷ���ݶ�ȡ������ַ
#define  HISTORYL_SAVE_ADDR     (EEPROM_ADD+180)      //��ʷ���ݱ���������ַ
#define  HISTORYL_DATA_ADDR     (EEPROM_ADD+181)      //��ʷ���ݵ�ַ

//#define  VERSION        *((const unsigned char *)(SOFTWARE_VERSION_ADD))
//#define  IP1            ((const union Network_Par_Uni *)(SERVER_ADD1_EEPROM_ADD))  //IP��ַ1
//#define  IP2            ((const union Network_Par_Uni *)(SERVER_ADD2_EEPROM_ADD))  //IP��ַ2
//#define  ADD_FLOW       ((const union flow_union *)(ADD_FLOW_ADD))                //�ۻ�����
//#define  CAL_STATE      ((const enum Cal_State_En *)(CAL_STATE_ADD))              //����״̬��
//#define  ACCOUNT_FLOW   ((const union flow_union *)(ACCOUNT_FLOW_ADD))            //����������
//#define  HISTORY_FLOW   ((const union flow_union *)(HISTORY_FLOW_ADD))           //��ʷ����
//#define  SET_DATE       ((const unsigned char *)(SET_DATE_ADD))                  //����������
//#define  READ_DATA      ((const unsigned char *)(READ_DATE_ADD))                 //����������
//#define  BAT_ALARM      ((const unsigned short *)(BAT_ALARM_ADD))                 //��ر���ֵ
//#define  UP_DATA        ((const struct Up_Date_Str *)(UP_DATE_ADD))               //�ϴ�ʱ��
/*********************************************************************************************************
�������Ͷ���
*********************************************************************************************************/
//struct Network_Par_Str
//{
//  unsigned char IP[4];   //����IP
//  unsigned short Port;   //���Ӷ˿�
//};
//
//union Network_Par_Uni
//{
//  struct Network_Par_Str Str;
//  unsigned char Byte[6];
//};
//
//struct Up_Date_Str
//{
//  unsigned char Day;
//  unsigned char Hour;
//};
/*********************************************************************************************************
�ⲿ����������
*********************************************************************************************************/

/*********************************************************************************************************
����������
*********************************************************************************************************/
void Led_Green_ON(void);
void Led_Green_OFF(void);
void Sleep(void);
void IWDG_INIT(void);
/********************************************************************************************************/
#endif

/******************************************END********************************************************/
