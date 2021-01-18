#include "Control.h"

float VOLTAGE_GRAVITY,VOLTAGE_GYRO;		//加速度计Z轴获取数据，陀螺仪角速度获取数据
float g_fGravityAngle,g_fGyroscopeAngleSpeed;		//加速度计Z轴最终数据,陀螺仪角速度最终数据
unsigned int AD_average;
unsigned int AD_data[10];		//存放陀螺仪数据的
float g_fCarAngle;		
float g_fGyroscopeAngleIntegral=0;		//积分后的角度
float g_fAngleControlOut, g_fSpeedControlOut,g_fDirectionControlOut;		//直立控制输出量,速度控制输出量,方向控制输出量
float g_fCarSpeed;		
int g_nLeftMotorPulseSigma,g_nRightMotorPulseSigma,DIR_CONTROL_D_VALUE;		//左编码器脉冲数，右编码器脉冲数,车模转动测量陀螺仪
float g_fSpeedControlIntegral;		
float g_fSpeedControlOutOld, g_fSpeedControlOutNew;
float g_fLeftVoltageSigma,g_fRightVoltageSigma,turn;		//电磁检测左脉冲，电磁检测右脉冲,转向 
float g_fDirectionControlOutOld , g_fDirectionControlOutNew;
float g_fLeftMotorOut , g_fRightMotorOut;
float mid_AD_shuiping,left_AD_shuzhi,right_AD_shuzhi;   //左，中，右竖直电感值
float left_AD_shuiping,right_AD_shuiping;                //左右水平电感值
int  diangan_flag=0,turn_flag=0;
float fe,fec;																							//电感偏差值
float SPEED_CONTROL_OUT_MAX,SPEED_CONTROL_OUT_MIN;				//积分饱和

float OutData[4] = { 0 };

extern int GYRO_set;
extern int MMA_set;
extern int speed_set;
extern float ANGLE_P,ANGLE_D;
extern float SPEED_P,SPEED_I;
extern float DIR_P,DIR_D;



//**************************************************************************	  
//  @brief      AD采集加速度计Z轴、陀螺仪
//  @return     void
//  函数结果：  采集到的Z轴、陀螺仪的数据
//**************************************************************************
void Rd_Ad_Value(void){

	int j=0;

    VOLTAGE_GRAVITY = icm_acc_z;   //Z 加速度计测（角度）	 
       
    for(j=0;j<10;j++)
    {
      VOLTAGE_GYRO = icm_gyro_y;    // gyro陀螺仪（角速度）	 （方向可能需要改动）
      AD_data[j]= VOLTAGE_GYRO;
    }
      AD_average=(AD_data[1]+AD_data[2]+AD_data[3]+AD_data[4]+AD_data[5]+AD_data[6]+AD_data[7]+AD_data[8])/8;
      VOLTAGE_GYRO =  AD_average;

}

//**************************************************************************
//  @brief      角度计算函数
//  @return     void
//  参数：GRAVITY_OFFSET：加速度计Z轴零偏值
//		  GRAVITY_ANGLE_RATIO：归一化比例系数											   
//		  GRAVITY_ADJUST_TIME_CONSTANT  时间校正系数      定时器时间 单位s
//  函数结果：直立情况下的角度
//  备注：参考清华源码
//**************************************************************************
void AngleCalculate(void) 
{
	   
		float fDeltaValue;
	
		g_fGravityAngle = (VOLTAGE_GRAVITY - MMA_set) *GRAVITY_ANGLE_RATIO; 		//加速度计 Z

		g_fGyroscopeAngleSpeed = (VOLTAGE_GYRO - GYRO_set) *GYROSCOPE_ANGLE_RATIO;	 //陀螺仪角速度

		g_fCarAngle = g_fGyroscopeAngleIntegral;		//最终融合角度		
		fDeltaValue = (g_fGravityAngle - g_fCarAngle) /GRAVITY_ADJUST_TIME_CONSTANT;	//时间系数矫正

		g_fGyroscopeAngleIntegral += (g_fGyroscopeAngleSpeed + fDeltaValue) *GYROSCOPE_ANGLE_SIGMA_FREQUENCY;   	//融合角度
}

//**************************************************************************
//  @brief      直立控制函数
//  @return     void
//  函数结果：	直立情况下的输出量
//  备注：参考清华源码
//**************************************************************************
void AngleControl(void) {

		float fValue;

		fValue = (CAR_ANGLE_SET - g_fCarAngle) * ANGLE_P +
		  		 (CAR_ANGLE_SPEED_SET - g_fGyroscopeAngleSpeed) *ANGLE_D;

/*
	if(fValue > ANGLE_CONTROL_OUT_MAX)		//输出限幅
		fValue = ANGLE_CONTROL_OUT_MAX;

		else if(fValue < ANGLE_CONTROL_OUT_MIN)									  
		fValue = ANGLE_CONTROL_OUT_MIN;	  
*/

		g_fAngleControlOut = fValue;
}

//**************************************************************************
//  @brief      获取小车速度
//  函数说明	  单位时间内采集到的脉冲数即可代表小车速度
//  @return     void
//  函数结果：	脉冲速度
//
//**************************************************************************
void GetMotorPulse(void)
{
		if(DIR_L == 1)
		{
				g_nLeftMotorPulseSigma = ctimer_count_read(CTIM3_P04); 		
		}
		else
		{
				g_nLeftMotorPulseSigma = -ctimer_count_read(CTIM3_P04); 
		}
		if(DIR_R == 1)
		{
				g_nRightMotorPulseSigma = ctimer_count_read(CTIM0_P34); 		
		}
		else
		{
				g_nRightMotorPulseSigma = -ctimer_count_read(CTIM0_P34); 
		}
	
	  ctimer_count_clean(CTIM3_P04);
	  ctimer_count_clean(CTIM0_P34);
}


//**************************************************************************
//  @brief      速度控制
//  @return     void
//  参数		CAR_SPEED_SETfDelta  ：给定速度
//  函数结果：	速度计算
//  备注：参考清华源码
//**************************************************************************
void SpeedControl(void) {

		float fDelta;															
		float fP, fI;													 

		g_fCarSpeed = (g_nLeftMotorPulseSigma + g_nRightMotorPulseSigma) / 2;

		g_nLeftMotorPulseSigma = g_nRightMotorPulseSigma = 0;		//脉冲清零
		g_fCarSpeed *= CAR_SPEED_CONSTANT;		//脉冲数平均值乘以比例值
		speed_now = g_fCarSpeed*20.1;					//转换为速度cm/s
				
		fDelta = speed_set - g_fCarSpeed;
		fP = fDelta * SPEED_P;
		fI = fDelta * SPEED_I;

		g_fSpeedControlIntegral += fI;		//积分	下面有饱和处理，防止积分饱和
	
		if(g_fSpeedControlIntegral > SPEED_CONTROL_OUT_MAX)	   //防止积分饱和
        g_fSpeedControlIntegral = SPEED_CONTROL_OUT_MAX;
   if(g_fSpeedControlIntegral < SPEED_CONTROL_OUT_MIN)  	
         g_fSpeedControlIntegral = SPEED_CONTROL_OUT_MIN;

		g_fSpeedControlOutOld = g_fSpeedControlOutNew;
		g_fSpeedControlOutNew = fP + g_fSpeedControlIntegral;		//最新速度计算值
}

//**************************************************************************
//  @brief      速度平滑输出函数
//  @return     void
//  参数		SPEED_CONTROL_COUNT	：差值输出平均到20个周期
//  函数结果：	速度输出值
//  备注：参考清华源码
//**************************************************************************
void SpeedControlOutput(void) {

		float fValue;

		fValue = g_fSpeedControlOutNew - g_fSpeedControlOutOld;		//两次速度计算差值
		g_fSpeedControlOut = fValue *  1 / SPEED_CONTROL_COUNT + g_fSpeedControlOutOld;
}

//**************************************************************************
//  @brief      AD采集电感线圈
//  @return     void
//  函数结果：	速度输出值
//**************************************************************************
void get_AD_Date(void)
{  
		float vr[DIRECTION_COUNT];    
    int  i,j;
    float vracc,change;
	
		//float mid_AD_shuiping=0;
    //float left_AD_shuiping=0;
    //float right_AD_shuiping=0;
	  //float left_AD_shuzhi=0;
		//float right_AD_shuzhi=0;
		int temp=0;
   	 
////////////////////////中间水平电感///////////////////////采集8次
		for(i=0;i<DIRECTION_COUNT;i++){
			vr[i]=adc_once(ADC_P12,ADC_10BIT);
			 for(j=0;j<i;j++)
      {
        if(vr[i-j]>vr[i-j-1])        //冒泡法从大到小对采样值排序
        {
          change   = vr[i-j-1];
          vr[i-j-1]= vr[i-j];
          vr[i-j]  = change;
        }
      }
			for(i=1;i<DIRECTION_COUNT-1;i++)    //排好序后，剔除1个较小值和1个较最大值
      vracc+= vr[i];                //后，再求和取平均。
      mid_AD_shuiping=vracc/(DIRECTION_COUNT-2);
      vracc=0;
			
		}
	////////////////////////左水平电感///////////////////////采集8次
		for(i=0;i<DIRECTION_COUNT;i++){
			vr[i]=adc_once(AMP1,ADC_10BIT);
			 for(j=0;j<i;j++)
      {
        if(vr[i-j]>vr[i-j-1])        //冒泡法从大到小对采样值排序
        {
          change   = vr[i-j-1];
          vr[i-j-1]= vr[i-j];
          vr[i-j]  = change;
        }
      }
			for(i=1;i<DIRECTION_COUNT-1;i++)    //排好序后，剔除1个较小值和1个较最大值
      vracc+= vr[i];                //后，再求和取平均。
      left_AD_shuiping=vracc/(DIRECTION_COUNT-2);
      vracc=0;
			
		}
	////////////////////////右水平电感///////////////////////
    for(i=0;i<DIRECTION_COUNT;i++){
			vr[i]=adc_once(AMP2,ADC_10BIT);
			 for(j=0;j<i;j++)
      {
        if(vr[i-j]>vr[i-j-1])        //冒泡法从大到小对采样值排序
        {
          change   = vr[i-j-1];
          vr[i-j-1]= vr[i-j];
          vr[i-j]  = change;
        }
      }
			for(i=1;i<DIRECTION_COUNT-1;i++)    //排好序后，剔除1个较小值和1个较最大值
      vracc+= vr[i];                //后，再求和取平均。
      right_AD_shuiping=vracc/(DIRECTION_COUNT-2);
      vracc=0;
			
		}
	////////////////////////左竖直电感///////////////////////
		for(i=0;i<DIRECTION_COUNT;i++){
			vr[i]=adc_once(ADC_P02,ADC_10BIT);
			 for(j=0;j<i;j++)
      {
        if(vr[i-j]>vr[i-j-1])        //冒泡法从大到小对采样值排序
        {
          change   = vr[i-j-1];
          vr[i-j-1]= vr[i-j];
          vr[i-j]  = change;
        }
      }
			for(i=1;i<DIRECTION_COUNT-1;i++)    //排好序后，剔除1个较小值和1个较最大值
      vracc+= vr[i];                //后，再求和取平均。
      left_AD_shuzhi=vracc/(DIRECTION_COUNT-2);
      vracc=0;
			
		}
		////////////////////////右竖直电感///////////////////////
		for(i=0;i<DIRECTION_COUNT;i++){
			vr[i]=adc_once(ADC_P03,ADC_10BIT);
			 for(j=0;j<i;j++)
      {
        if(vr[i-j]>vr[i-j-1])        //冒泡法从大到小对采样值排序
        {
          change   = vr[i-j-1];
          vr[i-j-1]= vr[i-j];
          vr[i-j]  = change;
        }
      }
			for(i=1;i<DIRECTION_COUNT-1;i++)    //排好序后，剔除1个较小值和1个较最大值
      vracc+= vr[i];                //后，再求和取平均。
      right_AD_shuzhi=vracc/(DIRECTION_COUNT-2);
      vracc=0;
	////////////////////////////////////////////////////		
		}
    for(i=0;i<DIRECTION_COUNT;i++)		//采集8次
    { 
			//left_AD_Date+=adc_once(AMP1,ADC_10BIT);
      //right_AD_Date+=adc_once(AMP2,ADC_10BIT);
      temp+= icm_gyro_x;						//需改动  该陀螺仪方向
    }
			
			if(diangan_flag==0)
				{
      g_fLeftVoltageSigma +=(int) ( left_AD_shuiping- LEFT_AD_SHUIPING_OFFEST);
      g_fRightVoltageSigma +=(int) (right_AD_shuiping - RIGHT_AD_SHUIPING_OFFEST);
				}
				else {
				 g_fLeftVoltageSigma +=(int) ( left_AD_shuzhi- LEFT_AD_SHUZHI_OFFEST);
				g_fRightVoltageSigma +=(int) (right_AD_shuzhi - RIGHT_AD_SHUZHI_OFFEST);
				}
/////////////////////////////转向环控制//////////////////////////////////////////////////
			if(turn_flag==0){
					DIR_CONTROL_D_VALUE +=(int) (temp/DIRECTION_COUNT - DIRECTION_OFFSET); 
			}
			else if (turn_flag==1){
					DIR_CONTROL_D_VALUE +=(int) (temp/DIRECTION_COUNT - DIRECTION_OFFSET+corner_ruku); 								//入库转向
						turn_flag=0;
			}
			else if (turn_flag==2){
					DIR_CONTROL_D_VALUE +=(int) (temp/DIRECTION_COUNT - DIRECTION_OFFSET+corner_chuku); 							//出库转向
						turn_flag=0;
			}
}
//**************************************************************************
//  @brief      方向计算函数
//  @return     void
//  参数		DIRECTION_CONTROL_COUNT ：累加次数 2
//				DIR_CONTROL_P：		P值
//				DIR_CONTROL_D：		陀螺仪D值
//				DIRECTION_OFFSET：	车模转动（车模转动陀螺仪零点偏移）
//				DIR_CONTROL_D_VALUE：	陀螺仪转动测量
//  函数结果：	速度输出值
//  备注：参考清华源码
//**************************************************************************
void DirectionControl(void) {

		float fLeftRightAdd, fLeftRightSub, fValue=0;
		int nLeft, nRight ,turn ;	
		float mid=mid_AD_shuiping-MID_AD_SHUIPING_OFFEST;

		nLeft = (int)(g_fLeftVoltageSigma /= DIRECTION_CONTROL_COUNT);			//平均后
		nRight = (int)(g_fRightVoltageSigma /= DIRECTION_CONTROL_COUNT);
		turn = (int)(DIR_CONTROL_D_VALUE/= DIRECTION_CONTROL_COUNT);

		g_fLeftVoltageSigma = 0;
		g_fRightVoltageSigma = 0;
		DIR_CONTROL_D_VALUE = 0;

		fLeftRightAdd = nLeft + nRight;
		fLeftRightSub = nLeft - nRight;

		fec=fe;
		fe=150*fLeftRightSub/(fLeftRightAdd+mid);									//还有小白的电感定位方案和国一双车电感定位方案，150数值可调？
		
		g_fDirectionControlOutOld = g_fDirectionControlOutNew;		//这个周期的老等于上个周期的新

		if(fLeftRightAdd < LEFT_RIGHT_MINIMUM) {

		g_fDirectionControlOutNew = 0;

		} else {
			speed_fuzzy_mem_cal_forecast();
			speed_fuzzy_query_forecast();
			speed_fuzzy_solve_forecast();
			speedcontrol_forecast();

			speed_fuzzy_mem_cal_fe();
			speed_fuzzy_query_fe();
			speed_fuzzy_solve_fe();
			
		fValue = fe * control_P ;
			  
		turn *= DIR_D;

		fValue += turn;

		g_fDirectionControlOutNew = fValue;			 
       }
}

//**************************************************************************
//  @brief      方向平滑输出
//  @return     void
//  函数结果：	方向输出
//  备注：参考清华源码
//**************************************************************************
void DirectionControlOutput(void) {

		float fValue;

		fValue = g_fDirectionControlOutNew - g_fDirectionControlOutOld;
		g_fDirectionControlOut = fValue *  1 / DIRECTION_CONTROL_COUNT + g_fDirectionControlOutOld;
}

//**************************************************************************
//  @brief      电机输出控制
//  @return     void
//  函数结果：	电机输出值
//  备注：参考清华源码
//**************************************************************************
void MotorOutput(void) {

		float fLeft, fRight;

		fLeft = g_fAngleControlOut -
		g_fSpeedControlOut -
		g_fDirectionControlOut;
		fRight = g_fAngleControlOut -
		g_fSpeedControlOut +
		g_fDirectionControlOut;

		g_fLeftMotorOut = fLeft;
		g_fRightMotorOut = fRight;
		MotorSpeedOut();
}

//**************************************************************************
//  @brief      电机PWM计算函数
//  @return     void
//  参数		MOTOR_OUT_DEAD_VAL：		死区补偿电压
//  函数结果：	电机PWM输出值
//  备注：参考清华源码
//**************************************************************************
void MotorSpeedOut(void) {

		float fLeftVal, fRightVal;

		fLeftVal = g_fLeftMotorOut;
		fRightVal = g_fRightMotorOut;

		if(fLeftVal > 0)
		fLeftVal += MOTOR_OUT_DEAD_VAL;
		else if(fLeftVal < 0)
		fLeftVal -= MOTOR_OUT_DEAD_VAL;

		if(fRightVal > 0)
		fRightVal += MOTOR_OUT_DEAD_VAL;
		else if(fRightVal < 0)
		fRightVal -= MOTOR_OUT_DEAD_VAL;

		if(fLeftVal > MOTOR_OUT_MAX) 		//限幅
		fLeftVal = MOTOR_OUT_MAX;
		if(fLeftVal < MOTOR_OUT_MIN)
		fLeftVal = MOTOR_OUT_MIN;

		if(fRightVal > MOTOR_OUT_MAX)
		fRightVal = MOTOR_OUT_MAX;
		if(fRightVal < MOTOR_OUT_MIN)
		fRightVal = MOTOR_OUT_MIN;

		SetMotorVoltage(fLeftVal, fRightVal);
}

//**************************************************************************
//  @brief      PWM输出设置函数
//  @return     void
//  函数结果：	电机PWM输出值
//  备注：参考清华源码
//**************************************************************************
void SetMotorVoltage(float fLeftVoltage, float fRightVoltage) {
	if(stop_flag==0)
		{
		if(fLeftVoltage >= 0)    
        {
            pwm_duty(PWM2_P26,0);
            pwm_duty(PWM2_P22,(uint32)fLeftVoltage);   
        }
        else
        {
            pwm_duty(PWM2_P22,0);
            pwm_duty(PWM2_P26,(uint32)(-fLeftVoltage)); 
        }

     if(fRightVoltage >= 0) 
        {
            pwm_duty(PWM1_P17,0);
            pwm_duty(PWM1_P10,(uint32)fRightVoltage);   
        }
        else
        {
            pwm_duty(PWM1_P10,0);
            pwm_duty(PWM1_P17,(uint32)(-fRightVoltage));   
        }
		}	
				if(stop_flag==1){
					
							pwm_duty(PWM2_P26,0);						//电机制动
						pwm_duty(PWM2_P22,0);
						pwm_duty(PWM1_P10,0);
						pwm_duty(PWM1_P17,0);
				
				pca_delay_ms(100);
				}
					
}

void OutPut_Vcan(void)
{
			uint8 var[14];
			
     var[0] =VOLTAGE_GRAVITY;			//加速度计z轴
		 var[1] ='\n';
     var[2] =VOLTAGE_GYRO;						//陀螺仪y轴
     var[3] = '\n';
     var[4] =left_AD_shuiping;
     var[5] ='\n';
		 var[6] =right_AD_shuiping;
		 var[7] = '\n';
     var[8] =left_AD_shuzhi;
     var[9] ='\n';
		 var[10] =right_AD_shuzhi;
		 var[11]= '\n';
		 var[12] =icm_gyro_x;          //陀螺仪x轴（车模转动角度）
		 var[13]= '\n';
	
		uart_putbuff(UART_2,&var[0],14);
    //data_conversion(var[0], var[1], var[2], var[3], *dat);
		//seekfree_wireless_send_buff(dat, 4);
}