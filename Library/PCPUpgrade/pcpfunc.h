#ifndef __PCP_FUNC_H
#define   __PCP_FUNC_H

#include "include.h"
#include "pcpconfig.h"

/* Ack Query Device Version */
typedef struct
{
	unsigned char						ResultCode;										//�����
	unsigned char						DeviceSoftVersion[16];							//��ǰ�汾��
}PCP_AckQueryDeviceVersionTypeDef;

/* Rck New Version Notice */
typedef struct
{
	unsigned char						PlatformSoftVersion[16];						//Ŀ�İ汾��
	unsigned short						UpgradePackSliceSize;							//��������Ƭ��С
	unsigned short						UpgradePackSliceNum;							//��������Ƭ����
	unsigned short						UpgradePackCheckCode;							//������У����
}PCP_RckNewVersionNoticeTypeDef;

/* Request Upgrade Package */
typedef struct
{
	unsigned char						PlatformSoftVersion[16];						//Ŀ�İ汾��
	unsigned short						UpgradePackSliceIndex;							//��������Ƭ���
}PCP_RequestUpgradePackageTypeDef;

/* Ack Request Upgrade Package */
typedef struct
{
	unsigned char						ResultCode;										//�����
	unsigned short						UpgradePackSliceIndex;							//��������Ƭ���
	unsigned char						pUpgradeData[1];								//��Ƭ����
}PCP_AckRequestUpgradePackageTypeDef;

/* Report Upgrades */
typedef struct
{
	unsigned char						ResultCode;										//�����	
	unsigned char						DeviceSoftVersion[16];							//��ǰ�汾��
}PCP_ReportUpgradesTypeDef;
	
PCP_StatusTypeDef PCP_Func_FrameCheck(unsigned char* DataFrame, short DataLength);				//PCP����֡���

PCP_StatusTypeDef PCP_Func_SelectMessageExecuteCmd(PCP_ClientsTypeDef* pClient);				//PCP�жϲ�ͬ��Ϣ�봦��ͬ����
PCP_StatusTypeDef PCP_Func_AckQueryDeviceVersion(PCP_ClientsTypeDef* pClient);					//PCP��ѯ�豸�汾
PCP_StatusTypeDef PCP_Func_AckNewVersionNotice(PCP_ClientsTypeDef* pClient);					//PCP�°汾֪ͨ
PCP_StatusTypeDef PCP_Func_AckRequestUpgradePackage(PCP_ClientsTypeDef* pClient);				//PCP����������Ӧ��
PCP_StatusTypeDef PCP_Func_AckReportDownloadStatus(PCP_ClientsTypeDef* pClient);				//PCP�ϱ�����������״̬Ӧ��
PCP_StatusTypeDef PCP_Func_AckPerformUpgrade(PCP_ClientsTypeDef* pClient);						//PCPִ������
PCP_StatusTypeDef PCP_Func_AckReportUpgrades(PCP_ClientsTypeDef* pClient);						//PCP�ϱ��������Ӧ��

PCP_StatusTypeDef PCP_Func_SelectUpgradeStatusExecuteCmd(PCP_ClientsTypeDef* pClient);			//PCP�жϲ�ͬ�����ϴ������봦��ͬ����
PCP_StatusTypeDef PCP_Func_QueryDeviceVersion(PCP_ClientsTypeDef* pClient);					//PCP��ѯ�豸�汾Ӧ���
PCP_StatusTypeDef PCP_Func_RequestUpgradePackage(PCP_ClientsTypeDef* pClient);					//PCP����������
PCP_StatusTypeDef PCP_Func_ReportDownloadStatus(PCP_ClientsTypeDef* pClient);					//PCP�ϱ�����������״̬
PCP_StatusTypeDef PCP_Func_ReportUpgrades(PCP_ClientsTypeDef* pClient);						//PCP�ϱ��������

#endif /* __PCP_FUNC_H */

