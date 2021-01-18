#include"headfile.h"

//竖直电感进环，进环后切换水平电感（方法2）
uint8 round_flag,round_left,round_right,ON=0,OFF=1;	 //round_flag进出环岛判断标志，	round_leftflag检测到左环岛标志，round_left环岛操作控制标志
//float mid_AD_shuiping,left_AD_shuzhi,right_AD_shuzhi;
//float left_distance,right_distance;				//进入左或右环岛后走过的路程
	void round_judge(){
 	  if((round_flag == ON))
		{
      if(mid_AD_shuiping-MID_AD_SHUIPING_OFFEST>0&&left_AD_shuzhi-LEFT_AD_SHUZHI_OFFEST>0&&right_AD_shuzhi-RIGHT_AD_SHUZHI_OFFEST>0)                          //电感检测值
      {
          round_left=1;            //检测到左环岛                                     
          round_flag = ON;
					left_huandao_distance_flag=1;																	// left_distance+=g_fCarSpeed;
      }
			
			 if(mid_AD_shuiping-MID_AD_SHUIPING_OFFEST>0&&left_AD_shuzhi-LEFT_AD_SHUZHI_OFFEST>0&&right_AD_shuzhi-RIGHT_AD_SHUZHI_OFFEST>0)                          //电感检测值
      {
          round_right=1;            //检测到右环岛                                     
          round_flag = ON;
					right_huandao_distance_flag=1;																															//right_distance+=g_fCarSpeed;
      }
 
			
      if((round_left == 1)&&(round_flag == ON))			 //确定环岛开启，且为左环岛后进行控制
      {
       	//根据旧的电磁值，暴力改变新电磁值，强行转向
				//float Voltage_right_increase1;      			//电磁增量
				//g_fRightVoltageSigma+=Voltage_right_increase1;    //增大右电感后的值
				
				diangan_flag=1; 													//变为由竖直电感循迹
				
				if(left_huandao_distance>0) //判断条件，确定车子已经完全进入环岛
          {
						diangan_flag=0; 								//控制，由水平电感控制通过环岛
          round_flag= OFF;
          
           round_left=2;									   //进入环岛后改变状态位，为后续出岛做准备
         left_huandao_distance_flag=0;				//不再进行路径积分
          }
      }
	
			
			 if((round_right== 1)&&(round_flag == ON))			 //确定环岛开启，且为右环岛后进行控制
      {
       	//根据旧的电磁值，暴力改变新电磁值，强行转向
			//	float Voltage_left_increase1;      			//电磁增量
			//	g_fLeftVoltageSigma+=Voltage_left_increase1;    //增大左电感后的值
				
          diangan_flag=1;                  //控制，由竖直电感控制进入环岛
				
				
		  if(right_huandao_distance>0) //判断条件，确定车子已经完全进入环岛
          {
							diangan_flag=0; 															//控制，由水平电感控制过环岛
          round_flag= OFF;
          
           round_right=2;									   //进入环岛后改变状态位，为后续出岛做准备
         right_huandao_distance_flag=0;				//不再进行路径积分
          }
      }
 
		}
 
 
   if((round_flag==OFF)&&(round_left==2))  {
   if(mid_AD_shuiping-MID_AD_SHUIPING_OFFEST>0)										//电感出左岛检测
   {
        round_left=3;
        
   }
   if(round_left==3)   {
																										//执行出岛控制,限制陀螺仪大小
																										//根据旧的电磁值，暴力改变新电磁值，强行转向
				//float Voltage_left_increase2;      			    //电磁增量
				//g_fLeftVoltageSigma-=Voltage_left_increase2;//减小左电感后的值  
				round_flag=0;                   							//环岛标志打开
	                     }  													
   
   }
	 
	 
	 if((round_flag==OFF)&&(round_right==2))  {
   if(mid_AD_shuiping-MID_AD_SHUIPING_OFFEST>0)										//电感出右岛检测
   {
        round_right=3;
        
   }
   if(round_right==3)  { 
																																								//执行出岛控制
																																								//根据旧的电磁值，暴力改变新电磁值，强行转向
				//float Voltage_right_increase2;      																		//电磁增量
				//g_fRightVoltageSigma-=Voltage_right_increase2;    											//减小右电感后的值
				round_flag=0;  																														//环岛标志打开
	                     }
   }
  
 }