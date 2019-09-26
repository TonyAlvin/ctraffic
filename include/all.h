#ifndef __all_h__
#define __all_h__

#include "BtFeatures.h"
/*************���̲����궨��******************/
/******���ܼ�******/
#define F1 15104
#define F2 15360
#define F3 15616
#define F4 15872
#define ESC 283
#define ENTER 7181
#define START 14624 //BLANK�ո�  ����ʱ��ʼ
#define PAUSE 14624 //BLANK too
#define REPLAY 4978 //r ������ʱ�ز�replay
#define BACK 3592   //backspace���غ�ɾ��
#define DELETE 21248
#define LEFT 19200
#define RIGHT 19712
#define TAB 3849
/******���ּ�******/
#define ZERO 2864
#define ONE 561
#define TWO 818
#define THREE 1075
#define FOUR 1332
#define FIVE 1589
#define SIX 1846
#define SEVEN 2103
#define EIGHT 2360
#define NINE 2617

/******Menu�����ʼ�ͽ�������******/
#define STARTX 0
#define ENDX 1029
#define STARTY 0
#define ENDY 767

/*****Button DX, DY��ť��Ե��������ֵ*******/
#define BTDX 3
#define BTDY 2

/**************���Ƴ��ĺ궨��****************/
#define P 3.1415926 //Բ����
#define A 1.3       //���ӵĳߴ����ϵ��
#define D 13        //�������ĵ���ͷ��β�ľ��Ծ���
#define WC 5        //ͣ���ߵ������
#define RATE 3      //���ӵ��ٶȱ��ʻ�׼
#define ALARMDIS 32 //���������ľ������
#define DIS 37      //�̾������(*���ǵ�ת�Ǽ��������������*)
#define DIL 74      //���������

/**********��ť�ṹ***********/
struct BUTTON
{
    int btx1;     //��x����
    int bty1;     //��y����
    int btx2;     //βx����
    int bty2;     //βy����
    char *btname; //��ť����
    char *btmsg;  //��ʾ��Ϣ
    void (*feature)(void);
};
typedef struct BUTTON BT;
/***********�����ṹ************/
struct INPUT_BOX
{
    int bx0;    //��x����
    int by0;    //��y����
    int blen;   //��
    int bwid;   //��
    int bn[2];  //�洢����
    int bstate; //�����״̬
};
typedef struct INPUT_BOX BOX;

/***************���Ľṹ****************/
struct CarStruct
{
    int x;         //�������ĵ�ǰx����
    int y;         //�������ĵ�ǰy����
    int color;     //������ɫ
    int cnum;      //���ƺ�
    int alarm;     //������뾯��
    int dirt1;     //�������ڵĳ�ʼ·��
    int dirt2;     //����ת���·��
    int speed;     //���ӵ�ǰ�ٶ�
    int std_speed; //���ӱ������ٶ�
    int spflag;    //���ٱ�־λ
    int justment;  //��λΪ����·����λΪ���ڵ�
    int count;     //��ת��ʱ����ǲ�
    int flag;      //���ڱ�־�����Ƿ�ʻ������
    double angle;  //���ӵ�ǰ�ĽǶȣ�����Ϊ��ȣ���ʱ��
};
typedef struct CarStruct CAR;

/*����ť�Ķ���**��ȫ�ֱ������ı�ֵ**ֻ�ǰ�ť��Ϣ�Ķ���*/
static BT bt_czsm = {STARTX + 10, STARTY + 8, STARTX + 10 + 71, STARTY + 8 + 11, "����˵��F1", "���ڱ�ϵͳ�����Ľ���", BtIntro};
static BT bt_gnjs = {STARTX + 81 + 20, STARTY + 8, STARTX + 81 + 20 + 71, STARTY + 19, "���ܽ���F2", "���ڱ�ϵͳ���ܵļ��", BtIntro};
static BT bt_about = {STARTX + 172 + 20, STARTY + 8, STARTX + 172 + 20 + 71, STARTY + 19, "�����Ŷ�F3", "������Ա�ļ��", BtAbout};
static BT bt_exit = {ENDX - 33 - 36, STARTY + 8, ENDX - 10, STARTY + 19, "�˳�ESC", "���½��˳�ϵͳ", BtExit};

#endif