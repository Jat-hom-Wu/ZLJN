#include "key.h"


//**************************************************
// 定义 KEY 编号对应的管脚
//**************************************************
PTXn_e KEY_PTxn[KEY_MAX] = {P3_2, P3_3,P5_0,P5_1,P4_0};	  //需要根据pcb添加或删减地址


//******************************************************************
//   @brief      初始化key端口(key 小于 KEY_MAX 时初始化 对应端口，否则初始化全部端口)
//   @param      KEY_e    KEY编号
//   @since      v5.0
//   Sample usage:       key_init (KEY_U);    //初始化 KEY_U
//****************************************************************** 
void key_init(KEY_e key)
{
    if(key < KEY_MAX)
    {
       gpio_mode(KEY_PTxn[key],GPO_PP);        //初始化按键，选择推挽式
    }
    else
    {
        key = KEY_MAX;

        //初始化全部 按键
        while(key--)
        {
            gpio_mode(KEY_PTxn[key],GPO_PP);
        }

    }
}

//*******************************************
//  @brief      获取key状态（不带延时消抖）
//  @param      KEY_e           KEY编号
//  @return     KEY_STATUS_e    KEY状态（KEY_DOWN、KEY_DOWN）
//  @since      v5.0
//  Sample usage:
/*                    if(key_get(KEY_U) ==  KEY_DOWN)
                    {
                        printf("\n按键按下")
                    }
 */
//*******************************************
KEY_STATUS_e key_get(KEY_e key)
{
    if(KEY_PTxn[key] == KEY_DOWN)
    {
        return KEY_DOWN;
    }
    return KEY_UP;
}



//*******************************************
//  @brief      获取key状态（带延时消抖）
//  @param      KEY_e           KEY编号
//  @return     KEY_STATUS_e    KEY状态（KEY_DOWN、KEY_DOWN）
//  @since      v5.0
//  Sample usage:
/*                    if(key_get(KEY_U) ==  KEY_DOWN)
                    {
                        printf("\n按键按下")
                    }
 */
//*******************************************
KEY_STATUS_e key_check(KEY_e key)
{
    if(key_get(key) == KEY_DOWN)
    {
        pca_delay_ms(10);
        if( key_get(key) == KEY_DOWN)
        {
            return KEY_DOWN;
        }
    }
    return KEY_UP;
}