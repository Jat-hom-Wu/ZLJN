#include"headfile.h"

//��ֱ��н������������л�ˮƽ��У�����2��
uint8 round_flag,round_left,round_right,ON=0,OFF=1;	 //round_flag���������жϱ�־��	round_leftflag��⵽�󻷵���־��round_left�����������Ʊ�־
//float mid_AD_shuiping,left_AD_shuzhi,right_AD_shuzhi;
//float left_distance,right_distance;				//��������һ������߹���·��
	void round_judge(){
 	  if((round_flag == ON))
		{
      if(mid_AD_shuiping-MID_AD_SHUIPING_OFFEST>0&&left_AD_shuzhi-LEFT_AD_SHUZHI_OFFEST>0&&right_AD_shuzhi-RIGHT_AD_SHUZHI_OFFEST>0)                          //��м��ֵ
      {
          round_left=1;            //��⵽�󻷵�                                     
          round_flag = ON;
					left_huandao_distance_flag=1;																	// left_distance+=g_fCarSpeed;
      }
			
			 if(mid_AD_shuiping-MID_AD_SHUIPING_OFFEST>0&&left_AD_shuzhi-LEFT_AD_SHUZHI_OFFEST>0&&right_AD_shuzhi-RIGHT_AD_SHUZHI_OFFEST>0)                          //��м��ֵ
      {
          round_right=1;            //��⵽�һ���                                     
          round_flag = ON;
					right_huandao_distance_flag=1;																															//right_distance+=g_fCarSpeed;
      }
 
			
      if((round_left == 1)&&(round_flag == ON))			 //ȷ��������������Ϊ�󻷵�����п���
      {
       	//���ݾɵĵ��ֵ�������ı��µ��ֵ��ǿ��ת��
				//float Voltage_right_increase1;      			//�������
				//g_fRightVoltageSigma+=Voltage_right_increase1;    //�����ҵ�к��ֵ
				
				diangan_flag=1; 													//��Ϊ����ֱ���ѭ��
				
				if(left_huandao_distance>0) //�ж�������ȷ�������Ѿ���ȫ���뻷��
          {
						diangan_flag=0; 								//���ƣ���ˮƽ��п���ͨ������
          round_flag= OFF;
          
           round_left=2;									   //���뻷����ı�״̬λ��Ϊ����������׼��
         left_huandao_distance_flag=0;				//���ٽ���·������
          }
      }
	
			
			 if((round_right== 1)&&(round_flag == ON))			 //ȷ��������������Ϊ�һ�������п���
      {
       	//���ݾɵĵ��ֵ�������ı��µ��ֵ��ǿ��ת��
			//	float Voltage_left_increase1;      			//�������
			//	g_fLeftVoltageSigma+=Voltage_left_increase1;    //�������к��ֵ
				
          diangan_flag=1;                  //���ƣ�����ֱ��п��ƽ��뻷��
				
				
		  if(right_huandao_distance>0) //�ж�������ȷ�������Ѿ���ȫ���뻷��
          {
							diangan_flag=0; 															//���ƣ���ˮƽ��п��ƹ�����
          round_flag= OFF;
          
           round_right=2;									   //���뻷����ı�״̬λ��Ϊ����������׼��
         right_huandao_distance_flag=0;				//���ٽ���·������
          }
      }
 
		}
 
 
   if((round_flag==OFF)&&(round_left==2))  {
   if(mid_AD_shuiping-MID_AD_SHUIPING_OFFEST>0)										//��г��󵺼��
   {
        round_left=3;
        
   }
   if(round_left==3)   {
																										//ִ�г�������,���������Ǵ�С
																										//���ݾɵĵ��ֵ�������ı��µ��ֵ��ǿ��ת��
				//float Voltage_left_increase2;      			    //�������
				//g_fLeftVoltageSigma-=Voltage_left_increase2;//��С���к��ֵ  
				round_flag=0;                   							//������־��
	                     }  													
   
   }
	 
	 
	 if((round_flag==OFF)&&(round_right==2))  {
   if(mid_AD_shuiping-MID_AD_SHUIPING_OFFEST>0)										//��г��ҵ����
   {
        round_right=3;
        
   }
   if(round_right==3)  { 
																																								//ִ�г�������
																																								//���ݾɵĵ��ֵ�������ı��µ��ֵ��ǿ��ת��
				//float Voltage_right_increase2;      																		//�������
				//g_fRightVoltageSigma-=Voltage_right_increase2;    											//��С�ҵ�к��ֵ
				round_flag=0;  																														//������־��
	                     }
   }
  
 }