#include "headfile.h"

int8 m,num,pe,pec; 
//float fe; 						//���ƫ��ֵ��3�ּ��㷽ʽ��������
//float fec;						//���ƫ���΢��
int16 speed_now;			//��������ȡ���ٶ�

float eFuzzy[2] = {0,0}; 
float ecFuzzy[2] = {0,0}; 

float speed_fe = 0;  //�����������fe��ɾ���ֵ����ֵ��speed_fe
float speed_fec = 0; //�����������fec��ɾ���ֵ����ֵ��speed_fec
float speed_Fuzzy[6] = {0,0,0,0,0,/*ĩβΪ0,������ѯģ������*/ 0};
float speed_forecast; //Ԥ�⽫Ҫ�ﵽ���ٶȣ�PWM��
float speed_forecast_error; //Ԥ�⽫Ҫ�ﵽ���ٶȵ�ƫ����٣�
//float speed_forecast_error_last;
//float speed_forecast_error_error;
int8 speed_pe,speed_pec;
float speed_forecast = 0;  //Ԥ�⳵��

//float speed_round=0;  //�뻷ǿ�Ʋ��ٲ���
/*��Ҫ���ڵĲ���*/ 
//float speed_power = 1,speed_pp=1;
float speed_eRule[5] = {5,8,15,18,22}; //������speed_fe���ķ�Χ                       3 9 13 18 25                          ���������ɲ�ͬ����Ԫ�ص�е������/ƫ���м䵼�ߵ�ֵ
                                   //
float speed_Rule_range[5] = {100,100,100,100,100}; //���Ԥ���ٶȣ�speed_forecast���ķ�Χ       58;63;68                             ���������ɲ�ͬ����Ԫ�ص�Ԥ���ٶ���/���ǹ涨Ԥ���ٶȶ���100����
float speed_error_Rule[5] = {96,95,94,88,50};  //Ԥ���ٶ�ƫ��ķ�Χ                   //0 12 18  ��0 10 16�� 30354050              ������....��
int speed_rule[6][6] =   //�ٶȹ����
{
  //ec 0 1 2 3 4  //e
      {4,4,4,3,2,5},//0    
      {4,3,3,2,1,5},//1   
      {4,3,2,1,0,5},//2    
      {3,2,1,1,0,5},//3         
      {2,1,0,0,0,5},//4
      {5,5,5,5,5,5} //
};
/****************  ���²���Ϊ�ٶȱջ����ã�����δ���������ظ����壬��ʱ���� ************/

float speed_fe_last = 0;

//float speed_fe = 0;

//float speed_fec = 0;

float control_P;  // ����� P ֵ �� D ֵ
float speed_Fuzzy_kp[6] = {0,0,0,0,0,/*ĩβΪ0,������ѯģ������*/ 0};
float speed_Fuzzy_kd[6] = {0,0,0,0,0,/*ĩβΪ0,������ѯģ������*/ 0};

/*******************  ��Ҫ���ڵĲ���  ******************************************/
float speed_eRule_err[5] = {-55,-35,0,35,55}; //�������ķ�Χ           //       20 50                   (���Ҳ����...������Ҫ��θ�����ֵ����)��ʵ��ֵ��Ԥ��ֵ�Ĳ

float speed_Rule_kp_range[5] = {-35,-25,0,25,35};  //�����Pֵ�ķ�Χ   2.5��Ϊ15��25��    15��25֮��Ҫ��            30 20      45 60     60 80     14��16   ���������ɲ�ͬ����Ԫ�ص�pֵ�𣿣�      
int speed_rule_kp[6][6]=  //pֵ�����         //35 20 20 35
{
  //ec 0 1 2 3 4  //e
      {4,4,4,3,2,5},//0  
      {4,3,3,2,1,5},//1   
      {4,3,2,1,0,5},//2    
      {3,2,1,1,0,5},//3         
      {2,1,0,0,0,5},//4
      {5,5,5,5,5,5} //
};

uint8 speed_error_power = 1;
float DDD = 0;    //�������Dֵ
float D_error=0;
/*******************************************************************************
 *  @brief      speed_fuzzy_mem_cal_forecast����
 *  @note       �����ȼ��㺯��
                ����������eFuzzy[]��ecfuzzy[]���ȼ�Ϊpe��pec
 *  @warning    18/3/14 ����ο�binary-star��  v4.0
 ******************************************************************************/
void speed_fuzzy_mem_cal_forecast(void)//�����ȼ���
{
  
    if(fe < 0) speed_fe = -fe;  //�����������fe��ɾ���ֵ����ֵspeed_fe
    else speed_fe = fe;
    if(fec < 0) speed_fec = -fec; //�����������fe��ɾ���ֵ����ֵspeed_fec
    else speed_fec = fec;
    
    
  /*-----���������������-----*/
    if(speed_fe < speed_eRule[0])     //����ȷ��������		        // |x_x_x_x_x_     �ĸĸ�
    {
      eFuzzy[0] =1.0; 
      speed_pe= 0;          //?
    }
    else if(speed_fe < speed_eRule[1])	        // _x|x_x_x_x_
    {       
      eFuzzy[0] = (speed_fe-speed_eRule[0])/(speed_eRule[1]-speed_eRule[0]);
      speed_pe = 0;
    }
    else if(speed_fe < speed_eRule[2])	        // _x_x|x_x_x_
    {
      eFuzzy[0] = (speed_fe-speed_eRule[1])/(speed_eRule[2]-speed_eRule[1]);
      speed_pe =1;
    }
    else if(speed_fe < speed_eRule[3])	        // _x_x_x|x_x_
    {
      eFuzzy[0] = (speed_fe-speed_eRule[2])/(speed_eRule[3]-speed_eRule[2]);
      speed_pe =2;
    }
    else if(speed_fe < speed_eRule[4])		        // _x_x_x_x|x_
    {   
      eFuzzy[0] = (speed_fe-speed_eRule[3])/(speed_eRule[4]-speed_eRule[3]);
      speed_pe=3;
    }	
    else						        // _x_x_x_x_x|
    {
      eFuzzy[0] =1.0;
      speed_pe=4;
    }
    eFuzzy[1] = 1.0 - eFuzzy[0];                    //eFuzzy[0]+eFuzzy[1]=1;

/**/speed_pec = 2;                          //fecһ����20��
/**/ecFuzzy[0] = 1;
/**/ecFuzzy[1] = 0;
      
}
    

/*******************************************************************************
 *  @brief      speed_fuzzy_query_forecast����
 *  @note       ��ѯģ��������������ĸ�ռ��
                
 *  @warning    18/3/14 ����ο�binary-star��  v4.0
 ******************************************************************************/
void speed_fuzzy_query_forecast(void)//��ѯģ�������
{
    for(m = 0;m <= rank; m++) //��������Ա��ۼ�
    {
        speed_Fuzzy[m] = 0;
    }
      /*��ѯkpģ�������*/  
    num = speed_rule[speed_pe][speed_pec];
    speed_Fuzzy[num] += eFuzzy[0];
    
    num = speed_rule[speed_pe+1][speed_pec];
    speed_Fuzzy[num] += eFuzzy[1];
    
}

/*******************************************************************************
 *  @brief      speed_fuzzy_solve_forecast����
 *  @note       ���ķ���ģ��
                
 *  @warning    18/3/14 ����ο�binary-star�� v4.0
 ******************************************************************************/
void speed_fuzzy_solve_forecast(void)//��ģ���õ�pdֵ
{
    speed_forecast = 0; //���P��Dֵ�Ա��ۼ�
    speed_forecast_error = 0;
    /*������ķ���ģ��*/
    for(m = 0;m < rank; m++)
    {
      speed_forecast += (speed_Fuzzy[m] * speed_Rule_range[m]);
      speed_forecast_error += (speed_Fuzzy[m] * speed_error_Rule[m]);
    }
    
   // if( speed_forecast > 55) speed_forecast = 55;
   // if( speed_forecast_error > 30) speed_forecast_error = 30;
   // speed_forecast = speed_power * speed_forecast*speed_pp;                           //  ��������                                                                           
   // speed_forecast_error = speed_power * speed_forecast_error*speed_pp;
    
    
   // speed_forecast_error_error=speed_forecast_error-speed_forecast_error_last;   //���ٵķ�Χ
   // speed_forecast_error_last=speed_forecast_error;
}

/*******************************************************************************
 *  @brief      speedcontrol_forecast����
 *  @note       �ٶ�Ԥ�⺯�������Ԥ��Ҫ�ﵽ���ٶȵ�ƫ�����ƫ��仯��
                
 *  @warning    18/3/14 v4.0  δ���ԣ�����
 ******************************************************************************/
void speedcontrol_forecast(void)
{

  
    
    speed_fe_last = speed_fe;
    speed_fe = speed_now - speed_forecast;          //ʵ����Ԥ���ƫ����������������������������Ĳ�ֵ��
    speed_fec = speed_fe - speed_fe_last;           //ʵ����Ԥ���ƫ���ƫ���΢��
    
   
    
    
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////


/*******************************************************************************
 *  @brief      speed_fuzzy_mem_cal����
 *  @note       �����ȼ��㺯��
                ����������eFuzzy[]��ecfuzzy[]���ȼ�Ϊpe��pec
 *  @warning    18/3/14 ����ο�binary-star��  v4.0 δ���ԣ�����
 ******************************************************************************/
void speed_fuzzy_mem_cal_fe(void)//�����ȼ���
{
    //////////////////////////����//////////////////////////////////
  /*-----���������������-----*/
    if(speed_fe < speed_eRule_err[0])     //����ȷ��������		        // |x_x_x_x_x_
    {
      eFuzzy[0] =1.0; 
      pe= 0;          //?
    }
    else if(speed_fe < speed_eRule_err[1])	        // _x|x_x_x_x_
    {       
      eFuzzy[0] = (speed_fe-speed_eRule_err[0]) / (speed_eRule_err[1]-speed_eRule_err[0]);
      pe = 0;
    }
    else if(speed_fe < speed_eRule_err[2])	        // _x_x|x_x_x_
    {
      eFuzzy[0] = (speed_fe-speed_eRule_err[1]) / (speed_eRule_err[2]-speed_eRule_err[1]);
      pe =1;
    }
    else if(speed_fe < speed_eRule_err[3])	        // _x_x_x|x_x_
    {
      eFuzzy[0] = (speed_fe-speed_eRule_err[2]) / (speed_eRule_err[3]-speed_eRule_err[2]);
      pe =2;
    }
    else if(speed_fe < speed_eRule_err[4])		        // _x_x_x_x|x_
    {   
      eFuzzy[0] = (speed_fe-speed_eRule_err[3]) / (speed_eRule_err[4]-speed_eRule_err[3]);
      pe=3;
    }
   
    else						        // _x_x_x_x_x|
    {
      eFuzzy[0] =1.0;
      pe=4;
    }
    eFuzzy[1] = 1.0 - eFuzzy[0];                    //eFuzzy[0]+eFuzzy[1]=1;
 
/**/pec = 2;
/**/ecFuzzy[0] = 1;
/**/ecFuzzy[1] = 0;
    
   
}

/*******************************************************************************
 *  @brief      speed_fuzzy_query����
 *  @note       ��ѯģ��������������ĸ�ռ��
                
 *  @warning    18/3/14 ����ο�binary-star��  v4.0 δ���ԣ�����
 ******************************************************************************/
void speed_fuzzy_query_fe(void)//��ѯģ�������
{
    for(m = 0;m <= rank; m++) //��������Ա��ۼ�
    {
        speed_Fuzzy_kp[m] = 0;
        speed_Fuzzy_kd[m] = 0;
    }
      /*��ѯkpģ�������*/  
    num = speed_rule_kp[pe][pec];
    speed_Fuzzy_kp[num] += eFuzzy[0];	
    
    num = speed_rule_kp[pe+1][pec];
    speed_Fuzzy_kp[num] += eFuzzy[1];
    
}

/*******************************************************************************
 *  @brief      speed_fuzzy_solve����
 *  @note       ���ķ���ģ��
                
 *  @warning    18/3/14 ����ο�binary-star�� v4.0 δ���ԣ�����
 ******************************************************************************/
void speed_fuzzy_solve_fe(void)//��ģ���õ�pdֵ
{
    control_P = 0; //���P��Dֵ�Ա��ۼ�
    //speed_D = 0;
    /*������ķ���ģ��*/
    for(m = 0;m < rank; m++)
    {
      control_P += speed_Fuzzy_kp[m] * speed_Rule_kp_range[m];
    //  speed_D += speed_Fuzzy_kd[m] * speed_Rule_kd[m];   
    }
}

