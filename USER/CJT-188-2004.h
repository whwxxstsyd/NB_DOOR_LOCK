/*********************************************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************************************/
#ifndef  __CJT_188_2004_H
#define  __CJT_188_2004_H
/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include "include.h"
/*********************************************************************************************************
�궨����
*********************************************************************************************************/
#define PNULL  (void*)(0x0000)

#define  Locall_MeterType     ColdWaterMeter
#define  Locall_FactoryID     0x0000//('K' *10) + 'D'



#define  CJT_188_Preamble                0xFE
#define  CJT_188_PreambleLength          2
#define  CJT_188_StartCharacter          0x68
#define  CJT_188_Pause                   0x16


#define  CTR_ReadData_ASK       0x01          //������ ������
#define  CTR_ReadData_ACK       0x81          //������ ��Ӧ��
#define  CTR_ReadData_ERR       0xC1          //������ ������

#define  CTR_ReadVerNo_ASK      0x09          //����Կ�汾�� ������
#define  CTR_ReadVerNo_ACK      0x89          //����Կ�汾�� ��Ӧ��
#define  CTR_ReadVerNo_ERR      0xC9          //����Կ�汾�� ������

#define  CTR_ReadAddress_ASK    0x03          //����ַ ������
#define  CTR_ReadAddress_ACK    0x83          //����ַ ��Ӧ��
#define  CTR_ReadAddress_ERR    0xC3          //����ַ ������

#define  CTR_WriteData_ASK      0x04          //д���� ������
#define  CTR_WriteData_ACK      0x84          //д���� ��Ӧ��
#define  CTR_WriteData_ERR      0xC4          //д���� ������

#define  CTR_WriteAddress_ASK    0x15          //д��ַ ������
#define  CTR_WriteAddress_ACK    0x95          //д��ַ ��Ӧ��
#define  CTR_WriteAddress_ERR    0xD5          //д��ַ ������

#define  CTR_WriteOffset_ASK     0x16          //д����ͬ�����ݣ�����
#define  CTR_WriteOffset_ACK     0x96          //д����ͬ������ ��Ӧ��
#define  CTR_WriteOffset_ERR     0xD6          //д����ͬ������ ������

#define  DI_ReadData          0x901F      //��������
#define  DI_ReadHistData_1    0xD120      //��ʷ��������1
#define  DI_ReadHistData_2    0xD121      //��ʷ��������2
#define  DI_ReadHistData_3    0xD122      //��ʷ��������3
#define  DI_ReadHistData_4    0xD123      //��ʷ��������4
#define  DI_ReadHistData_5    0xD124      //��ʷ��������5
#define  DI_ReadHistData_6    0xD125      //��ʷ��������6
#define  DI_ReadHistData_7    0xD126      //��ʷ��������7
#define  DI_ReadHistData_8    0xD127      //��ʷ��������8
#define  DI_ReadHistData_9    0xD128      //��ʷ��������9
#define  DI_ReadHistData_10   0xD129      //��ʷ��������10
#define  DI_ReadHistData_11   0xD12A      //��ʷ��������11
#define  DI_ReadHistData_12   0xD12B      //��ʷ��������12
#define  DI_ReadPriceList     0x8102      //���۸��
#define  DI_ReadSelDate       0x8103      //��������
#define  DI_ReadRecordDate    0x8104      //��������
#define  DI_ReadRMoney        0x8105      //��������
#define  DI_ReadVerNo         0x8106      //����Կ�汾��
#define  DI_ReadTemp          0x8108      //�������¶�
#define  DI_ReadAddress       0x810A      //����ַ
#define  DI_ReadIP1           0x8209      //��ȡIP1
#define  DI_ReadIP2           0x820A      //��ȡIP2
#define  DI_ReadBattey        0x820B      //��ȡ��ص�ѹ
#define  DI_ReadBatteyAlarm   0x820C      //���͵�ѹ����ֵ
#define  DI_ReadUpdataTime    0x820D      //���ϴ�ʱ��
#define  DI_ReadRSSI          0x820E      //���ź�ǿ��S

#define  DI_WritePriceList     0xA010      //д�۸��
#define  DI_WriteSelDate       0xA011      //д��������
#define  DI_WriteRecordDate    0xA012      //д������
#define  DI_WriteRMoney        0xA013      //��������
#define  DI_WriteSecretKey     0xA014      //д��Կ�汾��
#define  DI_WriteTime          0xA015      //д��׼ʱ��
#define  DI_WriteValveState    0xA017      //д���ſ���
#define  DI_Lock               0xA019      //��������
#define  DI_WriteAddress       0xA018      //д��ַ
#define  DI_WriteOffset        0xA016      //д����ͬ��
#define  DI_WriteIP1           0xA201      //�����ϴ�IP1
#define  DI_WriteIP2           0xA202      //�����ϴ�IP2
#define  DI_WriteBatteyAlarm   0xA203      //���õ�ر���ֵ
#define  DI_WriteUpdataTime    0xA204      //�����ϴ�����
#define  DI_AotoUpData         0xA205      //�����Զ��ϴ�����

#define  CJT8_188ValveOpenCmd        0x55     //�� �� �� �� ���� :55H
#define  CJT8_188ValveCloseCmd       0x99     //�� �� �� �� ���� :99H
#define  CJT8_188ValveMaintainCmd    0xAA     //�� �� ���� :AAH
#define  CJT8_188ValveHalfCmd        0x77     //���Ű뿪
/*********************************************************************************************************
�������Ͷ���
*********************************************************************************************************/
typedef unsigned char           CJT_188_SER_Type;
typedef unsigned short          CJT_188_DI_Type;

typedef enum{ASK_CJT_188 = 0, ACK_CJT_188 = 1  }CJT_188_DirType;


  
typedef enum{
  ColdWaterMeter  = 0x10,   //��ˮ��
  HeatWaterMeter  = 0x11,   //������ˮ��
  DrinkWaterMeter = 0x12,   //ֱ��ˮ��
  RecycledWaterMeter = 0x13,   //��ˮ��
  UnknownMeterType   = 0xAA 
}CJT_188_MeterType;

typedef enum{
  Wh        = 0x02,   //��ˮ��
  KWh       = 0x05,   //������ˮ��
  MWh       = 0x08,
  MWh_100   = 0x0a,
  J         = 0x01,
  KJ        = 0x0B,
  MJ        = 0x0E, 
  GJ        = 0x11,
  GJ_100    = 0x13,
  W         = 0x14,
  KW        = 0x17,
  MW        = 0x1A,
  L         = 0x29,
  m3        = 0x2C,
  L_h       = 0x32,
  m3_h      = 0x35,
  V         = 0x36
}CJT_188_UnitType;

typedef struct{
  unsigned char MeterID[7];
  //unsigned char MeterID[5];
  //unsigned short FactoryID;
}CJT_188_AddresStruct;

typedef struct
{
  CJT_188_UnitType     Flow_Offse_Unit;            //���û���ͬ����λ
  unsigned char      Flow_Offse[4];              //���û���ͬ������
}CJT_188_WriteOffsetStruct;

typedef union{
  unsigned char Byte;
  struct{
    unsigned char Infr : 6;
    unsigned char CommFlg : 1;
    unsigned char DIR : 1;
  }Bit;
}CJT_188_CTRStruct;

typedef struct 
{
  unsigned char       StartCharacter;
  CJT_188_MeterType    Type;
  CJT_188_AddresStruct Addr;
  CJT_188_CTRStruct    CTR;
  unsigned char       Length;
}CJT_188_FrameHeadStruct;

typedef struct{
  CJT_188_DI_Type   DI;
  CJT_188_SER_Type  SER;
}CJT_188_DataHeadStruct;

typedef union {
   unsigned short Word;
   struct{
      unsigned short Custom :3;     //������ϸ״̬
      unsigned short Magnetism :1;   //�Ź�����־ 0:�޴Ź�����1���дŹ���
      unsigned short Fault :1;   //���ϱ�־ 0:�޹��ϣ�1���й���
      unsigned short Reserved :3;   //Ԥ��
      unsigned short Valve_State :2;    //����״̬ 00������ 01 �أ� 11���쳣
      unsigned short Bat_Flg: 1;    //��ص�ѹ   0������  1��Ƿѹ
      unsigned short CN: 2;          //����
      unsigned short F3: 1;          //�����Զ���3
      unsigned short F2: 1;          //�����Զ���2
      unsigned short F1: 1;          //�����Զ���1
    }Bit;
}CJT_188_ST_Stuct;

//ʵʱʱ�Ӹ�ʽ
typedef struct{
  unsigned char   Sec;
  unsigned char   Min;
  unsigned char   Hour;
  unsigned char   Day;
  unsigned char   Mon;
  unsigned char  Year_L;
  unsigned char  Year_H;
}CJT_188_Time_Struct;

//������ ��ʽ
typedef struct
{
  unsigned char       Flow_PV[4];              //��ǰ�ۻ�����
  CJT_188_UnitType     Flow_PV_Unit;            //��ǰ�ۻ�������λ
  unsigned char       Flow_Accounts[4];        //�������ۻ�����
  CJT_188_UnitType     Flow_Accounts_Unit;      //�������ۻ�������λ
  CJT_188_Time_Struct  Time;                    //ʵʱʱ��
  CJT_188_ST_Stuct     ST;                      //״̬ST
}CJT_188_MeterData_Struct;  

//�����ϴ���ʽ
typedef struct
{
  unsigned char       Flow_PV[4];              //��ǰ�ۻ�����
  CJT_188_UnitType     Flow_PV_Unit;            //��ǰ�ۻ�������λ
  unsigned char       Flow_Accounts[4];        //�������ۻ�����
  CJT_188_UnitType     Flow_Accounts_Unit;      //�������ۻ�������λ
  CJT_188_Time_Struct  Time;                    //ʵʱʱ��
  unsigned char      BatteyData[2];              //��ص�ѹ
  CJT_188_UnitType     BatteyData_Unit;            //��ص�ѹ��λ
  unsigned char RSSI;                         //�ź�ǿ��
  signed char Temp;                            //�¶���Ϣ
  CJT_188_ST_Stuct     ST;                      //״̬ST
}CJT_188_AotuUpData_Struct;  

//����ʷ ��ʽ
typedef struct
{
  unsigned char       Flow_History[4];              //��ʷ����
  CJT_188_UnitType     Flow_History_Unit;            //��ʷ������λ
}CJT_188_HistoryData_Struct;
  
//���¶ȸ�ʽ
typedef struct 
{
  signed char Temp;
  CJT_188_ST_Stuct    ST;
}CJT_188_TempData_Struct;

//��IP��ʽ
typedef struct 
{
  unsigned char PORT_L;
  unsigned char PORT_H;
  unsigned char IP_4;
  unsigned char IP_3;
  unsigned char IP_2;
  unsigned char IP_1;
  CJT_188_ST_Stuct    ST;
}CJT_188_IPData_Struct;  

//����ص�ѹ��ʽ
typedef struct
{
  unsigned char      BatteyData[2];              //��ص�ѹ
  CJT_188_UnitType     BatteyData_Unit;            //��ص�ѹ��λ
  CJT_188_ST_Stuct    ST;
}CJT_188BatteyData_Struct;

//���ϴ�����
typedef struct
{
  unsigned char Hour;
  unsigned char Day;
  CJT_188_ST_Stuct    ST;
}CJT_188UpDate_Struct;

//���ź�ǿ��
typedef struct
{
  unsigned char RSSI;
  CJT_188_ST_Stuct    ST;
}CJT_188RSSI_Struct;

//����Ӧ֡��ʽ
typedef struct
{
  CJT_188_DataHeadStruct    DataHead;
  union
  {
   CJT_188_MeterData_Struct MeterData;     //�����ݽṹ��
   CJT_188_HistoryData_Struct HistoryData; //����ʷ�����ṹ��
   CJT_188_ST_Stuct         ST;           //ST״̬
   unsigned char Read_Date;              //��������
   unsigned char Set_Date;              //����������
   CJT_188_TempData_Struct Temp;          //���¶Ƚṹ��
   CJT_188_IPData_Struct IP;              //��IP�Ľṹ��
   CJT_188BatteyData_Struct Battey;       //����صĽṹ��
   CJT_188UpDate_Struct UpDate;           //�ϴ����ڽṹ��
   CJT_188RSSI_Struct RSSI;               //�ϴ��ź�ǿ�Ƚṹ��
   CJT_188_AotuUpData_Struct Aotu ;       //�����ϴ��ṹ��
  }Data;  
}CJT_188_OkACK_Struct;

//����Ӧ��֡��ʽ
typedef struct{
  CJT_188_SER_Type         SER;
  CJT_188_ST_Stuct         ST;
}CJT8_188_ErrACK_Struct;


typedef struct{
  CJT_188_SER_Type         SER;
  CJT_188_ST_Stuct         ST;
  unsigned char           ValveCmd;
}CJT8_188_ValveAsk_Struct;
  
typedef struct{
CJT_188_DataHeadStruct  DataHead;
CJT_188_AddresStruct    Addres;
}CJT_188_Address_OkAckStruct; 

//���յĺ���
typedef struct
{
  CJT_188_FrameHeadStruct Head;
  CJT_188_DataHeadStruct  DataHead;
  union
  {
   unsigned char buff[16];              //��ͨ������
   unsigned char Valve_State;           //����״̬
   unsigned char Read_Date;             //��������
   unsigned char Set_Date;              //����������
   CJT_188_Time_Struct  Time;             //ʵʱʱ��
   CJT_188_IPData_Struct IP;              //��IP�Ľṹ��
   CJT_188BatteyData_Struct Battey;       //����صĽṹ��
   CJT_188UpDate_Struct UpDate;           //�ϴ����ڽṹ��
  }Data;  
}CJT_188_AskFrameData_Struct;

typedef enum
{
  NoErr   = 0,
  Busy = 1,
  Length_err   = 2,
  FramePreamble_err = 3,
  FrameSync_err = 4,
  Verify_err = 5,
  OperateCmd_err    = 6,
  DataRomWrite_err  = 7,
  FrameInvalid   = 8,        //��Ч����֡
  CounterFault =9 ,         //����������
  
  PassWordErr = 10,
  SendErr  = 11,
}ErrCodeType;
/*********************************************************************************************************
�ⲿ����������
*********************************************************************************************************/

/*********************************************************************************************************
����������
*********************************************************************************************************/
ErrCodeType CJT_188_2004_InPut (void *pbuff, unsigned length);
ErrCodeType send_AotuUpMeterData (void);
/********************************************************************************************************/
#endif

/******************************************END********************************************************/