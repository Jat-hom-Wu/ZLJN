/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * All rights reserved.
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		control
 *					      ����
 ********************************************************************************************************************/

#ifndef  __CONTROL__H
#define  __CONTROL__H

#include "headfile.h"

#define AMP1	    ADC_P00		//��ˮƽ��вɼ�
#define AMP2		  ADC_P01		//��ˮƽ��вɼ�
#define	DIR_L			P05
#define DIR_R			P47


/***********************ֱ�����Ʋ���********************/
#define GYROSCOPE_OFFSET 		0		//��������ƫ�� 
#define GRAVITY_OFFSET			0		//Z����ƫ�� 
#define GRAVITY_ANGLE_RATIO		0.06		//��Ҫ����  ���ٶȼ�Z���һ��ϵ��
#define GYROSCOPE_ANGLE_RATIO	0.061		//��Ҫ����  �����ǹ�һ��ϵ��
#define GRAVITY_ADJUST_TIME_CONSTANT	3  //1~4
#define GYROSCOPE_ANGLE_SIGMA_FREQUENCY		0.001				//�����ǽ�Ƶ��

#define CAR_ANGLE_SET		0
#define CAR_ANGLE_SPEED_SET		0

#define ANGLE_CONTROL_P		50		    //��������		
#define ANGLE_CONTROL_D		0.95		//��������												   
#define CAR_SPEED_CONSTANT		0.3		//��Ҫ���� ���ֺ��ٶȵĹ�ϵ

#define  CAR_SPEED_SETfDelta     0

#define SPEED_CONTROL_P		0.5			//��������
#define SPEED_CONTROL_I		5			//��������

#define SPEED_CONTROL_COUNT 		20
#define DIRECTION_CONTROL_COUNT 		2

#define LEFT_RIGHT_MINIMUM		0		//�����Сֵ		
#define DIR_CONTROL_P		480		//��Ҫ����
#define DIRECTION_COUNT		8		//��вɼ�����
#define LEFT_AD_SHUIPING_OFFEST		0		//���ˮƽ��������
#define RIGHT_AD_SHUIPING_OFFEST	0		//���ˮƽ��������
#define LEFT_AD_SHUZHI_OFFEST     0   //�����ֱ��������
#define RIGHT_AD_SHUZHI_OFFEST    0   //�����ֱ��������
#define MID_AD_SHUIPING_OFFEST    0		//��ż���м���ֵ

#define DIRECTION_OFFSET		0		//��ģת����������㣬��Ҫ����
#define DIR_CONTROL_D		0.6
#define MOTOR_OUT_DEAD_VAL		70		//������ѹ �����ҿ��ܲ�һ����
#define MOTOR_OUT_MAX		930		//��Ҫʵ�ʼ��㣨���ҿ��ܲ�һ����
#define MOTOR_OUT_MIN		-930	

#define corner_ruku 0

#define corner_chuku 0

/************************��������***********************/
void AngleCalculate(void);
void AngleControl(void);
void SpeedControl(void);
void SpeedControlOutput(void);
void DirectionControl(void);
void DirectionControlOutput(void);
void MotorOutput(void);
void MotorSpeedOut(void);
void SetMotorVoltage(float fLeftVoltage, float fRightVoltage);
void GetMotorPulse(void);
void Rd_Ad_Value(void);
void get_AD_Date(void);
void OutPut_Vcan(void);

extern float g_fLeftVoltageSigma,g_fRightVoltageSigma,mid_AD_shuiping,left_AD_shuzhi,right_AD_shuzhi,g_fCarSpeed,fe,fec;
extern int diangan_flag,turn_flag;


#endif