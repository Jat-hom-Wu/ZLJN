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
#include "zf_tim.h"				//��ʱ���ⲿ������������������ɼ���
#include "zf_pwm.h"
#include "zf_exti.h"			//�ⲿ�ж�
#include "zf_nvic.h"
#include "zf_pca_pit.h"
#include "zf_pca_delay.h"
#include "zf_eeprom.h"


#include "SEEKFREE_FONT.h"		//�����ļ�
#include "SEEKFREE_18TFT.h"		//1.8��TFT��Һ����
#include "SEEKFREE_IIC.h"		//IICͨѶЭ�飨ģ��IO��
#include "SEEKFREE_ICM20602.h" 		//����������
#include "SEEKFREE_TSL1401.h"		//����CCD
#include "SEEKFREE_IPS114_SPI.h"	//1.4��IPSҺ����
#include "SEEKFREE_OLED.h"
#include "SEEKFREE_VIRSCO.h"
#include "SEEKFREE_ABSOLUTE_ENCODER.h"
#include "SEEKFREE_WIRELESS.h"		//����ת����
#include "SEEKFREE_PRINTF.h"


/*******�Զ���ͷ�ļ�*******/
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