#include "headfile.h"

//board.h�ļ���FOSC��ֵ����Ϊ0,������Զ�ʶ��ϵͳƵ��

/*board.h�ļ���FOSC��ֵ���ò�Ϊ0����ϵͳƵ��ΪFOSC��ֵ��
��ʹ��stc-isp�������س����ʱ����Ҫ��IRCƵ������ΪFOSC��ֵ*/
int ruku_flag=0;
int ruku_distance_flag=0;
int chuku_distance_flag=0;
int left_huandao_distance_flag=0,right_huandao_distance_flag=0;
uint8 ruku_distance,chuku_distance;
float left_huandao_distance,right_huandao_distance;


void zebra_judge()
{
if(ruku_flag==0){
    uint8 max,i,dif,n,k;		//max��ֵ��dif������ֵ��nΪ����ĳ�����ֵ����
	//DisableGlobalIRQ();			//�ر����ж�
	//board_init();				//��ʼ���Ĵ���
	ccd_init();					//TSL1401����CCD��ʼ��
	//EnableGlobalIRQ();			//�������ж�
	ccd_collect();
    //����ͼ����λ��
	ccd_send_data(UART_1, ccd_data);
	//�ɼ���ɱ�־λ���			
	//tsl1401_finish_flag = 0;
	for(i=0;i<120;i++)
		{
		 dif=ccd_data[i]-ccd_data[i+1];
		 if(dif>max)k=k+1;
		}
		if(k>n)
		{
																																									//��ʼת����복��																																	
				turn_flag=1	;						//ת���־																				//ͨ��ת�򻷿���
																																									//����״̬λ
			//distance+=g_fCarSpeed;				//����·��
			ruku_distance_flag=1;									//��ʼ����·��
			ruku_flag=1;													//ʹ�����жϰ�����
			
		}	
	}
}

		void chuku(){														//����
			if(star_flag==1)
				{  //float distance;
					//distance+=g_fCarSpeed;					//����·��
					chuku_distance_flag=1;
					if(chuku_distance>0){
																																				//���      ֱ�ӿ���g_fDirectionControlOut???
																																				//�Ƿ���Ҫ�����ֵ��
/////////////////����һ��ֱ�Ӹĵ��ֵת��////////////////////////////
							//g_fLeftVoltageSigma=0;
							//g_fRightVoltageSigma=0;
						
//////////////////����������������ֵ�Ĵ�С///////////////////////////
						turn_flag=2;
						
						
////////////////////��������ֱ�Ӹĵ��//////////////////////////
																																					//�ı�־λ
					chuku_distance_flag=0;
					star_flag=2;     //�ı�ȫ�ֱ�����־λ��ʹ���ٽ���ѭ��
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
		     
						stop_flag=1;           //����ƶ���־
				ruku_distance_flag=0;				//�����־λ��ʹ���ٽ���·������
			}
		}
		
		
		
		