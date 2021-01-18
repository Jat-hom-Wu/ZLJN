#ifndef __CCD_JUDGE_H
#define __CCD_JUDGE_H

#include "headfile.h"


/*****º¯ÊıÉùÃ÷*******/
void zebra_judge();
void chuku();
void distance();
void stop_car();


static int star_flag;
extern int stop_flag;
extern int ruku_distance_flag,chuku_distance_flag,left_huandao_distance_flag,right_huandao_distance_flag;
extern float left_huandao_distance,right_huandao_distance;


#endif