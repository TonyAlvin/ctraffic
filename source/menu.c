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

void welcome(void);               //欢迎界面
void byebye(void);                //结束页面
void Simulation(void);            //仿真函数
void CountRunTimes(void);         //记录运行次数
void InitTimeCounter(counter *t); //初始化计时器
void TimeCounter(counter *t);     //计时器

/****************************************
 * 菜单函数：调度各个程序功能
 * 参数：无
 * 返回值：void
*****************************************/
void menu(void)
{
    welcome();
    Simulation();
    byebye();
}

/****************************************
 * ***函数名，void CountRunTimes(void)
 * ***参数·返回值：void
 * ***函数功能，记录程序运行次数，记录悲惨的奋斗历史
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
    delay(1000); //暂停一秒，让人看清
    return;
}

/********************************************
 * ***函数名：void Simulation(void)
 * ***参数：返回值：void
 * ***函数功能：仿真（本程序的核心功能）
 * ******************************************/
void Simulation(void)
{
    int i;
    int car_num;      //存储车的数量
    CAR *stright_car; //直道车辆链表
    CAR *turn_car;    //路口车辆链表
    counter t;        //计时器

    InitTimeCounter(&t);           //初始化计时器
    InitCarNum(&car_num);          //初始化车辆数量调整滑块
    CreatCarList(&stright_car, 3); //初始化车辆链表
    CreatCarList(&turn_car, 0);    //初始化路口车辆链表
    DrawRoad();                    //画马路
    DrawMenu();                    //画菜单

    while (1) //总循环体
    {
        SetCarNum(&car_num);                             //设置车数量
        MouseRead();                                     //刷新鼠标
        ButtonRefresh();                                 //刷新按钮显示按钮效果
        CarListDispatch(stright_car, turn_car, car_num); //车辆链表调度
        if (kbhit())                                     //键盘控制
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
                PutHZ24(200, 300, "退出仿真", WHITE, 6, 10, 0, 30);
                delay(1500);
                break;
            }
        }
        TimeCounter(&t); //计时
        delay(30);
        // TurnLeftCar(stright_car->next);//测试转弯用
        // DrawCar(stright_car->next);
    }
}

/**********************************************
 * ***函数名，void InitTimeCounter(counter *t)
 * ***参数，计时器结构体。返回值，空
 * ***函数功能，初始化计时器
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
    PutHZ24(20, 694, "相对时间", WHITE, 1, 0, 1, 0);
    PutAsc(20, 720, res, WHITE, 2, 2);
}

/**********************************************
 * ***函数名，void TimeCounter(counter *t)
 * ***参数，计时器结构体。返回值，空
 * ***函数功能，计时器v
***********************************************/
void TimeCounter(counter *t)
{
    char res[8]; //输出用
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
    if (t->min > 10) //将分位转换为字符串
    {
        itoa(t->min, res, 10);
    }
    else
    {
        res[0] = '0';
        itoa(t->min, res + 1, 10);
    }
    res[2] = ':';
    if (t->sec > 10) //将秒位转换为字符串
    {
        itoa(t->sec, res + 3, 10);
    }
    else
    {
        res[3] = '0';
        itoa(t->sec, res + 4, 10);
    }
    PutAsc(20, 720, res, WHITE, 2, 2); //输出结果
}

/***********************************************
 * ***函数名，void welcome(void)
 * ***参数·返回值，void
 * ***功能，欢迎页面
*************************************************/
void welcome(void)
{
    int i;
    typedef struct IntroStruct //介绍界面结构体 作用范围仅在此界面
    {
        int x;
        int y;
        char *stc; //stc：要显示的信息
        unsigned char color;
        unsigned char charsize;
        int fillship;
        int distance;
    } IntroWords;
    IntroWords sentence[4] = {
        {150, 160, "智能交通图形仿真软件", RED, 3, 0, 2},
        {370, 300, "团队介绍", RED, 2, 1, 60},
        {296, 350, "董国庆 U201814993", BLUE, 2, 1, 0},
        {296, 400, "陈兆朗 U201812992", BLUE, 2, 1, 0},
    };
    IntroWords *p = sentence;
    MouseHide(); //暂时隐藏鼠标
    setfillstyle(1, DARKGRAY);
    setcolor(WHITE);
    //画一个由小变大的圆
    for (i = 0; i < 650; i += 5)
    {
        fillellipse(ENDX / 2, ENDY / 2, i, i);
        delay(10);
    }
    //显示信息
    for (i = 0; i < 4; i++)
    {
        PutHZ24(p[i].x, p[i].y, p[i].stc, p[i].color, p[i].charsize, 3, 1, 20);
        //动画效果
        delay(100);
    }
    PutHZ24(850, 743, "任意键继续...", WHITE, 1, 0, 1, 0);
    //按任意键继续
    while (1)
    {
        if (kbhit())
            break;
    }
    cleardevice(); //清屏
    MouseShow();   //显示鼠标
    return;
}

/***********************************************
 * ***函数名，void byebye(void)
 * ***参数·返回值，void
 * ***功能，结束页面
*************************************************/
void byebye(void)
{
    int i;
    cleardevice();
    setfillstyle(1, DARKGRAY);
    setcolor(WHITE);
    //一个逐渐缩小的圆
    for (i = 540; i > 10; i -= 5)
    {
        cleardevice();
        fillellipse(ENDX / 2, ENDY / 2, i, i);
        delay(10);
    }
    cleardevice();
    //退出动画
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