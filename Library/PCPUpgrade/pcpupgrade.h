#ifndef __PCP_UPGRADE_H
#define   __PCP_UPGRADE_H

#include "include.h"
#include "pcpconfig.h"

typedef __packed struct
{
	u8			Status;
	u32			BaseAddr;
	u16			BlockNum;
	u16			BlockLen;
	u16			DataLen;
	u32			SoftVer;
	u32			CheckCode;
}PCP_APPInfoTypeDef;

#define	UPGRADE_BACKUP_APP_WRITE_DISABLE		0												//���Ʊ���
#define	UPGRADE_BACKUP_APP_WRITE_ENABLE		1											//ǿ�Ʊ���
#define	UPGRADE_BACKUP_APP_WRITE_TYPE			UPGRADE_BACKUP_APP_WRITE_DISABLE						//����������ģʽ

#if 0

#define	UPGRADE_LOW_LIMIT_RSSI			-100												//�ź�ֵ��������
#define	UPGRADE_LOW_LIMIT_SNR			-100	
//�ź�������������
#else

#define	UPGRADE_LOW_LIMIT_RSSI			TCFG_EEPROM_GetUpgradeLimitRssi()						//�ź�ֵ��������
#define	UPGRADE_LOW_LIMIT_SNR			TCFG_EEPROM_GetUpgradeLimitSnr()						//�ź�������������

#endif

#define	UPGRADE_PACK_SLICE_BLOCK_SIZE		512												//����������Ƭ��С

PCP_ResultCodeTypeDef PCP_Upgrade_BackupCurrentAPP(PCP_ClientsTypeDef* pClient);											//�������ݵ�ǰAPP
PCP_ResultCodeTypeDef PCP_Upgrade_NewVersionNotice(PCP_ClientsTypeDef* pClient);											//�����°汾֪ͨ
PCP_ResultCodeTypeDef PCP_Upgrade_DataDownload(PCP_ClientsTypeDef* pClient, u16 SliceIndex, u8* UpgradeData, u16 UpgradeDataLength);	//����������
PCP_ResultCodeTypeDef PCP_Upgrade_DataAssemble(PCP_ClientsTypeDef* pClient);												//��������װУ��
PCP_ResultCodeTypeDef PCP_Upgrade_AfterUpdata(PCP_ClientsTypeDef* pClient);												//��������ʼ����

u8 CalculateSumCheckCode(u32 regCode, uint8_t* buf, uint32_t length);														//�Ӻ�У��
u8 CalculateStringToHex(u8 HighChar, u8 LowChar);																		//�ַ�ת��

#endif

