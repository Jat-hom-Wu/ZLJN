#ifndef  __KEY__H
#define  __KEY__H

#include "headfile.h"

#define PTXn_e		unsigned char


//�����˿ڵ�ö��
typedef enum
{
    KEY_1,  //++
    KEY_2,  //--

    KEY_3,  //++
    KEY_4,  //--
	
		KEY_5,	//����Ϊ0

    KEY_MAX,
} KEY_e;


//key״̬ö�ٶ���
typedef enum
{
    KEY_DOWN  =   0,         //��������ʱ��Ӧ��ƽ
    KEY_UP    =   1,         //��������ʱ��Ӧ��ƽ

    KEY_HOLD,               //��������(���ڶ�ʱ����ɨ��)

} KEY_STATUS_e;


//������Ϣ�ṹ��
typedef struct
{
    KEY_e           key;        //�������
    KEY_STATUS_e    status;     //����״̬
} KEY_MSG_t;


extern void            key_init(KEY_e key);            // KEY��ʼ������(key С�� KEY_MAX ʱ��ʼ�� ��Ӧ�˿ڣ������ʼ��ȫ���˿�)
extern KEY_STATUS_e    key_get(KEY_e key);             //���key״̬��������ʱ������
extern KEY_STATUS_e    key_check(KEY_e key);           //���key״̬������ʱ������



#endif		  