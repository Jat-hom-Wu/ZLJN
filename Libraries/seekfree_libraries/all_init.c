#include "all_init.h"

void GPIO_Initialization(void)
{
		//���йܽų�ʼ��	
		board_init();
		//������ʼ��
		key_init(KEY_1);		//��ʼ��P3_2, ��ֵ++
		key_init(KEY_2);		//��ʼ��P3_3, ��ֵ--
		key_init(KEY_3);		//��ʼ��P5_0, ��ֵ+50
		key_init(KEY_4);		//��ʼ��P5_1, ��ֵ-50
		//��������ʼ��
		gpio_mode(P3_0,GPIO);		//��ʼ��
		gpio_mode(P3_1,GPIO);
		gpio_mode(P4_6,GPIO);
		//��������ʼ��
		gpio_mode(P5_4,GPIO);		//��ʼ�����������ʼ���͵�ƽ
}

void PWM_Initialization(void)
{
		 pwm_init(PWM2_P22, 13000, 0);		//����ǰ��						
		 pwm_init(PWM2_P26, 13000, 0);		//��������
	   pwm_init(PWM1_P10, 13000, 0);		//�ҵ��ǰ��				
		 pwm_init(PWM1_P17, 13000, 0);		//�ҵ������
}

void Ctimer_Count_Initialization(void)
{
		ctimer_count_init(CTIM3_P04);		//�����ı�����
		ctimer_count_init(CTIM0_P34);		//�ҵ���ı�����
}

void UART_Initialization(void)
{
	  seekfree_wireless_init();		//����λ��ͨ��
}

void ICM_20602_Initialization(void)
{
		
		icm20602_init_spi();					//Ӳ��spi	icm20602
		get_icm20602_accdata_spi();		//��ȡ���ٶȼ�����
		get_icm20602_gyro_spi();			//��ȡ����������
}

void ADC_Initialization(void)
{
		//���ݲɼ�
		adc_init(ADC_P00,ADC_SYSclk_DIV_2);		//���вɼ�
		adc_init(ADC_P01,ADC_SYSclk_DIV_2);		//�ҵ�вɼ�
		adc_init(ADC_P02,ADC_SYSclk_DIV_2);		//������ icm20602 ���ٶ�
		adc_init(ADC_P03,ADC_SYSclk_DIV_2);		//���ٶȼ� �Ƕ�
		adc_init(ADC_P16,ADC_SYSclk_DIV_2);		//���ٶȼ� ��ģת��
		adc_init(ADC_P11,ADC_SYSclk_DIV_2);		//���ݵ�ѹ���
}

void OLED_Initialization(void)
{
		//OLED��ʼ��
		oled_init();
} 

void all_init()
{
	 GPIO_Initialization();					//IO�ڳ�ʼ��
	 PWM_Initialization();					//PWMģ���ʼ��
	 Ctimer_Count_Initialization();			//��������ʼ��
	 UART_Initialization();					//���ڳ�ʼ��
	 ICM_20602_Initialization();  		    //spi��ʼ��
	 ADC_Initialization();				    //ADC��ʼ��
	 OLED_Initialization();					//OLED��ʼ��

}	  

	