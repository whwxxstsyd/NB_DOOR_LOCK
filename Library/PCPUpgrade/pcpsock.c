/**
  *********************************************************************************************************
  * @file    pcpsock.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-07-16
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "pcpsock.h"
	
/* -�����ʹ�С�˻���- */
#define BigLittleSwap16(A)		((((uint16_t)(A) & 0xFF00) >> 8) | (((uint16_t)(A) & 0x00FF) << 8))
/* -�����ʹ�С�˻���- */
#define BigLittleSwap32(A)		((((uint32_t)(A) & 0xFF000000) >> 24) | \
							 (((uint32_t)(A) & 0x00FF0000) >> 8 ) | \
							 (((uint32_t)(A) & 0x0000FF00) << 8 ) | \
							 (((uint32_t)(A) & 0x000000FF) << 24))

/**********************************************************************************************************
 @Function			static int checkCPUendian(void)
 @Description			checkCPUendian						: ���CPU��С��ģʽ
 @Input				void
 @Return				1								: ���
					0								: С��
**********************************************************************************************************/
static int checkCPUendian(void)
{
	union {
		unsigned long int i;
		unsigned char s[4];
	}c;
	
	c.i = 0x12345678;
	
	return (0x12 == c.s[0]);
}

/**********************************************************************************************************
 @Function			unsigned long int PCPSock_htonl(unsigned long int h)
 @Description			PCPSock_htonl						: ģ��htonl����,�����ֽ���ת�����ֽ���
 @Input				host	4byte
 @Return				net	4byte
**********************************************************************************************************/
unsigned long int PCPSock_htonl(unsigned long int h)
{
	return checkCPUendian() ? h : BigLittleSwap32(h);
}

/**********************************************************************************************************
 @Function			unsigned long int PCPSock_ntohl(unsigned long int n)
 @Description			PCPSock_ntohl						: ģ��ntohl����,�����ֽ���ת�����ֽ���
 @Input				net	4byte
 @Return				host	4byte
**********************************************************************************************************/
unsigned long int PCPSock_ntohl(unsigned long int n)
{
	return checkCPUendian() ? n : BigLittleSwap32(n);
}

/**********************************************************************************************************
 @Function			unsigned short int PCPSock_htons(unsigned short int h)
 @Description			PCPSock_htons						: ģ��htons����,�����ֽ���ת�����ֽ���
 @Input				host	2byte
 @Return				net	2byte
**********************************************************************************************************/
unsigned short int PCPSock_htons(unsigned short int h)
{
	return checkCPUendian() ? h : BigLittleSwap16(h);
}

/**********************************************************************************************************
 @Function			unsigned short int PCPSock_ntohs(unsigned short int n)
 @Description			PCPSock_ntohs						: ģ��ntohs����,�����ֽ���ת�����ֽ���
 @Input				net	2byte
 @Return				host	2byte
**********************************************************************************************************/
unsigned short int PCPSock_ntohs(unsigned short int n)
{
	return checkCPUendian() ? n : BigLittleSwap16(n);
}

/********************************************** END OF FLEE **********************************************/

