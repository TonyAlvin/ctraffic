/*****创建时间： 2019.9.22********************
 *  
 * ***作者：董国庆
 * ***功能：创建车链表，车位置计算
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
#include "light.h"

#include <string.h> //测试bug用

void SolveAlarm(CAR *p);                                //处理警报函数
void MoveCar(CAR *p);                                   //直线移动车函数
void CarSingle(CAR *car, CAR *p, CAR *prep, CAR *turn); //控制非路口内单个车还能输
int ChangeJustment(CAR *p);                             //通过路口后更改车辆justment函数
void PreScan(CAR *car, CAR *tar);                       //扫描前方车辆函数
void TurnCar(CAR *car, CAR *p, CAR *prep);              //控制路口内车辆函数
void TurnLeftCar(CAR *p);                               //控制车辆左转
void TurnRightCar(CAR *p);                              //控制车辆右转
void TurnStrightCar(CAR *p);                            //控制车辆直行
int TurnPreScan(CAR *car, CAR *p);                      //转弯前预扫描函数，用于判断前方路口是否压车至本路口
int GetLightStatusC(int just, int turn, int x, int y);  //获取十字路口当前车辆是否通行
int GetLightStatusT(int just, int turn, int x, int y);  //获取丁字路口当前车辆是否通行
void ExScan(CAR *car, int just);

//随机数发生函数，返回一个从min到max的随机数
int RandInt(int min, int max)
{
    static unsigned int temp = 0;
    if (temp == 0)
        temp = (unsigned)time(NULL);
    srand(temp);
    temp *= 234;
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
    newcar->turn[0] = RandInt(1,3); //生成转向信息
    newcar->turn[1] = RandInt(1,3);
    newcar->turn[2] = RandInt(1,3);
    newcar->turn[3] = RandInt(1,3);
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
        newcar->y = 1;
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
    newcar->std_speed = RandInt(1, 16); //生成初始速度
    newcar->speed = newcar->std_speed;  //当前速度=初始速度
    newcar->color = RandInt(2, 8);      //车颜色随机
    newcar->alarm = 0;                  //与前车距离初始化为零
    newcar->flag = 0;                   //驶出屏幕标记，初始化为零
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

/*******************************************
 * 函数功能：车辆整体调度
 * 参数： 车辆链表头
 * 返回值：无
 * *****************************************/
void CarListDispatch(CAR *car, CAR *turn)
{
    CAR *road, *current, *precurrent;
    road = car;
    NormalControl(2);
    precurrent = road;
    for (current = precurrent->next; current != NULL; current = precurrent->next)
    {
        CarSingle(car, current, precurrent, turn);
        precurrent = precurrent->next;
    }
    precurrent = turn;
    for (current = precurrent->next; current != NULL; current = precurrent->next)
    {
        TurnCar(car, current, precurrent);
        precurrent = precurrent->next;
    }
}

/**********************************************
 * 函数名：int JudgeCross(CAR *p)
 * 参数：               当前车指针
 * 返回值：判断在进入路口范围内，0为不在路口
***********************************************/
int JudgeCross(CAR *p)
{
    if ((p->x >= LEFT_X && p->x <= LEFT_X + 50 && p->y >= UP_Y + 40 && p->y <= UP_Y + 70) ||
        (p->x >= LEFT_X - 50 && p->x <= LEFT_X && p->y >= UP_Y - 70 && p->y <= UP_Y - 40) ||
        (p->x >= LEFT_X - 70 && p->x <= LEFT_X - 40 && p->y >= UP_Y && p->y <= UP_Y + 50) ||
        (p->x >= LEFT_X + 40 && p->x <= LEFT_X + 70 && p->y >= UP_Y - 50 && p->y <= UP_Y))
        return 1;
    else if ((p->x >= LEFT_X && p->x <= LEFT_X + 50 && p->y >= DOWN_Y + 40 && p->y <= DOWN_Y + 70) ||
             (p->x >= LEFT_X - 50 && p->x <= LEFT_X && p->y >= DOWN_Y - 70 && p->y <= DOWN_Y - 40) ||
             (p->x >= LEFT_X - 70 && p->x <= LEFT_X - 40 && p->y >= DOWN_Y && p->y <= DOWN_Y + 50) ||
             (p->x >= LEFT_X + 40 && p->x <= LEFT_X + 70 && p->y >= DOWN_Y - 50 && p->y <= DOWN_Y))
        return 2;
    else if ((p->x >= RIGHT_X && p->x <= RIGHT_X + 50 && p->y >= DOWN_Y + 40 && p->y <= DOWN_Y + 70) ||
             (p->x >= RIGHT_X - 50 && p->x <= RIGHT_X && p->y >= DOWN_Y - 70 && p->y <= DOWN_Y - 40) ||
             (p->x >= RIGHT_X - 70 && p->x <= RIGHT_X - 40 && p->y >= DOWN_Y && p->y <= DOWN_Y + 50) ||
             (p->x >= RIGHT_X + 40 && p->x <= RIGHT_X + 70 && p->y >= DOWN_Y - 50 && p->y <= DOWN_Y))
        return 3;
    else if ((p->x >= RIGHT_X && p->x <= RIGHT_X + 50 && p->y >= UP_Y + 40 && p->y <= UP_Y + 70) ||
             (p->x >= RIGHT_X - 50 && p->x <= RIGHT_X && p->y >= UP_Y - 70 && p->y <= UP_Y - 40) ||
             (p->x >= RIGHT_X - 70 && p->x <= RIGHT_X - 40 && p->y >= UP_Y && p->y <= UP_Y + 50) ||
             (p->x >= RIGHT_X + 40 && p->x <= RIGHT_X + 70 && p->y >= UP_Y - 50 && p->y <= UP_Y))
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
    int place = JudgeCross(p);
    switch (place)
    {
    //非路口只需判断前方有无车辆即可行驶
    case 0:
        PreScan(car, p);
        SolveAlarm(p);
        MoveCar(p);
        break;
    //十字路口统一处理
    case 1:
    case 2:
    case 3: //
        if ((GetLightStatusC(p->justment, p->turn[place - 1], (p->x < 500) ? 250 : 750, (p->y < 350) ? 170 : 570)) && !TurnPreScan(car, p))
        { //绿灯行
            PreScan(turn, p);
            SolveAlarm(p);
            MoveCar(p);
            prep->next = p->next; //将车移入路口链表
            p->next = turn->next;
            turn->next = p;
            PutAsc(p->x, p->y, "IN", RED, 2, 2);
            return; //将车移出后单车在turn car函数内处理
        }
        break; //红灯停，不对车的位置及所在链表进行操作
    //丁字路口单独处理
    case 4:
        if ((GetLightStatusT(p->justment, p->turn[place - 1], 750, 170)) && !TurnPreScan(car, p))
        { //绿灯行
            PreScan(turn, p);
            SolveAlarm(p);
            MoveCar(p);
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
    // itoa(p->turn[JudgeCross(p) - 1], a, 10);
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
    case 1: //直行
        if (just / 100 == 1)
            flag = getpixel(x - 58, y - 38); //水平方向
        else
            flag = getpixel(x + 38, y - 58); //竖直方向
        return (flag == GREEN);
    case 2: //左转
        if (just / 100 == 1)
            flag = getpixel(x - 58, y - 12); //水平方向车左转
        else
            flag = getpixel(x - 12, y + 58); //竖直方向左转
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
    case 1: //直行
        flag = getpixel(x - 58, y - 38);
        return (flag == GREEN);
    case 2: //左转
        if (just / 100 == 1)
            flag = getpixel(x - 58, y - 12); //竖直方向左转
        else
            flag = getpixel(x + 24, y - 58); //水平方向车左转
        return (flag == GREEN);
    case 3: //右转 //右转一直绿灯
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

void TurnCar(CAR *car, CAR *p, CAR *prep)
{
    char a[5]; //调bug专用
    switch (p->turn[JudgeInCross(p, 20) - 1])
    {
    case 1: //直行
        TurnStrightCar(p);
        break;
    case 2: //左转
        TurnLeftCar(p);
        break;
    case 3: //右转
        TurnRightCar(p);
        break;
    default: //错误处理
        // p->justment == ChangeJustment(p);
        PutAsc(p->x, p->y, "bad turn", RED, 1, 1);
        break;
    }
    DrawCar(p);
    if (p->count == 0) //判断车辆驶出路口，则移出路口链表，移入正常链表
    {
        MoveCar(p);
        itoa(p->justment, a, 10);
        PutAsc(p->x + 50, p->y + 32, a, WHITE, 2, 2);
        p->justment = ChangeJustment(p);
        prep->next = p->next;
        p->next = car->next;
        car->next = p;
        PutAsc(p->x + 50, p->y, "OUT", WHITE, 2, 2);
        itoa(p->justment, a, 10); //调试bug要用
        PutAsc(p->x + 150, p->y, a, WHITE, 2, 2);
    }
}

void TurnStrightCar(CAR *p)
{
    MoveCar(p);
    p->count++;
    if (!JudgeInCross(p, 20))
        p->count = 0;
}

void TurnLeftCar(CAR *car)
{
    if (car->alarm == 0)
    {
        if (JudgeInCross(car, 2) && car->count < 100)
        {
            car->angle += 9;
            car->x = car->x - (int)(11.2 * sin(car->angle * PI / 180));
            car->y = car->y - (int)(11.2 * cos(car->angle * PI / 180));
            car->count += 9;
        }
        else
            MoveCar(car);
        car->count++;
    }
    if (car->angle >= 360)
        car->angle -= 360;
    else if (car->angle < 0)
        car->angle += 360;
    if (!JudgeInCross(car, 20))
        car->count = 0;
}

void TurnRightCar(CAR *car)
{
    if (car->alarm == 0)
    {
        if (JudgeInCross(car, 2) && car->count < 90)
        {
            car->angle -= 15;
            car->x = car->x - (int)(3 * sin(car->angle * PI / 180));
            car->y = car->y - (int)(3 * cos(car->angle * PI / 180));
            car->count += 15;
        }
        else
            MoveCar(car);
        car->count++;
    }
    if (car->angle >= 360)
        car->angle -= 360;
    else if (car->angle < 0)
        car->angle += 360;
    if (!JudgeInCross(car, 20))
        car->count = 0;
}

int TurnPreScan(CAR *car, CAR *p) //转弯前直道预扫描函数（共用）
{
    CAR *current;
    int pre = ChangeJustment(p), position = JudgeCross(p);
    char a[5]; //调bug专用
    itoa(p->justment, a, 10);
    PutAsc(p->x + 300, p->y, a, WHITE, 2, 2);
    for (current = car->next; current != NULL; current = current->next)
        if ((current->justment == pre) && (JudgeCross(current) == position))
            return 1;
    return 0;
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