#ifndef __BC95_H__
#define __BC95_H__
/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include "include.h"
/*********************************************************************************************************
�궨����
*********************************************************************************************************/
#define         RECV_BUFF_SIZE               256//���ջ�������С
#define         SEND_BUFF_SIZE               128//���ͻ�������С
#define         CONNECT_ERROR_NUM            3  //���Ӵ������
#define         SEND_ERROR_NUM               3  //���ʹ������

#define         BC95_POWER_UP_TIME           5//BC95�ϵ�ʱ�� ��λms
#define         BC95_R_TIMEROUT_TIME         3 //���ճ�ʱ ��λms

#define         BC95_S_TIMEROUT_TIME         6000 //�������ݳ�ʱ ��λms

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

struct Error_Connter_Str
{
  unsigned char Send;
  unsigned char Connect;
};
 

enum Start_Process_En   //��������
{
  BC95_RECONNECT,        //����
  BC95_POWER_DOWN,      //����
  BC95_POWER_UP,        //�ϵ�
  
  NRB,                  //����
  AT,                   //ͬ��������
  GETNBAND,             //��ѯƵ��
  SETNBAND,             //����Ƶ��
  GETCFUN,              //��ѯ�绰����
  SETCFUN,              //���õ绰����
  CIMI,                 //��ѯIMSI
  CGSN,                 //��ѯIMEI  
  CCID,                //��ѯCCID
  CSQ,                  //��ѯ�ź�ǿ��
  GETCGDCONT,           //��ѯAPN
  SETCGDCONT,           //����APN  
  GETCGATT,             //��ѯ���缤��״̬
  SETCGATT,             //��������
  CEREG,                //��ѯ����ע��״̬
  CCLK,                 //��ѯʵʱʱ��
  GETNCDP,              //��ѯCDP������
  SETNCDP,              //����CDP������
  NSMI,                 //���÷�����Ϣָʾ
  NNMI,                 //���ý�����Ϣָʾ
  NMGS,                 //������Ϣ
  NQMGR,                //��ѯ��Ϣ����
  NMGR,                 //������Ϣ
  BC95_CONNECT_ERROR,      //��������
};
  
struct BC95_Str//BC95 �ܽṹ��
{
  char R_Buffer[RECV_BUFF_SIZE];        //���ջ�����
  unsigned short Recv_Length;         //���ճ���
  enum Start_Process_En Start_Process;  //���ӽ���
  u8 Report_Bit;                        //����λ
  struct Error_Connter_Str Err_Conner; //�������
  unsigned char ErrorRecord;          //�����¼
  unsigned char Rssi;                 //�ź�ǿ��
  bool Incident_Pend;                  //�¼������־

  unsigned char Reconnect_Times;      //��������
  unsigned char FailTimes;            //ʧ�ܴ���
 // unsigned short Run_Time;           //����ʱ��
  unsigned char ICCID[20];
  unsigned char IMEI[15];
// char S_Buffer[SEND_BUFF_SIZE]; //���ͻ�����
//  unsigned short Send_Length; //���ͳ���
//  unsigned char Data_Buffer[RECV_BUFF_SIZE-20];//���ݻ���
//  unsigned short Data_Length;//���ݳ���
//  unsigned char R_Channel;//���յ��ŵ�
//  unsigned char S_Channel;//���͵��ŵ�
//  bool Data_Pend;//���ݹ���
//  bool Channel_Connect[6];//�ŵ����ӱ�־
//  bool Channel_Connect_ERROR[6];//�ŵ�����ʧ�ܱ�־
//  unsigned char Connect_Channel_Num;//�����е��ŵ�
//  enum Send_Process_En Send_Process;//���ͽ���
//  struct Alarm_EN  Alarm;         //�澯
//  enum BC95_State_Eum State;//״̬
//  bool ZIPSTATUS_Pend;//���ڲ�ѯ״̬
};
 
/*********************************************************************************************************
�ⲿ����������
*********************************************************************************************************/
extern struct BC95_Str BC95;//BC95 �õļĴ���
/*********************************************************************************************************
����������
*********************************************************************************************************/
void BC95_Power_On(void); 
void BC95_Power_Off(void);
void BC95_Reset(void);
void BC95_Process(void);
void BC95_Data_Send(unsigned char *Data,unsigned short Len);

void BC95_Open(void);
void BC95_CLose(void);
void BC95_Start(void);
void BC95_Process(void);
void BC95_Start_Timeout_CallBack(void);
void BC95_Delay_CallBalk(void);
//void BC95_PowerDown_CallBalk(void);

void Recv_Data_Process(unsigned char* buff);
unsigned char Send_Data_Process(void);
void ACK(u8 messageId,u8 errcode,u8 mid[4]);
void Report_All_Parameters(void);
void Report_HC_Flow(void);

unsigned char Report_History_Data(void);
//void GSM_SET_AT_IP(unsigned short Port,unsigned char *IP,unsigned char Channel);
//void BC95_PowerUP_CallBalk(void);
//void GSM_Send_AT_Cmd(unsigned char *GSM_SEND_AT,unsigned short cmd_len);
//
//ErrorStatus BC95_Send(unsigned char *Send_Data,unsigned short Send_Lenght,unsigned char channel);
//unsigned short BC95_Receive(unsigned char *R_buff);
//void BC95_ZIPSTATUS(unsigned char Channel);//��ѯ��ǰTCP����
//void Send_Data_Timeout(void);//�������ݳ�ʱ
//void BC95_ZIPCLOSE(unsigned char Channel);//�رյ�ǰTCP����
/********************************************************************************/
#endif

/******************************************END********************************************************/