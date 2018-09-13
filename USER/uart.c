/*********************************************************************************
//������
//���ߣ����ƮԾ     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "include.h"
/*********************************************************************************
����������
*********************************************************************************/

/*********************************************************************************
��������������
*********************************************************************************/
NEAR struct Str_Uart2_Data Uart2;
//NEAR struct Str_Uart3_Data Uart3;
/*********************************************************************************
�ⲿ����������
*********************************************************************************/
/*********************************************************************************
˽�б���������
*********************************************************************************/ 
/*********************************************************************************
���Ա���������
*********************************************************************************/
/*********************************************************************************
�ڲ�����������
*********************************************************************************/
/*********************************************************************************
���ܴ��붨����
*********************************************************************************/

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void USART2_IRQHandler(void)                                                   //����2�жϷ�����
{
  if(USART_GetITStatus(USART2,USART_IT_RXNE)!= RESET)//USART_IT_RXNE�������ж�  
  {
    if(Uart2.Receive_Pend == FALSE)//�жϵ�ǰ�����ѱ�ȡ��
    {
      if(Uart2.Receive_Busy == FALSE)//�ж�Ϊ�״νӰ�
      {
        Uart2.Receive_Busy = TRUE;//��־����æ
        Uart2.Receive_Length = 0;//��������
        USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);            //�������߿����ж�
      }
      Uart2.R_Buffer[Uart2.Receive_Length] = USART_ReceiveData8 (USART2);//��������
      Uart2.Receive_Length++;
      if(Uart2.Receive_Length == UART2_RBUF_NUM)//�������ݳ����������򴮿���ǰ����
      {
        Uart2.Receive_Busy = FALSE;//��ս���æ
        Uart2.Receive_Pend = TRUE;//��־���ڹ���
      }
    }
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  }
  else if(USART_GetITStatus(USART2,USART_IT_IDLE)!= RESET)//������߿���
  {
    if(Uart2.Receive_Pend == FALSE)//�жϵ�ǰ�����ѱ�ȡ��
    {
      Uart2.Receive_Busy = FALSE;//��ս���æ
      Uart2.Receive_Pend = TRUE;//��־���ڹ���
      USART_ITConfig(USART2, USART_IT_IDLE, DISABLE);            //��ֹ���߿����ж�
    }
    USART_ClearITPendingBit (USART2, USART_IT_IDLE);
  }
//  else if(USART_GetITStatus(USART2,USART_IT_TC)!= RESET)//�жϷ������
//  {
//    if(Uart2.Send_Busy != FALSE)//�ж��Ƿ�Ϊ����æ״̬����ֹ�������ж�
//    {
//      if(Uart2.Send_Length != Uart2.Sent_Length)//�ж� δ�������
//      {
//        USART_SendData8 (USART2,Uart2.S_Buffer[Uart2.Sent_Length]);//�����ѷ����ֽڷ�����һ���ֽ�
//        Uart2.Sent_Length++;
//      
//      }
//      else //�ѷ������
//      {
//        USART_ITConfig(USART2,USART_IT_TC,DISABLE); //�رշ����ж�
//        Uart2.Send_Busy = FALSE;//���æ��־
//        #ifdef Uart2_Half_Duplex                                      //����ǰ�˫����������
//        Uart2_RX_ENABLE;
//        #endif
//      }
//    }
//    USART_ClearITPendingBit (USART2, USART_IT_TC);//�����ر�־
//  }
//  USART_ClearITPendingBit (USART2, USART_IT_TXE);//�����ر�־
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void USART2_TX_IRQHandler(void)                                                   //����2�����жϷ�����
{
  if(USART_GetITStatus(USART2,USART_IT_TC)!= RESET)//�жϷ������
  {
    if(Uart2.Send_Busy != FALSE)//�ж��Ƿ�Ϊ����æ״̬����ֹ�������ж�
    {
      if(Uart2.Send_Length != Uart2.Sent_Length)//�ж� δ�������
      {
        USART_SendData8 (USART2,Uart2.S_Buffer[Uart2.Sent_Length]);//�����ѷ����ֽڷ�����һ���ֽ�
        Uart2.Sent_Length++;     
      }
      else //�ѷ������
      {
        USART_ITConfig(USART2,USART_IT_TC,DISABLE); //�رշ����ж�
        Uart2.Send_Busy = FALSE;//���æ��־
        #ifdef Uart2_Half_Duplex                                      //����ǰ�˫����������
        Uart2_RX_ENABLE;
        #endif
      }
    }
    USART_ClearITPendingBit (USART2, USART_IT_TC);//�����ر�־
  }
  USART_ClearITPendingBit (USART2, USART_IT_TXE);//�����ر�־
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		    //
 Others:        //
*********************************************************************************/
ErrorStatus Uart2_Send(unsigned char *Send_Data,unsigned short Send_Lenght)
{
  ErrorStatus err;  //���巵��ֵ
  u16 i = 0;
  
  if(Uart2.Send_Length == Uart2.Sent_Length)//����ѷ�����ɣ�ǿ�Ʒ��Ϳ���
  {
    Uart2.Send_Busy = FALSE;
  }

  if(Uart2.Send_Busy == FALSE)                                  //����Ƿ�æ
  {
  #ifdef Uart2_Half_Duplex                                      //����ǰ�˫����رս���
    Uart2_RX_DISABLE;
  #endif
    USART_ITConfig(USART2,USART_IT_TC,ENABLE);                  //�򿪷����ж�
    if(Send_Lenght > UART2_SBUF_NUM)//���������������ֻ�ܰ�����󻺳巢��
      Uart2.Send_Length = UART2_SBUF_NUM;
    else
      Uart2.Send_Length = Send_Lenght;//���淢�ͳ���
    for(i=0;i<Send_Lenght;i++)
      Uart2.S_Buffer[i] = Send_Data[i];
    Uart2.Sent_Length = 0;          //�ѷ��͵ĳ�������
    Uart2.Send_Busy = TRUE;      //��־æ
    USART_SendData8 (USART2,Uart2.S_Buffer[0]);
    Uart2.Sent_Length++;
  }
  else
    err = ERROR;                                                //��ʾ���ʹ���
  return err; 
}

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		    //
 Others:        //
*********************************************************************************/
unsigned short Uart2_Receive(unsigned char *R_buff)
{
  unsigned short R_Len,i;
  if(Uart2.Receive_Pend != FALSE)
  {
    for(i=0;i<Uart2.Receive_Length;i++)
      R_buff[i] = Uart2.R_Buffer[i];
    Uart2.Receive_Pend = FALSE;
    R_Len = Uart2.Receive_Length;
  }
  else
    R_Len = 0;
  return R_Len;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
//void USART3_IRQHandler(void)                                                   //����3�жϷ�����
//{    
//  if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)//USART_IT_RXNE�������ж�  
//  {   
//    if(Uart3.Receive_Pend == FALSE)//�жϵ�ǰ�����ѱ�ȡ��
//    {
//      if(Uart3.Receive_Busy == FALSE)//�ж�Ϊ�״νӰ�
//      {
//        Uart3.Receive_Busy = TRUE;//��־����æ
//        Uart3.Receive_Length = 0;//��������
//        USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);            //�������߿����ж�
//      }
//      Uart3.R_Buffer[Uart3.Receive_Length] = USART_ReceiveData8(USART3);//��������
//      Uart3.Receive_Length++;
//      if(Uart3.Receive_Length == UART3_RBUF_NUM)//�������ݳ����������򴮿���ǰ����
//      {
//        Uart3.Receive_Busy = FALSE;//��ս���æ
//        Uart3.Receive_Pend = TRUE;//��־���ڹ���
//      }
//    }
//    USART_ClearITPendingBit (USART3, USART_IT_RXNE);
//  }
//  else if(USART_GetITStatus(USART3,USART_IT_IDLE) != RESET)//������߿���
//  {
//    if(Uart3.Receive_Pend == FALSE)//�жϵ�ǰ�����ѱ�ȡ��
//    {
//      Uart3.Receive_Busy = FALSE;//��ս���æ
//      Uart3.Receive_Pend = TRUE;//��־���ڹ���
//      USART_ITConfig(USART3, USART_IT_IDLE, DISABLE);            //��ֹ���߿����ж�
//    }
//    USART_ClearITPendingBit (USART3, USART_IT_IDLE);
//  }

//  else if(USART_GetITStatus(USART3,USART_IT_TC) == SET)//�жϷ������
//  {
//    if(Uart3.Send_Busy != FALSE)//�ж��Ƿ�Ϊ����æ״̬����ֹ�������ж�
//    {
//      if(Uart3.Send_Length != Uart3.Sent_Length)//�ж� δ�������
//      {
//        USART_SendData9 (USART3,Uart3.S_Buffer[Uart3.Sent_Length]);//�����ѷ����ֽڷ�����һ���ֽ�
//        Uart3.Sent_Length++;
//      }
//      else //�ѷ������
//      {
//        Uart3.Send_Busy = FALSE;//���æ��־
//        #ifdef Uart3_Half_Duplex                                      //����ǰ�˫����������
//        Uart3_RX_ENABLE;
//        #endif
//        USART_ITConfig(USART3,USART_IT_TC,DISABLE);                //��ֹ����3�����жϡ�  
//      }
//    }
//    USART_ClearITPendingBit (USART3, USART_IT_TC);//�����ر�־
//  }
//}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
//void USART3_TX_IRQHandler(void)                                                   //����3�����жϷ�����
//{
//  if(USART_GetITStatus(USART3,USART_IT_TC)!= RESET)//�жϷ������
//  {
//    if(Uart3.Send_Busy != FALSE)//�ж��Ƿ�Ϊ����æ״̬����ֹ�������ж�
//    {
//      if(Uart3.Send_Length != Uart3.Sent_Length)//�ж� δ�������
//      {
//        USART_SendData8 (USART3,Uart3.S_Buffer[Uart3.Sent_Length]);//�����ѷ����ֽڷ�����һ���ֽ�
//        Uart3.Sent_Length++;     
//      }
//      else //�ѷ������
//      {
//        USART_ITConfig(USART3,USART_IT_TC,DISABLE); //�رշ����ж�
//        Uart3.Send_Busy = FALSE;//���æ��־
//        #ifdef Uart3_Half_Duplex                                      //����ǰ�˫����������
//        Uart3_RX_ENABLE;
//        #endif
//      }
//    }
//    USART_ClearITPendingBit (USART3, USART_IT_TC);//�����ر�־
//  }
//  USART_ClearITPendingBit (USART3, USART_IT_TXE);//�����ر�־
//}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		    //
 Others:        //
*********************************************************************************/
//ErrorStatus Uart3_Send(unsigned char *Send_Data,unsigned short Send_Lenght)
//{
//  ErrorStatus err;  //���巵��ֵ
//  unsigned short i;
////  while(Uart3.Send_Busy != FALSE);
//   if(Uart3.Send_Length == Uart3.Sent_Length)//����ѷ�����ɣ�ǿ�Ʒ��Ϳ���
//  {
//    Uart3.Send_Busy = FALSE;
//  }
//  if(Uart3.Send_Busy == FALSE)                                  //����Ƿ�æ
//  {
//  #ifdef Uart3_Half_Duplex                                      //����ǰ�˫����رս���
//    Uart3_RX_DISABLE;
//  #endif
//    USART_ITConfig(USART3,USART_IT_TC,ENABLE);                //������3�����жϡ�  
//    if(Send_Lenght > UART3_SBUF_NUM)//���������������ֻ�ܰ�����󻺳巢��
//      Uart3.Send_Length = UART3_SBUF_NUM;
//    else
//      Uart3.Send_Length = Send_Lenght;//���淢�ͳ���
//    for(i=0;i<Send_Lenght;i++)
//      Uart3.S_Buffer[i] = Send_Data[i];
//    Uart3.Sent_Length = 0;          //�ѷ��͵ĳ�������
//    Uart3.Send_Busy = TRUE;      //��־æ
//    USART_SendData8(USART3,Uart3.S_Buffer[0]);
//    Uart3.Sent_Length++;
//  }
//  else
//    err = ERROR;                                                //��ʾ���ʹ���
//  return err;
//}

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		    //
 Others:        //
*********************************************************************************/
//unsigned short Uart3_Receive(unsigned char *R_buff)
//{
//  unsigned short R_Len,i;
//  if(Uart3.Receive_Pend != FALSE)
//  {
//    for(i=0;i<Uart3.Receive_Length;i++)
//      R_buff[i] = Uart3.R_Buffer[i];
//    Uart3.Receive_Pend = FALSE;
//    R_Len = Uart3.Receive_Length;
//  }
//  else
//    R_Len = 0;
//  return R_Len;
//}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

