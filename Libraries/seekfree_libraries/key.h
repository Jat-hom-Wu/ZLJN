#ifndef  __KEY__H
#define  __KEY__H

#include "headfile.h"

#define PTXn_e		unsigned char


//按键端口的枚举
typedef enum
{
    KEY_1,  //++
    KEY_2,  //--

    KEY_3,  //++
    KEY_4,  //--
	
		KEY_5,	//车速为0

    KEY_MAX,
} KEY_e;


//key状态枚举定义
typedef enum
{
    KEY_DOWN  =   0,         //按键按下时对应电平
    KEY_UP    =   1,         //按键弹起时对应电平

    KEY_HOLD,               //长按按键(用于定时按键扫描)

} KEY_STATUS_e;


//按键消息结构体
typedef struct
{
    KEY_e           key;        //按键编号
    KEY_STATUS_e    status;     //按键状态
} KEY_MSG_t;


extern void            key_init(KEY_e key);            // KEY初始化函数(key 小于 KEY_MAX 时初始化 对应端口，否则初始化全部端口)
extern KEY_STATUS_e    key_get(KEY_e key);             //检测key状态（不带延时消抖）
extern KEY_STATUS_e    key_check(KEY_e key);           //检测key状态（带延时消抖）



#endif		  