#include "headfile.h"

//board.h文件中FOSC的值设置为0,则程序自动识别系统频率

/*board.h文件中FOSC的值设置不为0，则系统频率为FOSC的值，
在使用stc-isp工具下载程序的时候需要将IRC频率设置为FOSC的值*/
int ruku_flag=0;
int ruku_distance_flag=0;
int chuku_distance_flag=0;
int left_huandao_distance_flag=0,right_huandao_distance_flag=0;
uint8 ruku_distance,chuku_distance;
float left_huandao_distance,right_huandao_distance;


void zebra_judge()
{
if(ruku_flag==0){
    uint8 max,i,dif,n,k;		//max阈值，dif两点间差值，n为定义的超出阈值次数
	//DisableGlobalIRQ();			//关闭总中断
	//board_init();				//初始化寄存器
	ccd_init();					//TSL1401线阵CCD初始化
	//EnableGlobalIRQ();			//开启总中断
	ccd_collect();
    //发送图像到上位机
	ccd_send_data(UART_1, ccd_data);
	//采集完成标志位清除			
	//tsl1401_finish_flag = 0;
	for(i=0;i<120;i++)
		{
		 dif=ccd_data[i]-ccd_data[i+1];
		 if(dif>max)k=k+1;
		}
		if(k>n)
		{
																																									//开始转弯进入车库																																	
				turn_flag=1	;						//转向标志																				//通过转向环控制
																																									//设置状态位
			//distance+=g_fCarSpeed;				//计量路程
			ruku_distance_flag=1;									//开始计量路程
			ruku_flag=1;													//使不再判断斑马线
			
		}	
	}
}

		void chuku(){														//出库
			if(star_flag==1)
				{  //float distance;
					//distance+=g_fCarSpeed;					//计量路程
					chuku_distance_flag=1;
					if(chuku_distance>0){
																																				//打角      直接控制g_fDirectionControlOut???
																																				//是否需要电感阈值？
/////////////////方案一，直接改电感值转向////////////////////////////
							//g_fLeftVoltageSigma=0;
							//g_fRightVoltageSigma=0;
						
//////////////////方案二，改陀螺仪值的大小///////////////////////////
						turn_flag=2;
						
						
////////////////////方案三，直接改电机//////////////////////////
																																					//改标志位
					chuku_distance_flag=0;
					star_flag=2;     //改变全局变量标志位，使不再进入循环
					}
					
				}
		}

		
		void distance()
		{
			if(ruku_distance_flag==1){
					ruku_distance+=g_fCarSpeed;
			}
			
			if(chuku_distance_flag==1){
					chuku_distance+=g_fCarSpeed;
			}
			
			if(left_huandao_distance_flag==1){
					left_huandao_distance+=g_fCarSpeed;
			}
			
			if(right_huandao_distance_flag==1){
					right_huandao_distance_flag+=g_fCarSpeed;
			}
			
		
		}
		
		void stop_car()
		{
			if(ruku_distance>0){
		     
						stop_flag=1;           //电机制动标志
				ruku_distance_flag=0;				//清楚标志位，使不再进行路径积分
			}
		}
		
		
		
		