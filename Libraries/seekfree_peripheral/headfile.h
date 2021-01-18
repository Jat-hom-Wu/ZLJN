#ifndef __HEADFILE_H_
#define __HEADFILE_H_



#include "board.h"
#include "common.h"
#include "STC8Gxx.h"
#include "zf_uart.h"
#include "zf_gpio.h"
#include "zf_iic.h"
#include "zf_adc.h"
#include "zf_spi.h"
#include "zf_tim.h"				//定时器外部计数（编码器的脉冲采集）
#include "zf_pwm.h"
#include "zf_exti.h"			//外部中断
#include "zf_nvic.h"
#include "zf_pca_pit.h"
#include "zf_pca_delay.h"
#include "zf_eeprom.h"


#include "SEEKFREE_FONT.h"		//字体文件
#include "SEEKFREE_18TFT.h"		//1.8寸TFT（液晶）
#include "SEEKFREE_IIC.h"		//IIC通讯协议（模拟IO）
#include "SEEKFREE_ICM20602.h" 		//六轴陀螺仪
#include "SEEKFREE_TSL1401.h"		//线性CCD
#include "SEEKFREE_IPS114_SPI.h"	//1.4寸IPS液晶屏
#include "SEEKFREE_OLED.h"
#include "SEEKFREE_VIRSCO.h"
#include "SEEKFREE_ABSOLUTE_ENCODER.h"
#include "SEEKFREE_WIRELESS.h"		//无线转串口
#include "SEEKFREE_PRINTF.h"


/*******自定义头文件*******/
#include "Control.h"
#include "key.h"
#include "all_init.h"
#include "test.h"
#include "ccd_judge.h"
#include "yuansu.h"
#include "speedcontrol.h"


#include "isr.h"
#include <string.h>
#include "intrins.h"
#include <stdio.h>

#endif