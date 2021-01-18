/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * All rights reserved.
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		control
 *					      破浪
 ********************************************************************************************************************/

#ifndef  __CONTROL__H
#define  __CONTROL__H

#include "headfile.h"

#define AMP1	    ADC_P00		//左水平电感采集
#define AMP2		  ADC_P01		//右水平电感采集
#define	DIR_L			P05
#define DIR_R			P47


/***********************直立控制参数********************/
#define GYROSCOPE_OFFSET 		0		//陀螺仪零偏量 
#define GRAVITY_OFFSET			0		//Z轴零偏量 
#define GRAVITY_ANGLE_RATIO		0.06		//需要计算  加速度计Z轴归一化系数
#define GYROSCOPE_ANGLE_RATIO	0.061		//需要计算  陀螺仪归一化系数
#define GRAVITY_ADJUST_TIME_CONSTANT	3  //1~4
#define GYROSCOPE_ANGLE_SIGMA_FREQUENCY		0.001				//陀螺仪角频率

#define CAR_ANGLE_SET		0
#define CAR_ANGLE_SPEED_SET		0

#define ANGLE_CONTROL_P		50		    //反复调整		
#define ANGLE_CONTROL_D		0.95		//反复调整												   
#define CAR_SPEED_CONSTANT		0.3		//需要计算 齿轮和速度的关系

#define  CAR_SPEED_SETfDelta     0

#define SPEED_CONTROL_P		0.5			//反复调整
#define SPEED_CONTROL_I		5			//反复调整

#define SPEED_CONTROL_COUNT 		20
#define DIRECTION_CONTROL_COUNT 		2

#define LEFT_RIGHT_MINIMUM		0		//电感最小值		
#define DIR_CONTROL_P		480		//需要测量
#define DIRECTION_COUNT		8		//电感采集次数
#define LEFT_AD_SHUIPING_OFFEST		0		//电磁水平检测左零点
#define RIGHT_AD_SHUIPING_OFFEST	0		//电磁水平检测右零点
#define LEFT_AD_SHUZHI_OFFEST     0   //电磁竖直检测左零点
#define RIGHT_AD_SHUZHI_OFFEST    0   //电磁竖直检测右零点
#define MID_AD_SHUIPING_OFFEST    0		//电磁检测中间电感值

#define DIRECTION_OFFSET		0		//车模转动陀螺仪零点，需要测量
#define DIR_CONTROL_D		0.6
#define MOTOR_OUT_DEAD_VAL		70		//死区电压 （左右可能不一样）
#define MOTOR_OUT_MAX		930		//需要实际计算（左右可能不一样）
#define MOTOR_OUT_MIN		-930	

#define corner_ruku 0

#define corner_chuku 0

/************************函数声明***********************/
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