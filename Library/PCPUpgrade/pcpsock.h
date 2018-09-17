#ifndef __PCP_SOCK_H
#define   __PCP_SOCK_H

#include "include.h"

unsigned long int PCPSock_htonl(unsigned long int h);							//模拟htonl函数,本机字节序转网络字节序
unsigned long int PCPSock_ntohl(unsigned long int n);							//模拟ntohl函数,网络字节序转本机字节序
unsigned short int PCPSock_htons(unsigned short int h);						//模拟htons函数,本机字节序转网络字节序
unsigned short int PCPSock_ntohs(unsigned short int n);						//模拟ntohs函数,网络字节序转本机字节序

#endif /* __PCP_SOCK_H */
	
