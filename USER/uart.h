/*********************************************************************************************************
//������
//���ߣ����ƮԾ    ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************************************/

#ifndef __UART_H__
#define __UART_H__
/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include "include.h"
/*********************************************************************************************************
�궨����
*********************************************************************************************************/
//#define Uart1_Half_Duplex           //����1��˫��
#define Uart1_Full_Duplex           //����1ȫ˫��

//#define Uart2_Half_Duplex           //����2��˫��
#define Uart2_Full_Duplex           //����2ȫ˫��

//#define Uart3_Half_Duplex           //����3��˫��
#define Uart3_Full_Duplex           //����3ȫ˫��

#define Uart1_RX_ENABLE           USART1->CR2 |= 0X04;
#define Uart1_RX_DISABLE          USART1->CR2 &= 0XFB;

#define Uart2_RX_ENABLE           USART2->CR2 |= 0X04;
#define Uart2_RX_DISABLE          USART2->CR2 &= 0XFB;

#define Uart3_RX_ENABLE           USART3->CR2 |= 0X04;
#define Uart3_RX_DISABLE          USART3->CR2 &= 0XFB;

#define UART1_SBUF_NUM            64//���ͻ�������С�궨��
#define UART1_RBUF_NUM            64//���ջ�������С�궨��

#define UART2_SBUF_NUM            200//���ͻ�������С�궨��
#define UART2_RBUF_NUM            256//���ջ�������С�궨��

#define UART3_SBUF_NUM            64//���ͻ�������С�궨��
#define UART3_RBUF_NUM            64//���ջ�������С�궨��
/*********************************************************************************************************
�������Ͷ���
*********************************************************************************************************/
struct Str_Uart2_Data                                                           //���ڴ��ڵļĴ����Ľṹ��
{
  unsigned char R_Buffer[UART2_RBUF_NUM];                                     //������ջ���
  unsigned char S_Buffer[UART2_SBUF_NUM];                                     //���ͻ�����
  bool Send_Busy;                                                              //����æ��־
  bool Receive_Busy;                                                           //����æ��־
  unsigned short Send_Length;                                                 //�������ݳ���
  unsigned short Sent_Length;                                                 //�ѷ��͵����ݳ���
  unsigned short Receive_Length;                                              //�������ݳ���
  bool Receive_Pend;                                                           //���ڽ��չ����־
};

struct Str_Uart3_Data                                                           //���ڴ��ڵļĴ����Ľṹ��
{
  unsigned char R_Buffer[UART3_RBUF_NUM];                                     //������ջ���
  unsigned char S_Buffer[UART3_SBUF_NUM];                                     //���ͻ�����
  bool Send_Busy;                                                              //����æ��־
  bool Receive_Busy;                                                           //����æ��־
  unsigned short Send_Length;                                                 //�������ݳ���
  unsigned short Sent_Length;                                                 //�ѷ��͵����ݳ���
  unsigned short Receive_Length;                                              //�������ݳ���
  bool Receive_Pend;                                                           //���ڽ��չ����־
};
/*********************************************************************************************************
�ⲿ����������
*********************************************************************************************************/
extern struct Str_Uart2_Data Uart2;
extern struct Str_Uart3_Data Uart3;
/*********************************************************************************************************
����������
*********************************************************************************************************/
ErrorStatus Uart2_Send(unsigned char *Send_Data,unsigned short Send_Lenght);
unsigned short Uart2_Receive(unsigned char *R_buff);
void USART2_IRQHandler(void);
void USART2_TX_IRQHandler(void);

ErrorStatus Uart3_Send(unsigned char *Send_Data,unsigned short Send_Lenght);
unsigned short Uart3_Receive(unsigned char *R_buff);
void USART3_IRQHandler(void);
void USART3_TX_IRQHandler(void);
/********************************************************************************************************/
#endif

/******************************************END********************************************************/
