/*****创建时间： 2019.9.22********************
 *  
 * ***作者：董国庆
 * ***功能：创建车链表，车位置计算
 * ***最后修改时间：2019.10.22
*********************************************/
#include <stdio.h>
#include <graphics.h>
#include <time.h>
#include <stdlib.h>
#include <dos.h>
#include <math.h>
#include "all.h"
#include "mouse.h"
#include "dispatch.h"
#include "draw.h"
#include "hanzi.h"

//测试bug用
#include <string.h>

//函数声明（部分
void SolveAlarm(CAR *p);                                //处理警报函数
void MoveCar(CAR *p);                                   //直线移动车函数
void MoveCarLite(CAR *p);                               //进入或驶出路口时移动一小段距离
void CarSingle(CAR *car, CAR *p, CAR *prep, CAR *turn); //控制非路口内单个车还能输
int ChangeJustment(CAR *p);                             //通过路口后更改车辆justment函数
void PreScan(CAR *car, CAR *tar);                       //扫描前方车辆函数
void TurnCar(CAR *car, CAR *turn, CAR *p, CAR *prep);   //控制路口内车辆函数
void TurnLeftCar(CAR *p);                               //控制车辆左转
void TurnRightCar(CAR *p);                              //控制车辆右转
void TurnStrightCar(CAR *p, CAR *turn);                 //控制车辆直行
int TurnPreScan(CAR *car, CAR *p);                      //转弯前预扫描函数，用于判断前方路口是否压车至本路口
int GetLightStatusC(int just, int turn, int x, int y);  //获取十字路口当前车辆是否通行
int GetLightStatusT(int just, int turn, int x, int y);  //获取丁字路口当前车辆是否通行
void ExScan(CAR *car, int just);                        //扫描转弯后要进入的车道是否堆满
void TransformConfirm(CAR *tar);                        //判断是否需要变道
int TransPreScan(CAR *car, CAR *tar);                   //变道前预扫描函数
void TransformLane(CAR *tar);                           //控制车辆变道
void CorrectJustment(CAR *p);                           //更正justment错误
void CorrectCoordinate(CAR *p);                         //车辆驶出路口后矫正车道位置

/*********************************************
 * ***函数名，int RandInt(int min, int max)
 * ***参数，返回值：返回一个从min到max的随机数
 * ***函数功能：随机数发生函数，
 * ******************************************/
int RandInt(int min, int max)
{
    static unsigned int temp = 0;
    if (temp == 0)
        temp = (unsigned)time(NULL);
    srand(clock() * temp * 123);
    temp *= 234;
    return rand() % (max - min + 1) + min;
}

/*********************************************
 * ***函数名，void InitCar(CAR *newcar)
 * 函数功能：初始化车辆；
 * 参数：newcar:车辆指针
 * 返回值：void
***********************************************/
void InitCar(CAR *newcar)
{
    int RoadR[] = {UU1, UU2, DU1, DU2}, //存储路的坐标信息，方便随机生成
        RoadL[] = {UD1, UD2, DD1, DD2},
        RoadU[] = {LL1, LL2},
        RoadD[] = {LR1, LR2, RR1, RR2},
        ang;
    newcar->turn[0] = RandInt(1, 3); //生成转向信息
    newcar->turn[1] = RandInt(1, 3);
    newcar->turn[2] = RandInt(1, 3);
    newcar->turn[3] = RandInt(1, 2);
    ang = 90 * RandInt(0, 3);
    newcar->angle = ang;
    switch (ang / 90)
    {
    case 0:
        newcar->x = RoadD[RandInt(0, 3)];
        newcar->y = 767;
        switch (newcar->x)
        {
        case LR1:
            newcar->justment = 223;
            newcar->turn[1] = RandInt(1, 2);
            break;
        case LR2:
            newcar->justment = 224;
            newcar->turn[1] = 1 + 2 * RandInt(0, 1);
            break;
        case RR1:
            newcar->justment = 243;
            newcar->turn[2] = RandInt(1, 2);
            break;
        case RR2:
            newcar->justment = 244;
            newcar->turn[2] = 1 + 2 * RandInt(0, 1);
            break;
        default:
            PutAsc(500, 300, "Init0 Car Error", RED, 2, 2);
        }
        break;
    case 1:
        newcar->x = 1023;
        newcar->y = RoadR[RandInt(0, 3)];
        switch (newcar->y)
        {
        case UU1:
            newcar->justment = 121;
            newcar->turn[3] = 1;
            break;
        case UU2:
            newcar->justment = 122;
            newcar->turn[3] = RandInt(1, 2);
            break;
        case DU1:
            newcar->justment = 151;
            newcar->turn[2] = 1 + 2 * RandInt(0, 1);
            break;
        case DU2:
            newcar->justment = 152;
            newcar->turn[2] = RandInt(1, 2);
            break;
        default:
            PutAsc(500, 300, "Init1 Car Error", RED, 2, 2);
        }
        break;
    case 2:
        newcar->x = RoadU[RandInt(0, 1)];
        newcar->y = 42;
        switch (newcar->x)
        {
        case LL1:
            newcar->justment = 201;
            newcar->turn[0] = 1 + 2 * RandInt(0, 1);
            break;
        case LL2:
            newcar->justment = 202;
            newcar->turn[0] = RandInt(1, 2);
            break;
        default:
            PutAsc(500, 300, "Init2 Car Error", RED, 2, 2);
        }
        break;
    case 3:
        newcar->x = 1;
        newcar->y = RoadL[RandInt(0, 3)];
        switch (newcar->y)
        {
        case UD1:
            newcar->justment = 103;
            newcar->turn[0] = RandInt(1, 2);
            break;
        case UD2:
            newcar->justment = 104;
            newcar->turn[0] = 1 + 2 * RandInt(0, 1);
            break;
        case DD1:
            newcar->justment = 133;
            newcar->turn[1] = RandInt(1, 2);
            break;
        case DD2:
            newcar->justment = 134;
            newcar->turn[1] = 1 + 2 * RandInt(0, 1);
            break;
        default:
            PutAsc(500, 300, "Init3 Car Error", RED, 2, 2);
        }
        break;
    default:
        PutAsc(500, 300, "Init Car Error", RED, 2, 2);
    }
    newcar->std_speed = RandInt(3, 8); //生成初始速度
    newcar->speed = newcar->std_speed; //当前速度=初始速度
    newcar->color = RandInt(2, 8);     //车颜色随机
    newcar->alarm = 0;                 //与前车距离初始化为零
    newcar->flag = 0;                  //驶出屏幕标记，初始化为零
    newcar->count = 0;
    // DrawCar(newcar);
}

//测试时生成链表使用，无实际意义
void CreatCarList(CAR **head, int n)
{
    int i;
    CAR **current = head;
    *current = (CAR *)malloc(sizeof(CAR));
    for (i = 0; i < n; i++)
    {
        current = &((*current)->next);
        *current = (CAR *)malloc(sizeof(CAR));
        InitCar(*current);
        // DrawCar(*current);
        // delay(1000);
    }
    (*current)->next = NULL;
}

/********************************************
 * ***函数名,void InitCarNum(int *num)
 * ***参数，存储车相对数量的地址，返回值，void
 * ***函数功能，初始化调整车相对数量的滑块。
*********************************************/
void InitCarNum(int *num)
{
    setfillstyle(2, WHITE); //画滑块
    bar(337, 720, 663, 750);
    // setcolor(GREEN);
    // setlinestyle(1, 0, 3);
    // rectangle(my - 5, 723, my + 5, 747);
    setcolor(YELLOW);
    setfillstyle(1, YELLOW);
    circle(350, 735, 12);
    floodfill(350, 735, YELLOW);
    PutAsc(333, 694, "MIN", GREEN, 2, 2);
    PutAsc(620, 694, "MAX", RED, 2, 2);
    PutHZ16(430, 694, "拖动滑块调整车数量", WHITE, 1, 1, 0, 0);
    *num = 9950;
}

/********************************************
 * ***函数名，void SetCarNum(int *num)
 * ***返回值，int *num车的相对数量。参数，void
 * ***函数功能：显示由用户决定生成车数辆的滑块。
*********************************************/
void SetCarNum(int *num)
{
    // char a[6];
    int mx, my;
    if (MousePressIn(350, 720, 650, 750)) //只有滑块状态改变的时候画滑块
    {
        MouseXY(&mx, &my);
        MouseHide(); //隐藏鼠标，防止乱套
        delay(20);
        mx = (int)((mx - 350) / 5); //将长为300的滑块分为150份
        my = mx * 5 + 350;
        setfillstyle(2, WHITE); //画滑块
        bar(337, 720, 663, 750);
        // setcolor(GREEN);
        // setlinestyle(1, 0, 3);
        // rectangle(my - 5, 723, my + 5, 747);
        setcolor(YELLOW);
        setfillstyle(1, YELLOW);
        circle(my, 735, 12);
        floodfill(my, 735, YELLOW);
        MouseShow(); //画完图显示鼠标
        // setfillstyle(1, BLACK);
        // bar(400, 700, 480, 720);
        // itoa(9950 - mx * 5, a, 10);
        // PutAsc(400, 700, a, WHITE, 2, 2);
        *num = 9950 - mx * 15;
    }
}

/*******************************************
 * 函数功能：车辆整体调度
 * 参数： 车辆链表头
 * 返回值：无,
 * *****************************************/
void CarListDispatch(CAR *car, CAR *turn, int CarNum)
{
    // char a[5];
    CAR *road, *current, *precurrent, *newcar;
    road = car;
    // itoa((getpixel(LEFT_X + 38, UP_Y - 58) == GREEN), a, 10);
    // PutAsc(LEFT_X + 38, UP_Y - 58, a, WHITE, 2, 2);
    // itoa(GetLightStatusC(202, 1, LEFT_X, UP_Y), a, 10);
    // PutAsc(LEFT_X - 6, UP_Y, a, WHITE, 2, 2);
    // itoa(GetLightStatusC(103, 1, LEFT_X, UP_Y), a, 10);
    // PutAsc(LEFT_X + 15, UP_Y, a, WHITE, 2, 2);
    // itoa(GetLightStatusC(213, 1, LEFT_X, UP_Y), a, 10);
    // PutAsc(LEFT_X + 28, UP_Y, a, WHITE, 2, 2);
    // NormalControl(2);
    precurrent = road;
    for (current = precurrent->next; current != NULL; current = precurrent->next)
    {
        CarSingle(car, current, precurrent, turn);
        precurrent = precurrent->next;
    }
    precurrent = turn;
    for (current = precurrent->next; current != NULL; current = precurrent->next)
    {
        TurnCar(car, turn, current, precurrent);
        precurrent = precurrent->next;
    }
    if (RandInt(0, 10000) > CarNum)
    {
        newcar = (CAR *)malloc(sizeof(CAR));
        InitCar(newcar);
        newcar->next = car->next;
        car->next = newcar;
    }
}

void SolveConflict(CAR *p)
{
    switch (p->justment)
    {
    case 233:
        p->turn[3] = 2;
        break;
    case 234:
        p->turn[3] = 3;
        break;
    case 113:
        p->turn[3] = 1;
        break;
    case 114:
        p->turn[3] = 1 + 2 * RandInt(0, 1);
        break;
    case 121:
        p->turn[3] = 1;
        break;
    case 122:
        p->turn[3] = RandInt(1, 2);
        break;
    default:
        PutAsc(p->x + 30, p->y, "Solve error", WHITE, 2, 2);
    }
}

/**********************************************
 * 函数名：int JudgeToCross(CAR *p)
 * 参数：               当前车指针
 * 返回值：判断在进入路口范围内，0为不在路口
***********************************************/
int JudgeToCross(CAR *p, int range)
{
    if ((p->x >= LEFT_X && p->x <= LEFT_X + 50 && p->y >= UP_Y + 40 && p->y <= UP_Y + 50 + range) ||
        (p->x >= LEFT_X - 50 && p->x <= LEFT_X && p->y >= UP_Y - 50 - range && p->y <= UP_Y - 40) ||
        (p->x >= LEFT_X - 50 - range && p->x <= LEFT_X - 40 && p->y >= UP_Y && p->y <= UP_Y + 50) ||
        (p->x >= LEFT_X + 40 && p->x <= LEFT_X + 50 + range && p->y >= UP_Y - 50 && p->y <= UP_Y))
        return 1;
    else if ((p->x >= LEFT_X && p->x <= LEFT_X + 50 && p->y >= DOWN_Y + 40 && p->y <= DOWN_Y + 50 + range) ||
             (p->x >= LEFT_X - 50 && p->x <= LEFT_X && p->y >= DOWN_Y - 50 - range && p->y <= DOWN_Y - 40) ||
             (p->x >= LEFT_X - 50 - range && p->x <= LEFT_X - 40 && p->y >= DOWN_Y && p->y <= DOWN_Y + 50) ||
             (p->x >= LEFT_X + 40 && p->x <= LEFT_X + 50 + range && p->y >= DOWN_Y - 50 && p->y <= DOWN_Y))
        return 2;
    else if ((p->x >= RIGHT_X && p->x <= RIGHT_X + 50 && p->y >= DOWN_Y + 40 && p->y <= DOWN_Y + 50 + range) ||
             (p->x >= RIGHT_X - 50 && p->x <= RIGHT_X && p->y >= DOWN_Y - 50 - range && p->y <= DOWN_Y - 40) ||
             (p->x >= RIGHT_X - 50 - range && p->x <= RIGHT_X - 40 && p->y >= DOWN_Y && p->y <= DOWN_Y + 50) ||
             (p->x >= RIGHT_X + 40 && p->x <= RIGHT_X + 50 + range && p->y >= DOWN_Y - 50 && p->y <= DOWN_Y))
        return 3;
    else if ((p->x >= RIGHT_X && p->x <= RIGHT_X + 50 && p->y >= UP_Y + 40 && p->y <= UP_Y + 50 + range) ||
             (p->x >= RIGHT_X - 50 && p->x <= RIGHT_X && p->y >= UP_Y - 50 - range && p->y <= UP_Y - 40) ||
             (p->x >= RIGHT_X - 50 - range && p->x <= RIGHT_X - 40 && p->y >= UP_Y && p->y <= UP_Y + 50) ||
             (p->x >= RIGHT_X + 40 && p->x <= RIGHT_X + 50 + range && p->y >= UP_Y - 50 && p->y <= UP_Y))
        return 4;
    else
        return 0;
}

/**********************************************************************
 * 函数名：void CarSingle(CAR *car, CAR *p, CAR *prep, CAR *turn)
 * 参数：              直道车链表头  当前车，当前车的前一个，转向车链表头
 * 返回值：空，
 * 函数功能：单个直道车控制，遇路口并判断绿灯后将车移至路口链表
 * *******************************************************************/
void CarSingle(CAR *car, CAR *p, CAR *prep, CAR *turn)
{
    char a[5];
    int place = JudgeToCross(p, 20);
    if (p->x < 0 || p->x > 1023 || p->y < 44 || p->y > 767)
    {
        prep->next = p->next;
        free(p);
        return;
    }
    switch (place)
    {
    //非路口只需判断前方有无车辆即可行驶
    case 0:
        if (p->flag == 0) //不需要变道，
        {
            PreScan(car, p);
            SolveAlarm(p);
            MoveCar(p);
        }
        else //需要变道行驶，为前方转弯做准备
        {
            switch (TransPreScan(car, p))
            {
            case 0:
                PutAsc(p->x + 150, p->y, "tran", WHITE, 2, 2);
                TransformLane(p);
                break;
            case 1:
                p->alarm = 1;
                SolveAlarm(p);
                break;
            case 2:
                p->speed = 0;
            }
            MoveCar(p);
        }
        break;
    //十字路口统一处理
    case 1:
    case 2:
    case 3: //
        if ((GetLightStatusC(p->justment, p->turn[place - 1], (p->x < 500) ? 250 : 750, (p->y < 350) ? 170 : 570)) && !TurnPreScan(car, p))
        { //绿灯行
            // PreScan(turn, p);
            // p->speed = p->speed / 4;//进入路口减速慢行
            // SolveAlarm(p);
            MoveCarLite(p);
            prep->next = p->next; //将车移入路口链表
            p->next = turn->next;
            turn->next = p;
            itoa(p->turn[place - 1], a, 10);
            PutAsc(p->x, p->y, a, RED, 2, 2);
            return; //将车移出后单车在turn car函数内处理
        }
        break; //红灯停，不对车的位置及所在链表进行操作
    //丁字路口单独处理
    case 4:
        SolveConflict(p);
        if ((GetLightStatusT(p->justment, p->turn[place - 1], 750, 170)) && !TurnPreScan(car, p))
        { //绿灯行
            // PreScan(turn, p);
            // p->speed = p->speed / 4;//进入路口减速慢行
            // SolveAlarm(p);
            MoveCarLite(p);
            prep->next = p->next; //将车移入路口链表
            p->next = turn->next;
            turn->next = p;
            // PutAsc(p->x, p->y, "IN", RED, 2, 2);
            return; //将车移出后单车在turn car函数内处理
        }
        break; //红灯停，不对车的位置及所在链表进行操作
    //车辆justment数据有误
    default:
        PutAsc(p->x, p->y, "find place error", RED, 2, 2);
    }
    DrawCar(p);
    // itoa(place, a, 10);
    // setfillstyle(0, BLACK); //测试用
    // setcolor(BLACK);
    // bar(300, 400, 400, 450);
    // PutAsc(300, 400, a, RED, 2, 2);
    // itoa(p->turn[JudgeToCross(p) - 1], a, 10);
    // PutAsc(300, 430, a, RED, 2, 2);
    // itoa(GetLightStatusC(p->justment, p->turn[place - 1], (p->x < 500) ? 250 : 750, (p->y < 400) ? 270 : 570), a, 10);
    // PutAsc(330, 430, a, RED, 2, 2);
}

/*******获取十字路口红绿灯状态函数*************************
 * int GetLightStatusC(int just, int turn, int x, int y)
 * 参数：            车辆当前车道 要走的方向   路口中心坐标
 * 参数说明：要走的方向1:直行·2：
 * 返回值：1:可以走 0：不能走
 * *******************************************************/
int GetLightStatusC(int just, int turn, int x, int y)
{
    int flag;
    switch (turn)
    {
    case 1:
        //直行
        if (just / 100 == 1) //水平方向
        {
            if (just % 10 < 3)
            {
                flag = getpixel(x - 58, y - 38);
            }
            else
            {
                flag = getpixel(x + 58, y + 38);
            }
        }
        else //竖直方向
        {
            if (just % 10 < 3)
            {
                flag = getpixel(x - 38, y + 58);
            }
            else
            {
                flag = getpixel(x + 38, y - 58);
            }
        }
        return (flag == GREEN);
    case 2:
        //左转
        if (just / 100 == 1) //水平方向车左转
        {
            if (just % 10 < 3)
            {
                flag = getpixel(x - 58, y - 12);
            }
            else
            {
                flag = getpixel(x + 58, y + 12);
            }
        }
        else //竖直方向左转
        {
            if (just % 10 < 3)
            {
                flag = getpixel(x - 12, y + 58);
            }
            else
            {
                flag = getpixel(x + 12, y - 58);
            }
        }
        return (flag == GREEN);
    case 3: //右转 //右转一直绿灯
        return 1;
    default: //错误处理
        PutAsc(x, y, "LightErrorC", RED, 1, 1);
        return 0;
    }
}

/*******获取丁字路口红绿灯状态函数***************************
 * int GetLightStatusT(int just, int turn, int x, int y)
 * 参数：            车辆当前车道 要走的方向   路口中心坐标
 * 返回值：1:可以走 0：不能走
 * ********************************************************/
int GetLightStatusT(int just, int turn, int x, int y)
{
    int flag;
    switch (turn)
    {
    case 1:
        //直行
        if (just % 10 < 3)
        {
            flag = getpixel(x - 58, y - 38);
        }
        else
        {
            flag = getpixel(x + 58, y + 38);
        }
        return (flag == GREEN);
    case 2:
        //左转
        if (just % 10 < 3) //竖直方向左转
        {
            flag = getpixel(x - 58, y - 12);
        }
        else //水平方向左转
        {
            flag = getpixel(x + 24, y - 58);
        }
        return (flag == GREEN);
    case 3:
        //右转 //右转一直绿灯
        return 1;
    default: //错误处理
        PutAsc(x, y, "LightErrorT", RED, 1, 1);
        return 0;
    }
}

//判断车是否在路口内是返回1否返回0
int JudgeInCross(CAR *p, int range)
{
    // setcolor(WHITE); //标定范围使用
    // setlinestyle(0, 0, 1);
    // rectangle(200 - range, 120 - range, 300 + range, 220 + range);
    // rectangle(200 - range, 520 - range, 300 + range, 620 + range);
    // rectangle(700 - range, 120 - range, 800 + range, 220 + range);
    // rectangle(700 - range, 520 - range, 800 + range, 620 + range);
    if ((p->x > 200 - range) && (p->x < 300 + range) && (p->y > 120 - range) && (p->y < 220 + range))
        return 1;
    else if ((p->x > 200 - range) && (p->x < 300 + range) && (p->y > 520 - range) && (p->y < 620 + range))
        return 2;
    else if ((p->x > 700 - range) && (p->x < 800 + range) && (p->y > 500 - range) && (p->y < 620 + range))
        return 3;
    else if ((p->x > 700 - range) && (p->x < 800 + range) && (p->y > 120 - range) && (p->y < 220 + range))
        return 4;
    else
        return 0;
}

//控制路口内车辆
void TurnCar(CAR *car, CAR *turn, CAR *p, CAR *prep)
{
    // char a[5]; //调bug专用
    if (p->y < 35)
    {
        prep->next = p->next;
        free(p);
        return;
    }
    switch (p->turn[JudgeInCross(p, 20) - 1])
    {
    case 1: //直行
        TurnStrightCar(p, turn);
        break;
    case 2: //左转
        TurnLeftCar(p);
        break;
    case 3: //右转
        TurnRightCar(p);
        break;
    default: //错误处理
        // p->justment == ChangeJustment(p);
        p->count = 0;
        PutAsc(p->x, p->y, "bad turn", RED, 1, 1);
        break;
    }
    DrawCar(p);
    if (p->count == 0) //判断车辆驶出路口，则移出路口链表，移入正常链表
    {
        MoveCar(p); //先移动一小段确保出路口
        // itoa(p->justment, a, 10);
        // PutAsc(p->x + 50, p->y + 32, a, WHITE, 2, 2);
        p->justment = ChangeJustment(p);
        TransformConfirm(p);
        // if (p->x > 200 && p->x < 700 && p->y > 120 && p->y < 720)
        //     p->flag = 1; //测试变道用
        prep->next = p->next;
        p->next = car->next;
        car->next = p;
        CorrectCoordinate(p);
        PutAsc(p->x + 50, p->y, "OUT", WHITE, 2, 2);
        // itoa(p->flag, a, 10); //调试bug要用
        // PutAsc(p->x + 150, p->y, a, WHITE, 2, 2);
    }
}

//控制车辆直行
void TurnStrightCar(CAR *p, CAR *turn)
{
    //路口减速慢行
    PreScan(turn, p);
    SolveAlarm(p);
    MoveCar(p);
    p->count++;
    if (!JudgeInCross(p, 20))
        p->count = 0;
}

//控制车辆左转
void TurnLeftCar(CAR *car)
{
    if (JudgeInCross(car, 9) && car->count < 106)
    {
        car->angle += 6;
        car->x = car->x - (int)(8.2 * sin(car->angle * PI / 180));
        car->y = car->y - (int)(8.2 * cos(car->angle * PI / 180));
        car->count += 6;
    }
    else
    {
        //直行角度矫正
        car->angle = (int)(car->angle / 90 + 0.5) * 90;
        MoveCar(car);
    }
    car->count++;
    if (car->angle >= 360)
        car->angle -= 360;
    else if (car->angle < 0)
        car->angle += 360;
    if (!JudgeInCross(car, 20))
    {
        //转弯角度矫正
        car->angle = (int)(car->angle / 90 + 0.5) * 90;
        //转弯结束标记位
        car->count = 0;
    }
}

//控制车辆右转
void TurnRightCar(CAR *car)
{
    if (JudgeInCross(car, 14) && car->count < 99)
    {
        car->angle -= 10;
        car->x = car->x - (int)(5.2 * sin(car->angle * PI / 180));
        car->y = car->y - (int)(5.2 * cos(car->angle * PI / 180));
        car->count += 10;
    }
    else
    {
        //直行角度矫正
        car->angle = (int)(car->angle / 90 + 0.5) * 90;
        MoveCar(car);
    }
    car->count++;
    if (car->angle >= 360)
        car->angle -= 360;
    else if (car->angle < 0)
        car->angle += 360;
    if (!JudgeInCross(car, 22)) //判断出路口
    {
        //转弯角度矫正
        car->angle = (int)(car->angle / 90 + 0.5) * 90;
        //转弯结束标记位
        car->count = 0;
    }
}

//直行或转弯前预扫描将要进入的车道
int TurnPreScan(CAR *car, CAR *p) //转弯前直道预扫描函数（共用）
{
    CAR *current;
    int pre = ChangeJustment(p), position = JudgeToCross(p, 20);
    char a[5]; //调bug专用
    itoa(p->justment, a, 10);
    PutAsc(p->x + 300, p->y, a, WHITE, 2, 2);
    for (current = car->next; current != NULL; current = current->next)
        if ((current->justment == pre) && (JudgeInCross(current, 40) == position))
            return 1;
    return 0;
}

void TransformConfirm(CAR *tar)
{
    switch (tar->justment)
    {
    case 113:
        if (tar->turn[3] == 3)
            tar->flag = 1;
        break;
    case 143:
        if (tar->turn[2] == 3)
            tar->flag = 1;
        break;
    case 144:
        if (tar->turn[2] == 2)
            tar->flag = 1;
        break; //走向路口3
    case 111:
        if (tar->turn[0] == 2)
            tar->flag = 1;
        break;
    case 112:
        if (tar->turn[0] == 3)
            tar->flag = 1;
        break;
    case 141:
        if (tar->turn[1] == 2)
            tar->flag = 1;
        break;
    case 142: //走向路口0
        if (tar->turn[1] == 3)
            tar->flag = 1;
        break;
    case 213:
        if (tar->turn[0] == 3)
            tar->flag = 1;
        break;
    case 214:
        if (tar->turn[0] == 2)
            tar->flag = 1;
        break;
    case 211:
        if (tar->turn[1] == 2)
            tar->flag = 1;
        break;
    case 212:
        if (tar->turn[1] == 3)
            tar->flag = 1;
        break;
    case 231:
        if (tar->turn[2] == 2)
            tar->flag = 1;
        break;
    case 232:
        if (tar->turn[2] == 3)
            tar->flag = 1;
        break;
    case 233:
        if (tar->turn[3] == 3)
            tar->flag = 1;
        break;
    case 234:
        if (tar->turn[3] == 2)
            tar->flag = 1;
        break;
    default:
        break;
    }
}

/******************************************
 * ***函数名，void PreScan(CAR *car, CAR *tar)
 * ***参数，car直道车链表，tar待扫描车辆
 * ***函数功能，直道扫描函数（共用）
******************************************/
void PreScan(CAR *car, CAR *tar)
{
    char a[4];
    CAR *current;
    tar->alarm = 0;
    for (current = car->next; current != NULL; current = current->next)
    {
        if (tar->justment == current->justment)
            switch (tar->justment)
            {
            case 101:
            case 102:
            case 111:
            case 112:
            case 121:
            case 122:
            case 131:
            case 132:
            case 141:
            case 142:
            case 151:
            case 152:
                if ((current->x < tar->x) && ((current->x + DIS) > tar->x))
                    tar->alarm = 2;
                else if (((current->x + DIS) <= tar->x) && ((current->x + DIL) > tar->x) && (tar->alarm != 2))
                    tar->alarm = 1; //将距离梯度化，在最短距离内停车，中距内慢速行驶，长距内以初始化速度行驶
                // else if (((current->x + DIL) <= tar->x) && (tar->alarm != 0))
                //     tar->alarm = 0;
                break;
            case 103:
            case 104:
            case 113:
            case 114:
            case 123:
            case 124:
            case 133:
            case 134:
            case 143:
            case 144:
            case 153:
            case 154:
                if ((current->x > tar->x) && ((current->x - DIS) < tar->x))
                    tar->alarm = 2;
                else if (((current->x - DIS) >= tar->x) && ((current->x - DIL) < tar->x) && (tar->alarm != 2))
                    tar->alarm = 1; //将距离梯度化，在最短距离内停车，中距内慢速行驶，长距内以初始化速度行驶
                // else if (((current->x - DIL) >= tar->x) && (tar->alarm != 0))
                //     tar->alarm = 0;
                break;
            case 201:
            case 202:
            case 211:
            case 212:
            case 221:
            case 222:
            case 231:
            case 232:
            case 241:
            case 242:
                if ((current->y > tar->y) && ((current->y - DIS) < tar->y))
                    tar->alarm = 2;
                else if (((current->y - DIS) >= tar->y) && ((current->y - DIL) < tar->y && (tar->alarm != 2)))
                    tar->alarm = 1; //将距离梯度化，在最短距离内停车，中距内慢速行驶，长距内以初始化速度行驶
                // else if (((current->y + DIL) >= tar->y) && (tar->alarm != 0))
                //     tar->alarm = 0;
                break;
            case 203:
            case 204:
            case 213:
            case 214:
            case 223:
            case 224:
            case 233:
            case 234:
            case 243:
            case 244:
                if ((current->y < tar->y) && ((current->y + DIS) > tar->y))
                    tar->alarm = 2;
                else if (((current->y + DIS) <= tar->y) && ((current->y + DIL) > tar->y && (tar->alarm != 2)))
                    tar->alarm = 1; //将距离梯度化，在最短距离内停车，中距内慢速行驶，长距内以初始化速度行驶
                // else if (((current->y + DIL) >= tar->y) && (tar->alarm != 0))
                //      tar->alarm = 0;
                break;
            default:
                PutAsc(tar->x, tar->y, "bad justment", RED, 2, 2);
                CorrectJustment(tar);
                break;
            }
    }
    itoa(tar->alarm, a, 10);
    PutAsc(tar->x + 20, tar->y, a, WHITE, 1, 1);
}

/********************************************
 * ***函数名，void CorrectCoordinate(CAR *p)
 * ***参数，要修正的车辆指针
 * ***返回值，void
 * ***函数功能，修正车辆出路口后的坐标错误
***********************************************/
void CorrectCoordinate(CAR *p)
{
    int road_x = (JudgeInCross(p, 40) > 2) ? RIGHT_X : LEFT_X, road_y = (JudgeInCross(p, 40) % 4 < 2) ? UP_Y : DOWN_Y;
    switch ((int)(p->angle / 90))
    {
    case 0:
        if (p->x < road_x + 25)
            p->x = road_x + 13;
        else
            p->x = road_x + 38;
        break;
    case 2:
        if (p->x < road_x - 25)
            p->x = road_x - 38;
        else
            p->x = road_x - 13;
        break;
    case 1:
        if (p->y < road_y - 25)
            p->y = road_y - 38;
        else
            p->y = road_y - 13;
        break;
    case 3:
        if (p->y < road_y + 25)
            p->y = road_y + 13;
        else
            p->y = road_y + 38;
        break;
    }
}

/********************************************
 * ***函数名，void CorrectJustment(CAR *p)
 * ***参数，要修正的车辆指针
 * ***返回值，void
 * ***函数功能，修正bad justment错误
***********************************************/
void CorrectJustment(CAR *p)
{
    switch ((int)(p->angle / 90))
    {
    case 0:
        if (p->x < 275)
        {
            if (p->y < 170)
                p->justment = 203;
            else if (p->y < 570)
                p->justment = 213;
            else
                p->justment = 223;
        }
        else if (p->x < 310)
        {
            if (p->y < 170)
                p->justment = 204;
            else if (p->y < 570)
                p->justment = 214;
            else
                p->justment = 224;
        }
        else if (p->x < 775)
        {
            if (p->y < 570)
                p->justment = 233;
            else
                p->justment = 243;
        }
        else
        {
            if (p->y < 570)
                p->justment = 234;
            else
                p->justment = 244;
        }
        break;
    case 2:
        if (p->x < 225)
        {
            if (p->y < 170)
                p->justment = 201;
            else if (p->y < 570)
                p->justment = 211;
            else
                p->justment = 221;
        }
        else if (p->x < 310)
        {
            if (p->y < 170)
                p->justment = 202;
            else if (p->y < 570)
                p->justment = 212;
            else
                p->justment = 222;
        }
        else if (p->x < 725)
        {
            if (p->y < 570)
                p->justment = 231;
            else
                p->justment = 241;
        }
        else
        {
            if (p->y < 570)
                p->justment = 232;
            else
                p->justment = 242;
        }
        break;
    case 1:
        if (p->y < 145)
        {
            if (p->x < 170)
                p->justment = 101;
            else if (p->x < 750)
                p->justment = 111;
            else
                p->justment = 121;
        }
        else if (p->y < 170)
        {
            if (p->x < 170)
                p->justment = 102;
            else if (p->x < 750)
                p->justment = 112;
            else
                p->justment = 122;
        }
        else if (p->y < 545)
        {
            if (p->x < 170)
                p->justment = 131;
            else if (p->x < 750)
                p->justment = 141;
            else
                p->justment = 151;
        }
        else
        {
            if (p->x < 170)
                p->justment = 132;
            else if (p->x < 750)
                p->justment = 142;
            else
                p->justment = 152;
        }
        break;
    case 3:
        if (p->y < 195)
        {
            if (p->x < 170)
                p->justment = 103;
            else if (p->x < 750)
                p->justment = 113;
            else
                p->justment = 123;
        }
        else if (p->y < 220)
        {
            if (p->x < 170)
                p->justment = 104;
            else if (p->x < 750)
                p->justment = 114;
            else
                p->justment = 124;
        }
        else if (p->y < 595)
        {
            if (p->x < 170)
                p->justment = 133;
            else if (p->x < 750)
                p->justment = 143;
            else
                p->justment = 153;
        }
        else
        {
            if (p->x < 170)
                p->justment = 134;
            else if (p->x < 750)
                p->justment = 144;
            else
                p->justment = 154;
        }
        break;
    }
}

int ChangeJustment(CAR *p)
{
    // char a[5]; //调bug专用
    // itoa(p->justment, a, 10);
    // PutAsc(p->x + 300, p->y, a, WHITE, 2, 2);
    switch (p->turn[JudgeInCross(p, 40) - 1])
    {
    case 1:
        if (p->justment > 190)
        {
            if ((p->justment % 10) <= 2)
                return p->justment + 10;
            else
                return p->justment - 10;
        }
        else
        {
            if ((p->justment % 10) <= 2)
                return p->justment - 10;
            else
                return p->justment + 10;
        }
    case 2:
        switch (p->justment)
        {
        case 103:
            return 203;
        case 112:
            return 212;
        case 213:
            return 102;
        case 202:
            return 113;
        case 133:
            return 213;
        case 142:
            return 222;
        case 212:
            return 143;
        case 223:
            return 132;
        case 122:
            return 232;
        case 233:
            return 112;
        case 243:
            return 142;
        case 232:
            return 153;
        case 143:
            return 233;
        case 152:
            return 242;
        default:
            PutAsc(p->x, p->y, "justment error", RED, 1, 1);
        }
        break;
    case 3:
        switch (p->justment)
        {
        case 104:
            return 211;
        case 111:
            return 204;
        case 201:
            return 101;
        case 214:
            return 114;
        case 211:
            return 131;
        case 224:
            return 144;
        case 134:
            return 221;
        case 141:
            return 214;
        case 114:
            return 231;
        case 234:
            return 124;
        case 244:
            return 154;
        case 231:
            return 141;
        case 144:
            return 241;
        case 151:
            return 234;
        default:
            PutAsc(p->x, p->y, "justment error", RED, 1, 1);
        }
        break;
    default:
        PutAsc(p->x, p->y, "turn error", RED, 1, 1);
    }
    return 0xffffff;
}

//直线行驶时计算车的位移
//输入参数：CAR*车辆指针
//返回值：空
void MoveCar(CAR *p)
{
    switch ((int)p->angle / 90)
    {
    case 0:
        p->y -= p->speed;
        break;
    case 1:
        p->x -= p->speed;
        break;
    case 2:
        p->y += p->speed;
        break;
    case 3:
        p->x += p->speed;
        break;
    }
}
/************************************************************
 * ***函数名，void MoveCarLite(CAR *p)
 * ***函数功能，进入或驶出路口时移动一小段距离，避免位置判断出错。
 * ***输入参数：CAR*车辆指针
 * ***返回值：空
*************************************************************/
void MoveCarLite(CAR *p)
{
    switch ((int)p->angle / 90)
    {
    case 0:
        p->y -= 2;
        break;
    case 1:
        p->x -= 2;
        break;
    case 2:
        p->y += 2;
        break;
    case 3:
        p->x += 2;
        break;
    }
}

//处理前方报警的函数(共用)
void SolveAlarm(CAR *p)
{
    // char a[5];
    switch (p->alarm)
    {
    case 0:                      //没有警报
        p->speed = p->std_speed; //置为初始化的速度
        break;
    case 1:                  //如果在中距内，有警报
        p->speed = 1 * RATE; //置为慢速
        break;
    case 2:           //如果在最短距离内，有警报
        p->speed = 0; //停车
        break;
    default:
        // itoa(p->alarm, a, 10);
        PutAsc(p->x, p->y, "bad alarm", WHITE, 2, 2);
        delay(1000);
        break;
    }
}

/****************************************************
 *  变道扫描函数int TransPreScan(CAR *car, CAR *tar)
 *  输入值：含全部车的链表；当前车
 *  返回值：是否能变道：0 可变道
 *                    1 不可变
 *                    2 停车等待
****************************************************/
int TransPreScan(CAR *car, CAR *tar)
{
    CAR *current;
    int i, flag = 0;
    if (tar->justment % 2 == 0)
        i = tar->justment - 1;
    else
        i = tar->justment + 1;
    for (current = car->next; current != NULL; current = current->next)
    {
        if ((current->justment == i) && current != tar->next)
        {
            switch (i)
            {
            case 111:
            case 112:
            case 141:
            case 142:
                if ((current->x < tar->x) && ((current->x + DIS) > tar->x))
                    flag = 1;
                break;
            case 113:
            case 114:
            case 143:
            case 144:
                if ((current->x > tar->x - DIL) && ((current->x) < tar->x))
                    flag = 1;
                break;
            case 211:
            case 212:
            case 231:
            case 232:
                if ((current->y < tar->y + DIL) && ((current->y) > tar->y))
                    flag = 1;
                break;
            case 213:
            case 214:
            case 233:
            case 234:
                if ((current->y > tar->y - DIL) && ((current->y) < tar->y))
                    flag = 1;
                break;
            default:
                PutAsc(tar->x, tar->y, "tran scan error", RED, 2, 2);
            }
        }
    }
    if (flag == 1)
    {
        if (JudgeToCross(tar, 80))
        {
            return 2; //2为要停车等待
        }
        return 1; //减速代转
    }
    else
    {
        return 0; //可以转
    }
}

void TransformLane(CAR *tar)
{
    CAR *AddedCar = NULL;
    if (tar->count == 0)
    {
        AddedCar = (CAR *)malloc(sizeof(CAR));
        AddedCar->angle = tar->angle;
        AddedCar->next = tar->next;
        AddedCar->speed = tar->speed;
        AddedCar->alarm = 0;
        tar->next = AddedCar;
        AddedCar->color = 0;
        if (tar->justment % 2 == 0)
        {
            AddedCar->justment = tar->justment - 1;
        }
        else
        {
            AddedCar->justment = tar->justment + 1;
        }
    }
    AddedCar = tar->next;
    switch (tar->justment)
    {
    case 111:
    case 113:
    case 141:
    case 143:
        if (tar->count == 0)
        {
            AddedCar->x = tar->x;
            AddedCar->y = tar->y - 25;
        }
        if (tar->count >= 0 && tar->count < 4)
        {
            tar->y = tar->y + 5;
            tar->count++;
        }
        else if (tar->count == 4)
        {
            tar->count = 0;
            tar->flag = 0;
            tar->y = tar->y + 4;
            if (tar->justment % 2 == 0)
            {
                tar->justment = tar->justment - 1;
            }
            else
            {
                tar->justment = tar->justment + 1;
            }
            tar->next = AddedCar->next;
            PutAsc(tar->x + 150, tar->y, "F tran", WHITE, 2, 2);
            if (AddedCar)
                free(AddedCar);
        }
        break;
    case 112:
    case 114:
    case 142:
    case 144:
        if (tar->count == 0)
        {
            AddedCar->x = tar->x;
            AddedCar->y = tar->y - 25;
        }
        if (tar->count >= 0 && tar->count < 4)
        {
            tar->y = tar->y - 5;
            tar->count++;
        }
        else if (tar->count == 4)
        {
            tar->count = 0;
            tar->flag = 0;
            tar->y = tar->y - 4;
            if (tar->justment % 2 == 0)
            {
                tar->justment = tar->justment - 1;
            }
            else
            {
                tar->justment = tar->justment + 1;
            }
            tar->next = AddedCar->next;
            PutAsc(tar->x + 150, tar->y, "F tran", WHITE, 2, 2);
            if (AddedCar)
                free(AddedCar);
        }
        break;

    case 211:
    case 213:
    case 231:
    case 233:
        if (tar->count == 0)
        {
            AddedCar->x = tar->x + 25;
            AddedCar->y = tar->y;
        }
        if (tar->count >= 0 && tar->count < 4)
        {
            tar->x = tar->x + 5;
            tar->count++;
        }
        else if (tar->count == 4)
        {
            tar->count = 0;
            tar->flag = 0;
            tar->x = tar->x + 4;
            if (tar->justment % 2 == 0)
            {
                tar->justment = tar->justment - 1;
            }
            else
            {
                tar->justment = tar->justment + 1;
            }
            tar->next = AddedCar->next;
            PutAsc(tar->x + 150, tar->y, "F tran", WHITE, 2, 2);
            if (AddedCar)
                free(AddedCar);
        }
        break;
    case 212:
    case 214:
    case 232:
    case 234:

        if (tar->count == 0)
        {
            AddedCar->x = tar->x - 25;
            AddedCar->y = tar->y;
        }
        if (tar->count >= 0 && tar->count < 4)
        {
            tar->x = tar->x - 5;
            tar->count++;
        }
        else if (tar->count == 4)
        {
            tar->count = 0;
            tar->flag = 0;
            tar->x = tar->x - 4;
            if (tar->justment % 2 == 0)
            {
                tar->justment = tar->justment - 1;
            }
            else
            {
                tar->justment = tar->justment + 1;
            }
            tar->next = AddedCar->next;
            PutAsc(tar->x + 150, tar->y, "F tran", WHITE, 2, 2);
            if (AddedCar)
                free(AddedCar);
        }
        break;
    default:
        tar->flag = 0;
        PutAsc(tar->x + 150, tar->y, "bad tran", WHITE, 2, 2);
        break;
    }
}