/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * @file       		main
 * note				����
 ********************************************************************************************************************/

/****************����ͷ�ļ�********************/

#include "headfile.h"


/****************��������********************/
uint8 CONTROL_PERIOD = 5 ;
int stop_flag=0;
int g_n1MSEventCount = 0;
			int g_nSpeedControlCount;
			int g_nDirectionControlCount;
/**********ֱ������*****************/


void main()
{		
	
	
		if(1)			//������
		{	
			int star_flag=1;
			
		  DisableGlobalIRQ();			//�ر����ж�								  
		  all_init();


		//�ж� 1ms	      ��ʱ��0�ж�
		  TMOD = 0x00;
		  TH0 = 0xf8;
		  TL0 = 0x30;
		  TR0 = 1;
		  ET0 = 1;
													
			TH1 = 0xD8;					//��ʱ��1�жϣ��ж�5ms
			TL1 = 0xF0;
			TR1 = 1;
			ET1 = 1;
			
		  EnableGlobalIRQ();				//�����ж�
		  
			
			while(1)
		  {
				oled_view();
				OutPut_Vcan();
				
				
				
			}
	   }
	}


void TM0_Rountine(void)		interrupt 1		   //1ms���ж�	 5msһ����			 ����ʱ��0�жϣ�
{			

			
	
			if (g_n1MSEventCount >= CONTROL_PERIOD) {
				g_n1MSEventCount = 0;
				GetMotorPulse();   //��ȡ�����缫�������ֵ�����٣�					 
			}
			else if (g_n1MSEventCount == 1) {	 //��ȡ���ֵ��Ϊֱ����������׼����
			Rd_Ad_Value();
			
				} 
			else if(g_n1MSEventCount == 2) {							 //ֱ�����㷨
				AngleCalculate();
				AngleControl();
				MotorOutput();
			} 
			else if (g_n1MSEventCount == 3) {							//�ٶ����㷨
			g_nSpeedControlCount ++;
			SpeedControlOutput(); 		
			if(g_nSpeedControlCount >= SPEED_CONTROL_COUNT) {
			SpeedControl();		//100ms
			g_nSpeedControlCount = 0;
				}
			} 
			else if (g_n1MSEventCount == 4) {							   //�������㷨
			g_nDirectionControlCount ++;
			get_AD_Date();					//��ȡ��е���ֵ
			DirectionControlOutput();		
			if(g_nDirectionControlCount >= DIRECTION_CONTROL_COUNT) {
				DirectionControl();		//10ms
				g_nDirectionControlCount = 0;
						}
					}
			g_n1MSEventCount++ ;
}															


void TM1_Rountine(void) interrupt 3       //��ʱ5ms
	{
       
		chuku();
		round_judge();
			zebra_judge();					
		distance();
		stop_car();
		
		
}