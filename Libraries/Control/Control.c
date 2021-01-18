#include "Control.h"

float VOLTAGE_GRAVITY,VOLTAGE_GYRO;		//���ٶȼ�Z���ȡ���ݣ������ǽ��ٶȻ�ȡ����
float g_fGravityAngle,g_fGyroscopeAngleSpeed;		//���ٶȼ�Z����������,�����ǽ��ٶ���������
unsigned int AD_average;
unsigned int AD_data[10];		//������������ݵ�
float g_fCarAngle;		
float g_fGyroscopeAngleIntegral=0;		//���ֺ�ĽǶ�
float g_fAngleControlOut, g_fSpeedControlOut,g_fDirectionControlOut;		//ֱ�����������,�ٶȿ��������,������������
float g_fCarSpeed;		
int g_nLeftMotorPulseSigma,g_nRightMotorPulseSigma,DIR_CONTROL_D_VALUE;		//����������������ұ�����������,��ģת������������
float g_fSpeedControlIntegral;		
float g_fSpeedControlOutOld, g_fSpeedControlOutNew;
float g_fLeftVoltageSigma,g_fRightVoltageSigma,turn;		//��ż�������壬��ż��������,ת�� 
float g_fDirectionControlOutOld , g_fDirectionControlOutNew;
float g_fLeftMotorOut , g_fRightMotorOut;
float mid_AD_shuiping,left_AD_shuzhi,right_AD_shuzhi;   //���У�����ֱ���ֵ
float left_AD_shuiping,right_AD_shuiping;                //����ˮƽ���ֵ
int  diangan_flag=0,turn_flag=0;
float fe,fec;																							//���ƫ��ֵ
float SPEED_CONTROL_OUT_MAX,SPEED_CONTROL_OUT_MIN;				//���ֱ���

float OutData[4] = { 0 };

extern int GYRO_set;
extern int MMA_set;
extern int speed_set;
extern float ANGLE_P,ANGLE_D;
extern float SPEED_P,SPEED_I;
extern float DIR_P,DIR_D;



//**************************************************************************	  
//  @brief      AD�ɼ����ٶȼ�Z�ᡢ������
//  @return     void
//  ���������  �ɼ�����Z�ᡢ�����ǵ�����
//**************************************************************************
void Rd_Ad_Value(void){

	int j=0;

    VOLTAGE_GRAVITY = icm_acc_z;   //Z ���ٶȼƲ⣨�Ƕȣ�	 
       
    for(j=0;j<10;j++)
    {
      VOLTAGE_GYRO = icm_gyro_y;    // gyro�����ǣ����ٶȣ�	 �����������Ҫ�Ķ���
      AD_data[j]= VOLTAGE_GYRO;
    }
      AD_average=(AD_data[1]+AD_data[2]+AD_data[3]+AD_data[4]+AD_data[5]+AD_data[6]+AD_data[7]+AD_data[8])/8;
      VOLTAGE_GYRO =  AD_average;

}

//**************************************************************************
//  @brief      �Ƕȼ��㺯��
//  @return     void
//  ������GRAVITY_OFFSET�����ٶȼ�Z����ƫֵ
//		  GRAVITY_ANGLE_RATIO����һ������ϵ��											   
//		  GRAVITY_ADJUST_TIME_CONSTANT  ʱ��У��ϵ��      ��ʱ��ʱ�� ��λs
//  ���������ֱ������µĽǶ�
//  ��ע���ο��廪Դ��
//**************************************************************************
void AngleCalculate(void) 
{
	   
		float fDeltaValue;
	
		g_fGravityAngle = (VOLTAGE_GRAVITY - MMA_set) *GRAVITY_ANGLE_RATIO; 		//���ٶȼ� Z

		g_fGyroscopeAngleSpeed = (VOLTAGE_GYRO - GYRO_set) *GYROSCOPE_ANGLE_RATIO;	 //�����ǽ��ٶ�

		g_fCarAngle = g_fGyroscopeAngleIntegral;		//�����ںϽǶ�		
		fDeltaValue = (g_fGravityAngle - g_fCarAngle) /GRAVITY_ADJUST_TIME_CONSTANT;	//ʱ��ϵ������

		g_fGyroscopeAngleIntegral += (g_fGyroscopeAngleSpeed + fDeltaValue) *GYROSCOPE_ANGLE_SIGMA_FREQUENCY;   	//�ںϽǶ�
}

//**************************************************************************
//  @brief      ֱ�����ƺ���
//  @return     void
//  ���������	ֱ������µ������
//  ��ע���ο��廪Դ��
//**************************************************************************
void AngleControl(void) {

		float fValue;

		fValue = (CAR_ANGLE_SET - g_fCarAngle) * ANGLE_P +
		  		 (CAR_ANGLE_SPEED_SET - g_fGyroscopeAngleSpeed) *ANGLE_D;

/*
	if(fValue > ANGLE_CONTROL_OUT_MAX)		//����޷�
		fValue = ANGLE_CONTROL_OUT_MAX;

		else if(fValue < ANGLE_CONTROL_OUT_MIN)									  
		fValue = ANGLE_CONTROL_OUT_MIN;	  
*/

		g_fAngleControlOut = fValue;
}

//**************************************************************************
//  @brief      ��ȡС���ٶ�
//  ����˵��	  ��λʱ���ڲɼ��������������ɴ���С���ٶ�
//  @return     void
//  ���������	�����ٶ�
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
//  @brief      �ٶȿ���
//  @return     void
//  ����		CAR_SPEED_SETfDelta  �������ٶ�
//  ���������	�ٶȼ���
//  ��ע���ο��廪Դ��
//**************************************************************************
void SpeedControl(void) {

		float fDelta;															
		float fP, fI;													 

		g_fCarSpeed = (g_nLeftMotorPulseSigma + g_nRightMotorPulseSigma) / 2;

		g_nLeftMotorPulseSigma = g_nRightMotorPulseSigma = 0;		//��������
		g_fCarSpeed *= CAR_SPEED_CONSTANT;		//������ƽ��ֵ���Ա���ֵ
		speed_now = g_fCarSpeed*20.1;					//ת��Ϊ�ٶ�cm/s
				
		fDelta = speed_set - g_fCarSpeed;
		fP = fDelta * SPEED_P;
		fI = fDelta * SPEED_I;

		g_fSpeedControlIntegral += fI;		//����	�����б��ʹ�����ֹ���ֱ���
	
		if(g_fSpeedControlIntegral > SPEED_CONTROL_OUT_MAX)	   //��ֹ���ֱ���
        g_fSpeedControlIntegral = SPEED_CONTROL_OUT_MAX;
   if(g_fSpeedControlIntegral < SPEED_CONTROL_OUT_MIN)  	
         g_fSpeedControlIntegral = SPEED_CONTROL_OUT_MIN;

		g_fSpeedControlOutOld = g_fSpeedControlOutNew;
		g_fSpeedControlOutNew = fP + g_fSpeedControlIntegral;		//�����ٶȼ���ֵ
}

//**************************************************************************
//  @brief      �ٶ�ƽ���������
//  @return     void
//  ����		SPEED_CONTROL_COUNT	����ֵ���ƽ����20������
//  ���������	�ٶ����ֵ
//  ��ע���ο��廪Դ��
//**************************************************************************
void SpeedControlOutput(void) {

		float fValue;

		fValue = g_fSpeedControlOutNew - g_fSpeedControlOutOld;		//�����ٶȼ����ֵ
		g_fSpeedControlOut = fValue *  1 / SPEED_CONTROL_COUNT + g_fSpeedControlOutOld;
}

//**************************************************************************
//  @brief      AD�ɼ������Ȧ
//  @return     void
//  ���������	�ٶ����ֵ
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
   	 
////////////////////////�м�ˮƽ���///////////////////////�ɼ�8��
		for(i=0;i<DIRECTION_COUNT;i++){
			vr[i]=adc_once(ADC_P12,ADC_10BIT);
			 for(j=0;j<i;j++)
      {
        if(vr[i-j]>vr[i-j-1])        //ð�ݷ��Ӵ�С�Բ���ֵ����
        {
          change   = vr[i-j-1];
          vr[i-j-1]= vr[i-j];
          vr[i-j]  = change;
        }
      }
			for(i=1;i<DIRECTION_COUNT-1;i++)    //�ź�����޳�1����Сֵ��1�������ֵ
      vracc+= vr[i];                //�������ȡƽ����
      mid_AD_shuiping=vracc/(DIRECTION_COUNT-2);
      vracc=0;
			
		}
	////////////////////////��ˮƽ���///////////////////////�ɼ�8��
		for(i=0;i<DIRECTION_COUNT;i++){
			vr[i]=adc_once(AMP1,ADC_10BIT);
			 for(j=0;j<i;j++)
      {
        if(vr[i-j]>vr[i-j-1])        //ð�ݷ��Ӵ�С�Բ���ֵ����
        {
          change   = vr[i-j-1];
          vr[i-j-1]= vr[i-j];
          vr[i-j]  = change;
        }
      }
			for(i=1;i<DIRECTION_COUNT-1;i++)    //�ź�����޳�1����Сֵ��1�������ֵ
      vracc+= vr[i];                //�������ȡƽ����
      left_AD_shuiping=vracc/(DIRECTION_COUNT-2);
      vracc=0;
			
		}
	////////////////////////��ˮƽ���///////////////////////
    for(i=0;i<DIRECTION_COUNT;i++){
			vr[i]=adc_once(AMP2,ADC_10BIT);
			 for(j=0;j<i;j++)
      {
        if(vr[i-j]>vr[i-j-1])        //ð�ݷ��Ӵ�С�Բ���ֵ����
        {
          change   = vr[i-j-1];
          vr[i-j-1]= vr[i-j];
          vr[i-j]  = change;
        }
      }
			for(i=1;i<DIRECTION_COUNT-1;i++)    //�ź�����޳�1����Сֵ��1�������ֵ
      vracc+= vr[i];                //�������ȡƽ����
      right_AD_shuiping=vracc/(DIRECTION_COUNT-2);
      vracc=0;
			
		}
	////////////////////////����ֱ���///////////////////////
		for(i=0;i<DIRECTION_COUNT;i++){
			vr[i]=adc_once(ADC_P02,ADC_10BIT);
			 for(j=0;j<i;j++)
      {
        if(vr[i-j]>vr[i-j-1])        //ð�ݷ��Ӵ�С�Բ���ֵ����
        {
          change   = vr[i-j-1];
          vr[i-j-1]= vr[i-j];
          vr[i-j]  = change;
        }
      }
			for(i=1;i<DIRECTION_COUNT-1;i++)    //�ź�����޳�1����Сֵ��1�������ֵ
      vracc+= vr[i];                //�������ȡƽ����
      left_AD_shuzhi=vracc/(DIRECTION_COUNT-2);
      vracc=0;
			
		}
		////////////////////////����ֱ���///////////////////////
		for(i=0;i<DIRECTION_COUNT;i++){
			vr[i]=adc_once(ADC_P03,ADC_10BIT);
			 for(j=0;j<i;j++)
      {
        if(vr[i-j]>vr[i-j-1])        //ð�ݷ��Ӵ�С�Բ���ֵ����
        {
          change   = vr[i-j-1];
          vr[i-j-1]= vr[i-j];
          vr[i-j]  = change;
        }
      }
			for(i=1;i<DIRECTION_COUNT-1;i++)    //�ź�����޳�1����Сֵ��1�������ֵ
      vracc+= vr[i];                //�������ȡƽ����
      right_AD_shuzhi=vracc/(DIRECTION_COUNT-2);
      vracc=0;
	////////////////////////////////////////////////////		
		}
    for(i=0;i<DIRECTION_COUNT;i++)		//�ɼ�8��
    { 
			//left_AD_Date+=adc_once(AMP1,ADC_10BIT);
      //right_AD_Date+=adc_once(AMP2,ADC_10BIT);
      temp+= icm_gyro_x;						//��Ķ�  �������Ƿ���
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
/////////////////////////////ת�򻷿���//////////////////////////////////////////////////
			if(turn_flag==0){
					DIR_CONTROL_D_VALUE +=(int) (temp/DIRECTION_COUNT - DIRECTION_OFFSET); 
			}
			else if (turn_flag==1){
					DIR_CONTROL_D_VALUE +=(int) (temp/DIRECTION_COUNT - DIRECTION_OFFSET+corner_ruku); 								//���ת��
						turn_flag=0;
			}
			else if (turn_flag==2){
					DIR_CONTROL_D_VALUE +=(int) (temp/DIRECTION_COUNT - DIRECTION_OFFSET+corner_chuku); 							//����ת��
						turn_flag=0;
			}
}
//**************************************************************************
//  @brief      ������㺯��
//  @return     void
//  ����		DIRECTION_CONTROL_COUNT ���ۼӴ��� 2
//				DIR_CONTROL_P��		Pֵ
//				DIR_CONTROL_D��		������Dֵ
//				DIRECTION_OFFSET��	��ģת������ģת�����������ƫ�ƣ�
//				DIR_CONTROL_D_VALUE��	������ת������
//  ���������	�ٶ����ֵ
//  ��ע���ο��廪Դ��
//**************************************************************************
void DirectionControl(void) {

		float fLeftRightAdd, fLeftRightSub, fValue=0;
		int nLeft, nRight ,turn ;	
		float mid=mid_AD_shuiping-MID_AD_SHUIPING_OFFEST;

		nLeft = (int)(g_fLeftVoltageSigma /= DIRECTION_CONTROL_COUNT);			//ƽ����
		nRight = (int)(g_fRightVoltageSigma /= DIRECTION_CONTROL_COUNT);
		turn = (int)(DIR_CONTROL_D_VALUE/= DIRECTION_CONTROL_COUNT);

		g_fLeftVoltageSigma = 0;
		g_fRightVoltageSigma = 0;
		DIR_CONTROL_D_VALUE = 0;

		fLeftRightAdd = nLeft + nRight;
		fLeftRightSub = nLeft - nRight;

		fec=fe;
		fe=150*fLeftRightSub/(fLeftRightAdd+mid);									//����С�׵ĵ�ж�λ�����͹�һ˫����ж�λ������150��ֵ�ɵ���
		
		g_fDirectionControlOutOld = g_fDirectionControlOutNew;		//������ڵ��ϵ����ϸ����ڵ���

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
//  @brief      ����ƽ�����
//  @return     void
//  ���������	�������
//  ��ע���ο��廪Դ��
//**************************************************************************
void DirectionControlOutput(void) {

		float fValue;

		fValue = g_fDirectionControlOutNew - g_fDirectionControlOutOld;
		g_fDirectionControlOut = fValue *  1 / DIRECTION_CONTROL_COUNT + g_fDirectionControlOutOld;
}

//**************************************************************************
//  @brief      ����������
//  @return     void
//  ���������	������ֵ
//  ��ע���ο��廪Դ��
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
//  @brief      ���PWM���㺯��
//  @return     void
//  ����		MOTOR_OUT_DEAD_VAL��		����������ѹ
//  ���������	���PWM���ֵ
//  ��ע���ο��廪Դ��
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

		if(fLeftVal > MOTOR_OUT_MAX) 		//�޷�
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
//  @brief      PWM������ú���
//  @return     void
//  ���������	���PWM���ֵ
//  ��ע���ο��廪Դ��
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
					
							pwm_duty(PWM2_P26,0);						//����ƶ�
						pwm_duty(PWM2_P22,0);
						pwm_duty(PWM1_P10,0);
						pwm_duty(PWM1_P17,0);
				
				pca_delay_ms(100);
				}
					
}

void OutPut_Vcan(void)
{
			uint8 var[14];
			
     var[0] =VOLTAGE_GRAVITY;			//���ٶȼ�z��
		 var[1] ='\n';
     var[2] =VOLTAGE_GYRO;						//������y��
     var[3] = '\n';
     var[4] =left_AD_shuiping;
     var[5] ='\n';
		 var[6] =right_AD_shuiping;
		 var[7] = '\n';
     var[8] =left_AD_shuzhi;
     var[9] ='\n';
		 var[10] =right_AD_shuzhi;
		 var[11]= '\n';
		 var[12] =icm_gyro_x;          //������x�ᣨ��ģת���Ƕȣ�
		 var[13]= '\n';
	
		uart_putbuff(UART_2,&var[0],14);
    //data_conversion(var[0], var[1], var[2], var[3], *dat);
		//seekfree_wireless_send_buff(dat, 4);
}