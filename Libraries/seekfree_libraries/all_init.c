#include "all_init.h"

void GPIO_Initialization(void)
{
		//所有管脚初始化	
		board_init();
		//按键初始化
		key_init(KEY_1);		//初始化P3_2, 数值++
		key_init(KEY_2);		//初始化P3_3, 数值--
		key_init(KEY_3);		//初始化P5_0, 数值+50
		key_init(KEY_4);		//初始化P5_1, 数值-50
		//拨码器初始化
		gpio_mode(P3_0,GPIO);		//初始化
		gpio_mode(P3_1,GPIO);
		gpio_mode(P4_6,GPIO);
		//蜂鸣器初始化
		gpio_mode(P5_4,GPIO);		//初始化，输出，初始化低电平
}

void PWM_Initialization(void)
{
		 pwm_init(PWM2_P22, 13000, 0);		//左电机前进						
		 pwm_init(PWM2_P26, 13000, 0);		//左电机后退
	   pwm_init(PWM1_P10, 13000, 0);		//右电机前进				
		 pwm_init(PWM1_P17, 13000, 0);		//右电机后退
}

void Ctimer_Count_Initialization(void)
{
		ctimer_count_init(CTIM3_P04);		//左电机的编码器
		ctimer_count_init(CTIM0_P34);		//右电机的编码器
}

void UART_Initialization(void)
{
	  seekfree_wireless_init();		//和上位机通信
}

void ICM_20602_Initialization(void)
{
		
		icm20602_init_spi();					//硬件spi	icm20602
		get_icm20602_accdata_spi();		//获取加速度计数据
		get_icm20602_gyro_spi();			//获取陀螺仪数据
}

void ADC_Initialization(void)
{
		//数据采集
		adc_init(ADC_P00,ADC_SYSclk_DIV_2);		//左电感采集
		adc_init(ADC_P01,ADC_SYSclk_DIV_2);		//右电感采集
		adc_init(ADC_P02,ADC_SYSclk_DIV_2);		//陀螺仪 icm20602 角速度
		adc_init(ADC_P03,ADC_SYSclk_DIV_2);		//加速度计 角度
		adc_init(ADC_P16,ADC_SYSclk_DIV_2);		//加速度计 车模转动
		adc_init(ADC_P11,ADC_SYSclk_DIV_2);		//电容电压检测
}

void OLED_Initialization(void)
{
		//OLED初始化
		oled_init();
} 

void all_init()
{
	 GPIO_Initialization();					//IO口初始化
	 PWM_Initialization();					//PWM模块初始化
	 Ctimer_Count_Initialization();			//编码器初始化
	 UART_Initialization();					//串口初始化
	 ICM_20602_Initialization();  		    //spi初始化
	 ADC_Initialization();				    //ADC初始化
	 OLED_Initialization();					//OLED初始化

}	  

	