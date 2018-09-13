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
/*********************************************************************************
�ⲿ����������
*********************************************************************************/
/*********************************************************************************
˽�б���������
*********************************************************************************/ 
NEAR struct Get_Cal_Str Cal;
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
void Pulse_Acquire_Config(void)
{
  GPIO_Init(Cai1_PORT, Cai1_Pin , Cai1_Mode);   //��ʼ����������1
  GPIO_Init(Cai2_PORT, Cai2_Pin , Cai2_Mode);   //��ʼ����������2
  
  GPIO_Init(Weak_Up_PORT, Weak_Up_Pin , Weak_Up_Mode);   //��ʼ���������ػ���
  
  EXTI_SelectPort(EXTI_Port_D);
  EXTI_SetHalfPortSelection(EXTI_HalfPort_D_LSB ,ENABLE);
  EXTI_SetPortSensitivity(EXTI_Port_D,EXTI_Trigger_Falling); //GPIOD�ӿ� �½����ж�
  EXTI_ClearITPendingBit (EXTI_IT_PortD);                    //���жϱ�־λ
  ITC_SetSoftwarePriority(EXTID_H_IRQn , ITC_PriorityLevel_1);//�ж����ȼ��趨
  

  EXTI_SetPinSensitivity(EXTI_Pin_0, EXTI_Trigger_Falling);
  EXTI_ClearITPendingBit (EXTI_IT_Pin0);                    //���жϱ�־λ
  ITC_SetSoftwarePriority(EXTI0_IRQn , ITC_PriorityLevel_2);//�ж����ȼ��趨
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
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void ExtiD_Interrupt (void)                        //���ж�D
{
  if(RESET == Cai1 || RESET == Cai2)
  {
    if(RESET == Cai1 && RESET == Cai2)       //ǿ�Ÿ���
    {
      if(Cal.Error != INTERFERE) 
      {
        Cal.Error = INTERFERE;
        Cal.ErrorTimes = 0;
        BC95.Report_Bit = 1;
        if(BC95.Start_Process == BC95_POWER_DOWN)       //��һ�γ��֣������ϱ�
        {
          MeterParameter.DeviceStatus = RUN;                
          BC95.Start_Process = BC95_RECONNECT;
        }
      }
    }
    else
    { 
      if(Cal.Cal_State == CAL2 && RESET == Cai1) //��1���������� �� �ϴ�״̬Ϊ��2����������
      {
        Cal.Cal_State = CAL1;
        
        Cal.Error = NORMAL;
        Cal.ErrorTimes = 0;
      }
      else if(Cal.Cal_State == CAL1 && RESET == Cai2) //��2���������� �� �ϴ�״̬Ϊ��1����������
      {
        Cal.Water_Data.flow32++;
        Cal.Cal_State = CAL2;
               
        Cal.Error = NORMAL;
        Cal.ErrorTimes = 0;
      }
      else if(Cal.Cal_State == CAL1 && RESET == Cai1) //��1���������� �� �ϴ�״̬Ϊ��1����������
      {
        Cal.ErrorTimes++;
        if(Cal.ErrorTimes >= 3)
        {
          if(Cal.Error != HALL1) 
          {
            Cal.Error = HALL1;
            BC95.Report_Bit = 1;
            if(BC95.Start_Process == BC95_POWER_DOWN)
            {
              MeterParameter.DeviceStatus = RUN;      
              BC95.Start_Process = BC95_RECONNECT;
            }
          }
        }
      }
      else if(Cal.Cal_State == CAL2 && RESET == Cai2) //��2���������� �� �ϴ�״̬Ϊ��2����������
      {
        Cal.ErrorTimes++;
        if(Cal.ErrorTimes >= 3)
        {
          if(Cal.Error != HALL2) 
          {
            Cal.Error = HALL2;
            BC95.Report_Bit = 1;
            if(BC95.Start_Process == BC95_POWER_DOWN)
            {
              MeterParameter.DeviceStatus = RUN;      
              BC95.Start_Process = BC95_RECONNECT;
            }
          }
        }
      }
    }
  }
  EXTI_ClearITPendingBit (EXTI_IT_PortD);            //���жϱ�־λ
  EXTI_ClearITPendingBit (EXTI_IT_PortH);            //���жϱ�־λ
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
void Exti0_Interrupt (void)                        //���ж�F
{
  if(RESET == Weak_Up)
  {
    BC95.Report_Bit= 1;
    if(BC95.Start_Process == BC95_POWER_DOWN)
    {
      MeterParameter.DeviceStatus = RUN;  
      BC95.Start_Process = BC95_RECONNECT;    
    }
  }
  EXTI_ClearITPendingBit (EXTI_IT_Pin0);            //���жϱ�־λ
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

