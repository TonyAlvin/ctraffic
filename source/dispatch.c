/*****创建时间： 2019.9.22********************
 *  
 * ***作者：董国庆
 * ***功能：创建车链表，车位置计算，红绿灯管理
*********************************************/
#include <stdio.h>
#include <graphics.h>
#include <time.h>
#include <stdlib.h>
#include <dos.h>
#include <math.h>
#include "mouse.h"
#include "draw.h"
#include "hanzi.h"
#include <string.h>
#include "all.h"
#include "dispatch.h"

void SolveAlarm(CAR *p);
void MoveCar(CAR *p);
void CarSingle(CAR *car, CAR *p);
void StrightChangeJustment(CAR *p);
void LeftChangeJustment(CAR *p);
void RightChangeJustment(CAR *p);
void PreScan(CAR *car, CAR *tar);
void TurnCar(CAR *p);
void TurnLeftCar(CAR *p);
void TurnRightCar(CAR *p);
void GoStrightCar(CAR *p);

int RandInt(int min, int max)
{
    srand((unsigned)time(NULL));
    return rand() % (max - min + 1) + min;
}

/*********************************************
 * 函数功能：初始化车辆；
 * 参数：newcar:车辆指针，dirt:路名1U、2D、3L、4R
 * 返回值：void
 * ******************************************/
void InitCar(CAR *newcar)
{
    int RoadR[] = {UU1, UU2, DU1, DU2}, //存储路的坐标信息，方便随机生成
        RoadL[] = {UD1, UD2, DD1, DD2},
        RoadU[] = {LL1, LL2},
        RoadD[] = {LR1, LR2, RR1, RR2},
        ang;
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
            break;
        case LR2:
            newcar->justment = 224;
            break;
        case RR1:
            newcar->justment = 243;
            break;
        case RR2:
            newcar->justment = 244;
            break;
        default:
            PutAsc(500, 300, "Init0 Car Error", RED, 2, 2);
        }
        break;
    case 1:
        newcar->x = 1;
        newcar->y = RoadL[RandInt(0, 3)];
        switch (newcar->y)
        {
        case UD1:
            newcar->justment = 103;
            break;
        case UD2:
            newcar->justment = 104;
            break;
        case DD1:
            newcar->justment = 133;
            break;
        case DD2:
            newcar->justment = 134;
            break;
        default:
            PutAsc(500, 300, "Init3 Car Error", RED, 2, 2);
        }
        break;
    case 2:
        newcar->x = RoadU[RandInt(0, 1)];
        newcar->y = 1;
        switch (newcar->x)
        {
        case LL1:
            newcar->justment = 201;
            break;
        case LL2:
            newcar->justment = 202;
            break;
        default:
            PutAsc(500, 300, "Init2 Car Error", RED, 2, 2);
        }
        break;
    case 3:
        newcar->x = 1023;
        newcar->y = RoadR[RandInt(0, 3)];
        switch (newcar->y)
        {
        case UU1:
            newcar->justment = 121;
            break;
        case UU2:
            newcar->justment = 122;
            break;
        case DU1:
            newcar->justment = 151;
            break;
        case DU2:
            newcar->justment = 152;
            break;
        default:
            PutAsc(500, 300, "Init1 Car Error", RED, 2, 2);
        }
        break;
    default:
        PutAsc(500, 300, "Init Car Error", RED, 2, 2);
    }
    newcar->turn1 = RandInt(1, 3); //生成转向信息
    newcar->turn2 = RandInt(1, 3);
    newcar->turn3 = RandInt(1, 3);
    newcar->turn4 = RandInt(1, 3);
    newcar->std_speed = RandInt(1, 16); //生成初始速度
    newcar->speed = newcar->std_speed;  //当前速度=初始速度
    newcar->color = RandInt(2,8);                //车颜色随机
    newcar->alarm = 0;                  //与前车距离初始化为零
    newcar->flag = 0;                   //驶出屏幕标记，初始化为零
    // DrawCar(newcar);
}

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

/*******************************************
 * 函数功能：车辆整体调度
 * 参数： 车辆链表头
 * 返回值：无
 * *****************************************/
void CarListDispatch(CAR *car, CAR *turn)
{
    CAR *road, *current;
    road = car;
    for (current = road->next; current != NULL; current = current->next)
        CarSingle(car, current);
    road = turn;
    for (current = road->next; current != NULL; current = current->next)
        TurnCar(current);
}

int JudgeCross(CAR *p)
{
    if (p->x > LEFT_X - 80 && p->x < LEFT_X + 80 && p->y > UP_Y - 80 && p->y < UP_Y + 80)
        return 1;
    else if (p->x > LEFT_X - 80 && p->x < LEFT_X + 80 && p->y > DOWN_Y - 80 && p->y < DOWN_Y + 80)
        return 2;
    else if (p->x > RIGHT_X - 80 && p->x < RIGHT_X + 80 && p->y > DOWN_Y - 80 && p->y < DOWN_Y + 80)
        return 3;
    else if (p->x > RIGHT_X - 80 && p->x < RIGHT_X + 80 && p->y > UP_Y - 80 && p->y < UP_Y + 80)
        return 4;
    else
        return 0;
}

void CarSingle(CAR *car, CAR *p)
{
    switch (JudgeCross(p))
    {
    case 0: //非路口只需判断前方有无车辆即可行驶
        PreScan(car, p);
        SolveAlarm(p);
        MoveCar(p);
        break;
    case 1:
    case 2:
    case 3: //十字路口统一处理
        PreScan(car, p);
        SolveAlarm(p);
        MoveCar(p);
        break;
    case 4: //丁字路口单独处理
        PreScan(car, p);
        SolveAlarm(p);
        MoveCar(p);
        break;
    default: //车辆justment数据有误
        PutAsc(p->x, p->y, "find place error", RED, 2, 2);
    }
    DrawCar(p);
    setcolor(BLACK);
    bar(300, 400, 400, 450);
    PutAsc(300, 400, "car", RED, 2, 2);
}

int GetLightStatus()
{
    return 0;
}

void TurnCar(CAR *p)
{
}

void TurnLeftCar(CAR *p)
{
}

void TurnRightCar(CAR *p)
{
}

void GoStrightCar(CAR *p)
{
}

//直道扫描函数（共用）
void PreScan(CAR *car, CAR *tar)
{
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
                    tar->alarm = 1;
                else if (((current->x + DIL) <= tar->x) && (tar->alarm != 0))
                    tar->alarm = 0; //将距离梯度化，在最短距离内停车，中距内慢速行驶，长距内以初始化速度行驶
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
                    tar->alarm = 1;
                else if (((current->x - DIL) >= tar->x) && (tar->alarm != 0))
                    tar->alarm = 0; //将距离梯度化，在最短距离内停车，中距内慢速行驶，长距内以初始化速度行驶
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
                if ((current->y > tar->y) && ((current->y + DIS) < tar->y))
                    tar->alarm = 2;
                else if (((current->y + DIS) >= tar->y) && ((current->y + DIL) < tar->y && (tar->alarm != 2)))
                    tar->alarm = 1;
                else if (((current->y + DIL) >= tar->y) && (tar->alarm != 0))
                    tar->alarm = 0; //将距离梯度化，在最短距离内停车，中距内慢速行驶，长距内以初始化速度行驶
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
                if ((current->y < tar->y) && ((current->y - DIS) > tar->y))
                    tar->alarm = 2;
                else if (((current->y - DIS) <= tar->y) && ((current->y + DIL) > tar->y))
                    tar->alarm = 1;
                else if (((current->y + DIL) <= tar->y) && (tar->alarm != 0))
                    tar->alarm = 0; //将距离梯度化，在最短距离内停车，中距内慢速行驶，长距内以初始化速度行驶
                break;
            default:
                PutAsc(tar->x, tar->y, "bad justment", RED, 2, 2);
                break;
            }
    }
}

void RightChangeJustment(CAR *p)
{
    switch (p->justment)
    {
    case 104:
        p->justment = 211;
        break;
    case 111:
        p->justment = 204;
        break;
    case 201:
        p->justment = 101;
        break;
    case 214:
        p->justment = 114;
        break;
    case 211:
        p->justment = 131;
        break;
    case 224:
        p->justment = 144;
        break;
    case 134:
        p->justment = 221;
        break;
    case 141:
        p->justment = 214;
        break;
    case 114:
        p->justment = 231;
        break;
    case 234:
        p->justment = 124;
        break;
    case 244:
        p->justment = 154;
        break;
    case 231:
        p->justment = 141;
        break;
    case 144:
        p->justment = 241;
        break;
    case 151:
        p->justment = 234;
        break;
    default:
        break;
    }
    return;
}

void LeftChangeJustment(CAR *p)
{
    switch (p->justment)
    {
    case 103:
        p->justment = 203;
        break;
    case 112:
        p->justment = 212;
        break;
    case 213:
        p->justment = 102;
        break;
    case 202:
        p->justment = 113;
        break;
    case 133:
        p->justment = 213;
        break;
    case 142:
        p->justment = 222;
        break;
    case 212:
        p->justment = 143;
        break;
    case 223:
        p->justment = 132;
        break;
    case 122:
        p->justment = 232;
        break;
    case 233:
        p->justment = 112;
        break;
    case 243:
        p->justment = 142;
        break;
    case 232:
        p->justment = 153;
        break;
    case 143:
        p->justment = 233;
        break;
    case 152:
        p->justment = 242;
        break;
    default:
        break;
    }
    return;
}

void StrightChangeJustment(CAR *p)
{
    p->justment += ((p->angle - 120 > 0) ? 10 : -10);
    return;
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
        p->x += p->speed;
        break;
    case 2:
        p->y += p->speed;
        break;
    case 3:
        p->x -= p->speed;
        break;
    }
}

//处理前方报警的函数(共用)
void SolveAlarm(CAR *p)
{
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
    default:
        PutAsc(p->x, p->y, "bad car alarm", RED, 2, 2);
        delay(1000);
        break;
    }
}