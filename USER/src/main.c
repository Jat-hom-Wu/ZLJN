/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * @file       		main
 * note				破浪
 ********************************************************************************************************************/

/****************声明头文件********************/

#include "headfile.h"


/****************参数声明********************/
uint8 CONTROL_PERIOD = 5 ;
int stop_flag=0;
int g_n1MSEventCount = 0;
			int g_nSpeedControlCount;
			int g_nDirectionControlCount;
/**********直立控制*****************/


void main()
{		
	
	
		if(1)			//充电完成
		{	
			int star_flag=1;
			
		  DisableGlobalIRQ();			//关闭总中断								  
		  all_init();


		//中断 1ms	      定时器0中断
		  TMOD = 0x00;
		  TH0 = 0xf8;
		  TL0 = 0x30;
		  TR0 = 1;
		  ET0 = 1;
													
			TH1 = 0xD8;					//定时器1中断，中断5ms
			TL1 = 0xF0;
			TR1 = 1;
			ET1 = 1;
			
		  EnableGlobalIRQ();				//开总中断
		  
			
			while(1)
		  {
				oled_view();
				OutPut_Vcan();
				
				
				
			}
	   }
	}


void TM0_Rountine(void)		interrupt 1		   //1ms的中断	 5ms一周期			 （定时器0中断）
{			

			
	
			if (g_n1MSEventCount >= CONTROL_PERIOD) {
				g_n1MSEventCount = 0;
				GetMotorPulse();   //读取两个电极脉冲计数值（测速）					 
			}
			else if (g_n1MSEventCount == 1) {	 //读取电感值（为直立、方向做准备）
			Rd_Ad_Value();
			
				} 
			else if(g_n1MSEventCount == 2) {							 //直立类算法
				AngleCalculate();
				AngleControl();
				MotorOutput();
			} 
			else if (g_n1MSEventCount == 3) {							//速度类算法
			g_nSpeedControlCount ++;
			SpeedControlOutput(); 		
			if(g_nSpeedControlCount >= SPEED_CONTROL_COUNT) {
			SpeedControl();		//100ms
			g_nSpeedControlCount = 0;
				}
			} 
			else if (g_n1MSEventCount == 4) {							   //方向类算法
			g_nDirectionControlCount ++;
			get_AD_Date();					//获取电感的数值
			DirectionControlOutput();		
			if(g_nDirectionControlCount >= DIRECTION_CONTROL_COUNT) {
				DirectionControl();		//10ms
				g_nDirectionControlCount = 0;
						}
					}
			g_n1MSEventCount++ ;
}															


void TM1_Rountine(void) interrupt 3       //定时5ms
	{
       
		chuku();
		round_judge();
			zebra_judge();					
		distance();
		stop_car();
		
		
}