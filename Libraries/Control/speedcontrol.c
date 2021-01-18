#include "headfile.h"

int8 m,num,pe,pec; 
//float fe; 						//电磁偏差值，3种计算方式，都试试
//float fec;						//电磁偏差的微分
int16 speed_now;			//编码器读取的速度

float eFuzzy[2] = {0,0}; 
float ecFuzzy[2] = {0,0}; 

float speed_fe = 0;  //将输入回来的fe变成绝对值，赋值进speed_fe
float speed_fec = 0; //将输入回来的fec变成绝对值，赋值进speed_fec
float speed_Fuzzy[6] = {0,0,0,0,0,/*末尾为0,用来查询模糊表步骤*/ 0};
float speed_forecast; //预测将要达到的速度（PWM）
float speed_forecast_error; //预测将要达到的速度的偏差（差速）
//float speed_forecast_error_last;
//float speed_forecast_error_error;
int8 speed_pe,speed_pec;
float speed_forecast = 0;  //预测车速

//float speed_round=0;  //入环强制差速参数
/*需要调节的参数*/ 
//float speed_power = 1,speed_pp=1;
float speed_eRule[5] = {5,8,15,18,22}; //输入误差（speed_fe）的范围                       3 9 13 18 25                          （可以理解成不同赛道元素电感的误差吗）/偏离中间导线的值
                                   //
float speed_Rule_range[5] = {100,100,100,100,100}; //输出预测速度（speed_forecast）的范围       58;63;68                             （可以理解成不同赛道元素的预测速度吗/还是规定预测速度都是100？）
float speed_error_Rule[5] = {96,95,94,88,50};  //预测速度偏差的范围                   //0 12 18  改0 10 16； 30354050              （不懂....）
int speed_rule[6][6] =   //速度规则表
{
  //ec 0 1 2 3 4  //e
      {4,4,4,3,2,5},//0    
      {4,3,3,2,1,5},//1   
      {4,3,2,1,0,5},//2    
      {3,2,1,1,0,5},//3         
      {2,1,0,0,0,5},//4
      {5,5,5,5,5,5} //
};
/****************  以下参数为速度闭环所用，函数未测试且有重复定义，暂时不理 ************/

float speed_fe_last = 0;

//float speed_fe = 0;

//float speed_fec = 0;

float control_P;  // 输出的 P 值 和 D 值
float speed_Fuzzy_kp[6] = {0,0,0,0,0,/*末尾为0,用来查询模糊表步骤*/ 0};
float speed_Fuzzy_kd[6] = {0,0,0,0,0,/*末尾为0,用来查询模糊表步骤*/ 0};

/*******************  需要调节的参数  ******************************************/
float speed_eRule_err[5] = {-55,-35,0,35,55}; //输入误差的范围           //       20 50                   (这个也不懂...具体是要如何给定数值。。)（实测值与预测值的差）

float speed_Rule_kp_range[5] = {-35,-25,0,25,35};  //输出的P值的范围   2.5改为15，25；    15到25之间要大            30 20      45 60     60 80     14；16   （可以理解成不同赛道元素的p值吗？）      
int speed_rule_kp[6][6]=  //p值规则表         //35 20 20 35
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
float DDD = 0;    //这里加了D值
float D_error=0;
/*******************************************************************************
 *  @brief      speed_fuzzy_mem_cal_forecast函数
 *  @note       隶属度计算函数
                输出结果放在eFuzzy[]和ecfuzzy[]，等级为pe、pec
 *  @warning    18/3/14 代码参考binary-star队  v4.0
 ******************************************************************************/
void speed_fuzzy_mem_cal_forecast(void)//隶属度计算
{
  
    if(fe < 0) speed_fe = -fe;  //将输入回来的fe变成绝对值，赋值speed_fe
    else speed_fe = fe;
    if(fec < 0) speed_fec = -fec; //将输入回来的fe变成绝对值，赋值speed_fec
    else speed_fec = fec;
    
    
  /*-----误差隶属函数描述-----*/
    if(speed_fe < speed_eRule[0])     //用来确定隶属度		        // |x_x_x_x_x_     改改改
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

/**/speed_pec = 2;                          //fec一定是20？
/**/ecFuzzy[0] = 1;
/**/ecFuzzy[1] = 0;
      
}
    

/*******************************************************************************
 *  @brief      speed_fuzzy_query_forecast函数
 *  @note       查询模糊规则表，算出输出的各占比
                
 *  @warning    18/3/14 代码参考binary-star队  v4.0
 ******************************************************************************/
void speed_fuzzy_query_forecast(void)//查询模糊规则表
{
    for(m = 0;m <= rank; m++) //清空数组以便累加
    {
        speed_Fuzzy[m] = 0;
    }
      /*查询kp模糊规则表*/  
    num = speed_rule[speed_pe][speed_pec];
    speed_Fuzzy[num] += eFuzzy[0];
    
    num = speed_rule[speed_pe+1][speed_pec];
    speed_Fuzzy[num] += eFuzzy[1];
    
}

/*******************************************************************************
 *  @brief      speed_fuzzy_solve_forecast函数
 *  @note       重心法解模糊
                
 *  @warning    18/3/14 代码参考binary-star队 v4.0
 ******************************************************************************/
void speed_fuzzy_solve_forecast(void)//解模糊得到pd值
{
    speed_forecast = 0; //清空P和D值以便累加
    speed_forecast_error = 0;
    /*面积中心法解模糊*/
    for(m = 0;m < rank; m++)
    {
      speed_forecast += (speed_Fuzzy[m] * speed_Rule_range[m]);
      speed_forecast_error += (speed_Fuzzy[m] * speed_error_Rule[m]);
    }
    
   // if( speed_forecast > 55) speed_forecast = 55;
   // if( speed_forecast_error > 30) speed_forecast_error = 30;
   // speed_forecast = speed_power * speed_forecast*speed_pp;                           //  （？？）                                                                           
   // speed_forecast_error = speed_power * speed_forecast_error*speed_pp;
    
    
   // speed_forecast_error_error=speed_forecast_error-speed_forecast_error_last;   //差速的范围
   // speed_forecast_error_last=speed_forecast_error;
}

/*******************************************************************************
 *  @brief      speedcontrol_forecast函数
 *  @note       速度预测函数，算出预测要达到的速度的偏差，及其偏差变化率
                
 *  @warning    18/3/14 v4.0  未测试，待定
 ******************************************************************************/
void speedcontrol_forecast(void)
{

  
    
    speed_fe_last = speed_fe;
    speed_fe = speed_now - speed_forecast;          //实际与预测的偏差，！！！！！！！！！！！真正的差值；
    speed_fec = speed_fe - speed_fe_last;           //实际与预测的偏差的偏差；及微分
    
   
    
    
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////


/*******************************************************************************
 *  @brief      speed_fuzzy_mem_cal函数
 *  @note       隶属度计算函数
                输出结果放在eFuzzy[]和ecfuzzy[]，等级为pe、pec
 *  @warning    18/3/14 代码参考binary-star队  v4.0 未测试，待定
 ******************************************************************************/
void speed_fuzzy_mem_cal_fe(void)//隶属度计算
{
    //////////////////////////左轮//////////////////////////////////
  /*-----误差隶属函数描述-----*/
    if(speed_fe < speed_eRule_err[0])     //用来确定隶属度		        // |x_x_x_x_x_
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
 *  @brief      speed_fuzzy_query函数
 *  @note       查询模糊规则表，算出输出的各占比
                
 *  @warning    18/3/14 代码参考binary-star队  v4.0 未测试，待定
 ******************************************************************************/
void speed_fuzzy_query_fe(void)//查询模糊规则表
{
    for(m = 0;m <= rank; m++) //清空数组以便累加
    {
        speed_Fuzzy_kp[m] = 0;
        speed_Fuzzy_kd[m] = 0;
    }
      /*查询kp模糊规则表*/  
    num = speed_rule_kp[pe][pec];
    speed_Fuzzy_kp[num] += eFuzzy[0];	
    
    num = speed_rule_kp[pe+1][pec];
    speed_Fuzzy_kp[num] += eFuzzy[1];
    
}

/*******************************************************************************
 *  @brief      speed_fuzzy_solve函数
 *  @note       重心法解模糊
                
 *  @warning    18/3/14 代码参考binary-star队 v4.0 未测试，待定
 ******************************************************************************/
void speed_fuzzy_solve_fe(void)//解模糊得到pd值
{
    control_P = 0; //清空P和D值以便累加
    //speed_D = 0;
    /*面积中心法解模糊*/
    for(m = 0;m < rank; m++)
    {
      control_P += speed_Fuzzy_kp[m] * speed_Rule_kp_range[m];
    //  speed_D += speed_Fuzzy_kd[m] * speed_Rule_kd[m];   
    }
}

