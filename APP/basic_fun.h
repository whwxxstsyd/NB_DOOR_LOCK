#ifndef	__BASICFUNC_H
#define __BASICFUNC_H
 
#ifdef __cplusplus
extern "C" {
#endif

#include "include.h"	

// ��������  : BCDתHex
u8 BCDToHex(u8 TC_char);
// ��������  : BCDתHex
u32 U32BCDToU32Hex(u32 TC_char);
// ��������  : BCDתHex
u16 U16BCDToU16Hex(u16 TC_char);	
// ��������  : HexתBCD
// hexֵ<100(0x64)
u8 HexToBCD(u8 TC_char);
// ��������  : HexתBCD
u16 HexToIntBCD(u8 HexVar);
// ��������  : 16λHexתBCD
// hexֵ<10000(0x2710)
u16 IntHexToIntBCD(u16 HexVar);
////////////////////////////////////////////////////////////////////////////////////
// ��������  : �ֽڵĴ�С�˵�ת��
// �������1 : Count  ��������ݸ���
// �������2 : P      ���������ָ��
void ByteEndian(u16 Count ,u8 *P);
// ��������  : ������char�ͺϳ�һ��int
u16 TwoCharToInt(u8 c1,u8 c2);
////////////////////////////////hex ascii ��ת��/////////////////////////////////////
// ��������  : "9"��ASCII����"A"��ASCII��ֵ���==7  ע����ĸ��ͳһΪ��д��ĸ��ASCII��ֵ 
u8 Hi_HexToAsc(u8 c1);
// ��������  : "9"��ASCII����"A"��ASCII��ֵ���==7  ע����ĸ��ͳһΪ��д��ĸ��ASCII��ֵ 
u8 Low_HexToAsc(u8 c1);
//��һ���ֽڵ�����ת��Ϊ����ASCII ��
void CharToAsc(u8 c1,u8 *buf);
//�������ֽڵ�����ת��Ϊ����ASCII ��
void u16ToAsc(u16 c1,u8 *buf);
//���ĸ��ֽڵ�����ת��Ϊ����ASCII ��
void u32ToAsc(u32 c1,u8 *buf);		
// ��������  : ��ascii ��'0'~'f' ת���� 0~f һ��byte
u8 AscToHex(u8 c1);
// ��������  : ������ASC��ϳ�һ��HEX�ַ�
u8 TwoAscTOHex(u8 TPA,u8 TPB);
// ��������  : ��ASC�뻺��ת����HEX������
void  BufAscToHex(u8 *PTemp,u16 Len);

int  BufStrToASC(char *PTemp,u16 size);

/////////////////////////////////////////////////////////////////////////////////////
//����д��ĸת��ΪСд
u8 CapitalTo(u8 cl);
//����д��ĸת��ΪСд
void BufCapitalTo(u8 *p,u16 len);

//////////////////////////////////У��ͼ���/////////////////////////////////////////
// ����У���
u16 BasicFunc_check_sum(u8 *data,u16 len);
//ͨ�õ�CRC16λУ����㷨
//Quantic ����ʽ0xA001,
//intData ��ʼֵ��0xFFFF
//prt			����ָ��
//Lenth		���ݳ���
u16 GeneralCRCFun(u16 Quantic,u16 intData,u8 *ptr,int Lenth);
//����ͼ���
u8 ModBusCRC16ChkSum(u8 Ta,u16 Quantic,u16 CRCInit,u8 *ptr,int Lenth);
//////////////////////////////////////////////////////////////////////////////////////
u16 BasicFunc_GetRadom(void);
//////////////////////////////////////////////////////////////////////////////////////
//u32 TimeTransform32bit(Date_Time *date);	




	

/* ���Դ�ӡ */	
#define DEBUG_NONE 		0 // ��������Դ�ӡ��Ϣ
#define DEBUG_ERROR 	1 // ���������Ϣ
#define DEBUG_WARN 		2 // ���������Ϣ
#define DEBUG_NOTICE 	3 // ���һ���Ƶ������Ϣ
#define DEBUG_INFO 		4 // ���һ���Ƶ������Ϣ


/* ���ݲ�ͬ�ȼ�������Դ�ӡ��Ϣ */
#define DEBUG_PRINT(level, fmt, args...)\
	do\
	{\
		if (level <= get_debug_level())\
		{\
			if (DEBUG_ERROR == level)\
			{\
				printf("[%s][%s][%d]"fmt, "ERROR", __FILE__, __LINE__, ##args);\
			}\
			else if (DEBUG_WARN == level)\
			{\
				printf("[%s][%s][%d]"fmt, "WARNING", __FILE__, __LINE__, ##args);\
			}\
			else if (DEBUG_NOTICE == level)\
			{\
				printf("[%s][%s][%d]"fmt, "NOTICE", __FILE__, __LINE__, ##args);\
			}\
			else if (DEBUG_INFO == level)\
			{\
				printf("[%s][%s][%d]"fmt, "INFO", __FILE__, __LINE__, ##args);\
			}\
		}\
	}while(0)
	
	
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

extern s32 get_debug_level(void);
extern void set_debug_level(s32 level);	
extern u32 get_build_date(s8 *p_date_buff, s32 buff_len);







#ifdef __cplusplus
}
#endif

#endif
