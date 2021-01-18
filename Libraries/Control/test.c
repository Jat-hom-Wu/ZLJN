#include "test.h"

int GYRO_set=GYROSCOPE_OFFSET ;
int MMA_set=GRAVITY_OFFSET;
int speed_set=CAR_SPEED_SETfDelta;
float ANGLE_P=ANGLE_CONTROL_P;		//ֱ������ P
float ANGLE_D=ANGLE_CONTROL_D;		//ֱ������ D
float SPEED_P=SPEED_CONTROL_P;		//�ٶȿ��� P
float SPEED_I=SPEED_CONTROL_I;		//�ٶȿ��� I
float DIR_P=DIR_CONTROL_P;				//������� P
float DIR_D=DIR_CONTROL_D;				//������� D

void oled_view(void)
{
			int num = 0;
	
				/************ ������ 000 ************/	
				if((P30==0)&&(P31==0)&&(P46==0))
				{
						oled_fill(0x00);
						oled_p6x8str(40,2,"GYRO");		//��������ֵ
						oled_p6x8str(84,2,"ACC");			//���ٶȼ���ֵ
						oled_uint16(40,5,GYRO_set);
						oled_uint16(84,5,MMA_set);
						if(key_check(KEY_1) == KEY_DOWN)
							 {  
									pca_delay_ms(10);
									GYRO_set++;
									pca_delay_ms(200);
							 } 
							if(key_check(KEY_2) == KEY_DOWN)
							 {  
									pca_delay_ms(10);
									GYRO_set--;
									pca_delay_ms(200);
							 } 
							if(key_check(KEY_3) == KEY_DOWN)
							 {  
									pca_delay_ms(10);
									MMA_set++;
									pca_delay_ms(200);
							 } 
							 if(key_check(KEY_4) == KEY_DOWN)
							 {  
									pca_delay_ms(10);
									MMA_set--;
									pca_delay_ms(200);
							 }						
				}
				/************ ������ 001 **************/	
				if(P30==0&&P31==0&&P46==1)
				{
						oled_fill(0x00); 
						oled_p6x8str(40,2,"ANGLE_P");		  //ֱ������ P
						oled_p6x8str(84,2,"ANGLE_D");			//ֱ������ D
						oled_printf_float(0,0,ANGLE_P,2,3);
						oled_printf_float(0,0,ANGLE_D,2,3);
						if(key_check(KEY_5) == KEY_DOWN)
								 {  
										pca_delay_ms(500);
										num++;
										if(num==2)
										{
											speed_set =0;
											num=0;										
										}									
								 } 
						if(key_check(KEY_1) == KEY_DOWN)
							 {  
									pca_delay_ms(10);
									ANGLE_P++;
									pca_delay_ms(200);
							 } 
							if(key_check(KEY_2) == KEY_DOWN)
							 {  
									pca_delay_ms(10);
									ANGLE_P--;
									pca_delay_ms(200);
							 } 
							if(key_check(KEY_3) == KEY_DOWN)
							 {  
									pca_delay_ms(10);
									ANGLE_D++;
									pca_delay_ms(200);
							 } 
							 if(key_check(KEY_4) == KEY_DOWN)
							 {  
									pca_delay_ms(10);
									ANGLE_D--;
									pca_delay_ms(200);
							 }						
				}
				/************** ������ 010 ****************/	
				if(P30==0&&P31==1&&P46==0)
				{
						oled_fill(0x00); 
						oled_p6x8str(40,2,"SPEED_P");		  //�ٶȿ��� P
						oled_p6x8str(84,2,"SPEED_I");			//�ٶȿ��� I
						oled_printf_float(0,0,SPEED_P,2,3);
						oled_printf_float(0,0,SPEED_I,2,3);
						if(key_check(KEY_5) == KEY_DOWN)
								 {  
										pca_delay_ms(500);
										num++;
										if(num==2)
										{
											speed_set =0;
											num=0;										
										}									
								 } 
						if(key_check(KEY_1) == KEY_DOWN)
							 {  
									pca_delay_ms(10);
									SPEED_P++;
									pca_delay_ms(200);
							 } 
							if(key_check(KEY_2) == KEY_DOWN)
							 {  
									pca_delay_ms(10);
									SPEED_P--;
									pca_delay_ms(200);
							 } 
							if(key_check(KEY_3) == KEY_DOWN)
							 {  
									pca_delay_ms(10);
									SPEED_I++;
									pca_delay_ms(200);
							 } 
							 if(key_check(KEY_4) == KEY_DOWN)
							 {  
									pca_delay_ms(10);
									SPEED_I--;
									pca_delay_ms(200);
							 }						
				}
				/************** ������ 011 ****************/	
				if(P30==0&&P31==1&&P46==1)
				{
						oled_fill(0x00); 
						oled_p6x8str(40,2,"DIR_P");		  //������� P
						oled_p6x8str(84,2,"DIR_D");			//������� D
						oled_printf_float(0,0,DIR_P,2,3);
						oled_printf_float(0,0,DIR_D,2,3);
						if(key_check(KEY_5) == KEY_DOWN)
								 {  
										pca_delay_ms(500);
										num++;
										if(num==2)
										{
											speed_set =0;
											num=0;										
										}									
								 } 
						if(key_check(KEY_1) == KEY_DOWN)
							 {  
									pca_delay_ms(10);
									DIR_P++;
									pca_delay_ms(200);
							 } 
							if(key_check(KEY_2) == KEY_DOWN)
							 {  
									pca_delay_ms(10);
									DIR_P--;
									pca_delay_ms(200);
							 } 
							if(key_check(KEY_3) == KEY_DOWN)
							 {  
									pca_delay_ms(10);
									DIR_D++;
									pca_delay_ms(200);
							 } 
							 if(key_check(KEY_4) == KEY_DOWN)
							 {  
									pca_delay_ms(10);
									DIR_D--;
									pca_delay_ms(200);
							 }						
				}
					/************** ������ 100 ****************/
					//////��̬�ٶȵ��� ����λ��ͨ��////////////
				if(P30==1&&P31==0&&P46==0)
				{
						if(key_check(KEY_1) == KEY_DOWN)
							 {  
									pca_delay_ms(10);
									speed_set++;
									pca_delay_ms(200);
							 } 
							if(key_check(KEY_2) == KEY_DOWN)
							 {  
									pca_delay_ms(10);
									speed_set--;
									pca_delay_ms(200);
							 } 
							if(key_check(KEY_3) == KEY_DOWN)
							 {  
									pca_delay_ms(10);
									speed_set+=50;
									pca_delay_ms(200);
							 } 
							 if(key_check(KEY_4) == KEY_DOWN)
							 {  
									pca_delay_ms(10);
									speed_set-=50;
									pca_delay_ms(200);
							 }
							 
				}	  
				
}
