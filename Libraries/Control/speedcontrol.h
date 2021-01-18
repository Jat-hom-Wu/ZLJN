#ifndef  __Speedcontrol_H__
#define  __Speedcontrol_H__

//extern int8 pe,pec; 
//extern int8 m,num;
//extern float speed_fe; 
//extern float speed_fec; 

//extern float eFuzzy[2]; 
//extern float ecFuzzy[2]; 

extern int16 speed_now ; 
extern float control_P,speed_D,speed_fe;
//extern int16 speedctrl_left,speedctrl_right; 
//extern int16 speedctrl_error_left,speedctrl_error_right; 
extern float speed_fe_last,speed_fec;

//extern float speed_Fuzzy[6];
extern float speed_forecast; 
extern float speed_forecast_error;
//extern int8 speed_pe,speed_pec;
//extern float speed_forecast_left;  //左轮预测车速
//extern float speed_forecast_right;  //右轮预测车速
extern float speed_round;  //入环强制差速参数
/**************************  声明函数体  **************************************/
void speed_fuzzy_mem_cal_forecast(void);
void speed_fuzzy_query_forecast(void);
void speed_fuzzy_solve_forecast(void);
void speedcontrol_forecast(void);

void speed_fuzzy_mem_cal_fe(void);
void speed_fuzzy_query_fe(void);
void speed_fuzzy_solve_fe(void);


/****************************  宏定义  ****************************************/
#define rank (5)  //分级的精确度

#endif