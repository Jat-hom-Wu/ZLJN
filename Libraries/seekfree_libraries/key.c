#include "key.h"


//**************************************************
// ���� KEY ��Ŷ�Ӧ�Ĺܽ�
//**************************************************
PTXn_e KEY_PTxn[KEY_MAX] = {P3_2, P3_3,P5_0,P5_1,P4_0};	  //��Ҫ����pcb��ӻ�ɾ����ַ


//******************************************************************
//   @brief      ��ʼ��key�˿�(key С�� KEY_MAX ʱ��ʼ�� ��Ӧ�˿ڣ������ʼ��ȫ���˿�)
//   @param      KEY_e    KEY���
//   @since      v5.0
//   Sample usage:       key_init (KEY_U);    //��ʼ�� KEY_U
//****************************************************************** 
void key_init(KEY_e key)
{
    if(key < KEY_MAX)
    {
       gpio_mode(KEY_PTxn[key],GPO_PP);        //��ʼ��������ѡ������ʽ
    }
    else
    {
        key = KEY_MAX;

        //��ʼ��ȫ�� ����
        while(key--)
        {
            gpio_mode(KEY_PTxn[key],GPO_PP);
        }

    }
}

//*******************************************
//  @brief      ��ȡkey״̬��������ʱ������
//  @param      KEY_e           KEY���
//  @return     KEY_STATUS_e    KEY״̬��KEY_DOWN��KEY_DOWN��
//  @since      v5.0
//  Sample usage:
/*                    if(key_get(KEY_U) ==  KEY_DOWN)
                    {
                        printf("\n��������")
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
//  @brief      ��ȡkey״̬������ʱ������
//  @param      KEY_e           KEY���
//  @return     KEY_STATUS_e    KEY״̬��KEY_DOWN��KEY_DOWN��
//  @since      v5.0
//  Sample usage:
/*                    if(key_get(KEY_U) ==  KEY_DOWN)
                    {
                        printf("\n��������")
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