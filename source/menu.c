#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "svgasub.h"
#include <graphics.h>
#include "light.h"
#include "mouse.h"
#include "hanzi.h"
#include "draw.h"
#include "dispatch.h"
#include "all.h"
#include <bios.h>

void welcome(void);               //��ӭ����
void byebye(void);                //����ҳ��
void Simulation(void);            //���溯��
void CountRunTimes(void);         //��¼���д���
void InitTimeCounter(counter *t); //��ʼ����ʱ��
void TimeCounter(counter *t);     //��ʱ��

/****************************************
 * �˵����������ȸ���������
 * ��������
 * ����ֵ��void
*****************************************/
void menu(void)
{
    welcome();
    Simulation();
    byebye();
}

/****************************************
 * ***��������void CountRunTimes(void)
 * ***����������ֵ��void
 * ***�������ܣ���¼�������д�������¼���ҵķܶ���ʷ
*****************************************/
void CountRunTimes(void)
{
    int times = 0;
    FILE *fp;
    if ((fp = fopen(".\\resource\\data.dat", "rb+")) == NULL)
    {
        printf("open data.dat fail~");
        delay(3000);
        exit(1);
    }
    fscanf(fp, "###%d###", &times);
    printf("No. %d Run", times);
    times++;
    rewind(fp);
    fprintf(fp, "###%d###", times);
    fclose(fp);
    delay(1000); //��ͣһ�룬���˿���
    return;
}

/********************************************
 * ***��������void Simulation(void)
 * ***����������ֵ��void
 * ***�������ܣ����棨������ĺ��Ĺ��ܣ�
 * ******************************************/
void Simulation(void)
{
    int i;
    int car_num;      //�洢��������
    CAR *stright_car; //ֱ����������
    CAR *turn_car;    //·�ڳ�������
    counter t;        //��ʱ��

    InitTimeCounter(&t);           //��ʼ����ʱ��
    InitCarNum(&car_num);          //��ʼ������������������
    CreatCarList(&stright_car, 3); //��ʼ����������
    CreatCarList(&turn_car, 0);    //��ʼ��·�ڳ�������
    DrawRoad();                    //����·
    DrawMenu();                    //���˵�

    while (1) //��ѭ����
    {
        SetCarNum(&car_num);                             //���ó�����
        MouseRead();                                     //ˢ�����
        ButtonRefresh();                                 //ˢ�°�ť��ʾ��ťЧ��
        CarListDispatch(stright_car, turn_car, car_num); //�����������
        if (kbhit())                                     //���̿���
        {
            i = bioskey(0);
            if (i == PAUSE)
                while (1)
                {
                    ButtonRefresh();
                    MouseRead();
                    if (bioskey(0) == START)
                        break;
                }
            else if (i == ESC)
            {
                PutHZ24(200, 300, "�˳�����", WHITE, 6, 10, 0, 30);
                delay(1500);
                break;
            }
        }
        TimeCounter(&t); //��ʱ
        delay(30);
        // TurnLeftCar(stright_car->next);//����ת����
        // DrawCar(stright_car->next);
    }
}

/**********************************************
 * ***��������void InitTimeCounter(counter *t)
 * ***��������ʱ���ṹ�塣����ֵ����
 * ***�������ܣ���ʼ����ʱ��
***********************************************/
void InitTimeCounter(counter *t)
{
    char res[8];
    t->hr = t->min = t->sec = 0;
    setfillstyle(1, BLACK);
    bar(20, 720, 119, 750);
    itoa(t->min, res, 10);
    res[2] = ':';
    itoa(t->sec, res + 3, 10);
    PutHZ24(20, 694, "���ʱ��", WHITE, 1, 0, 1, 0);
    PutAsc(20, 720, res, WHITE, 2, 2);
}

/**********************************************
 * ***��������void TimeCounter(counter *t)
 * ***��������ʱ���ṹ�塣����ֵ����
 * ***�������ܣ���ʱ��v
***********************************************/
void TimeCounter(counter *t)
{
    char res[8]; //�����
    t->sec++;
    if (t->min == 60)
    {
        t->hr++;
        t->min -= 60;
    }
    if (t->sec == 60)
    {
        t->sec -= 60;
        t->min += 1;
    }
    setfillstyle(1, BLACK);
    bar(20, 720, 119, 750);
    if (t->min > 10) //����λת��Ϊ�ַ���
    {
        itoa(t->min, res, 10);
    }
    else
    {
        res[0] = '0';
        itoa(t->min, res + 1, 10);
    }
    res[2] = ':';
    if (t->sec > 10) //����λת��Ϊ�ַ���
    {
        itoa(t->sec, res + 3, 10);
    }
    else
    {
        res[3] = '0';
        itoa(t->sec, res + 4, 10);
    }
    PutAsc(20, 720, res, WHITE, 2, 2); //������
}

/***********************************************
 * ***��������void welcome(void)
 * ***����������ֵ��void
 * ***���ܣ���ӭҳ��
*************************************************/
void welcome(void)
{
    int i;
    typedef struct IntroStruct //���ܽ���ṹ�� ���÷�Χ���ڴ˽���
    {
        int x;
        int y;
        char *stc; //stc��Ҫ��ʾ����Ϣ
        unsigned char color;
        unsigned char charsize;
        int fillship;
        int distance;
    } IntroWords;
    IntroWords sentence[4] = {
        {150, 160, "���ܽ�ͨͼ�η������", RED, 3, 0, 2},
        {370, 300, "�Ŷӽ���", RED, 2, 1, 60},
        {296, 350, "������ U201814993", BLUE, 2, 1, 0},
        {296, 400, "������ U201812992", BLUE, 2, 1, 0},
    };
    IntroWords *p = sentence;
    MouseHide(); //��ʱ�������
    setfillstyle(1, DARKGRAY);
    setcolor(WHITE);
    //��һ����С����Բ
    for (i = 0; i < 650; i += 5)
    {
        fillellipse(ENDX / 2, ENDY / 2, i, i);
        delay(10);
    }
    //��ʾ��Ϣ
    for (i = 0; i < 4; i++)
    {
        PutHZ24(p[i].x, p[i].y, p[i].stc, p[i].color, p[i].charsize, 3, 1, 20);
        //����Ч��
        delay(100);
    }
    PutHZ24(850, 743, "���������...", WHITE, 1, 0, 1, 0);
    //�����������
    while (1)
    {
        if (kbhit())
            break;
    }
    cleardevice(); //����
    MouseShow();   //��ʾ���
    return;
}

/***********************************************
 * ***��������void byebye(void)
 * ***����������ֵ��void
 * ***���ܣ�����ҳ��
*************************************************/
void byebye(void)
{
    int i;
    cleardevice();
    setfillstyle(1, DARKGRAY);
    setcolor(WHITE);
    //һ������С��Բ
    for (i = 540; i > 10; i -= 5)
    {
        cleardevice();
        fillellipse(ENDX / 2, ENDY / 2, i, i);
        delay(10);
    }
    cleardevice();
    //�˳�����
    PutAsc(400, 330, "ByeBye", WHITE, 4, 4);
    setfillstyle(1, BLACK);
    for (i = 3; i > 0; i--)
    {
        bar(593, 330, 730, 400);
        PutAsc(593, 330, "~---", WHITE, 4, 4);
        delay(500);
        bar(593, 330, 730, 400);
        PutAsc(593, 330, "-~--", WHITE, 4, 4);
        delay(500);
        bar(593, 330, 730, 400);
        PutAsc(593, 330, "--~-", WHITE, 4, 4);
        delay(500);
        bar(593, 330, 730, 400);
        PutAsc(593, 330, "---~", WHITE, 4, 4);
        delay(500);
    }
}