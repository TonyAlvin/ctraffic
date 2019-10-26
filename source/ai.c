#include <stdlib.h>
#include <graphics.h>
#include <time.h>
#include "all.h"
#include "draw.h"
#include "hanzi.h"
#include "light.h"
#include "ai.h"

typedef struct LaneStruct
{
    int lane;
    int car_number;
} Lane;

int LaneCount(CAR *car, int lane); //车道内车辆计数
int GetLight(int num);             //获取num路口的红绿灯状态

int LaneCount(CAR *car, int lane)
{
    int count = 0;
    CAR *current;
    for (current = car->next; current != NULL; current = current->next)
        if (current->justment == lane)
            count++;
    return count;
}

int GetLight(int num)
{
    int status = 0, x, y;
    if (num == 3) //丁字路口单独判断
    {
        if (getpixel(RIGHT_X - 58, UP_Y - 38) == GREEN)
            status = 21; //水平方直行
        else if (getpixel(RIGHT_X - 58, UP_Y - 12) == GREEN)
            status = 12; //竖直方向左转
        else if (getpixel(RIGHT_X + 24, UP_Y - 58))
            status = 22; //水平方向车左转
    }
    else
    {
        x = (num < 3) ? LEFT_X : RIGHT_X, y = (num < 1) ? UP_Y : DOWN_Y;
        if (getpixel(x - 58, y - 38) == GREEN)
            status = 21; //水平方向直行
        else if (getpixel(x + 38, y - 58))
            status = 11; //竖直方向直行
        else if (getpixel(x - 58, y - 12) == GREEN)
            status = 22; //水平方向车左转
        else if (getpixel(x - 12, y + 58))
            status = 12; //竖直方向左转
    }
    return status;
}
// void intelligence1(CAR *car)
// {
//     int a[4], n, i, max, min;
//     unsigned int interval;
//     static unsigned int origin = 0;
//     a[0] = LaneCount(car, 111) + LaneCount(car, 112);
//     a[1] = LaneCount(car, 103) + LaneCount(car, 104);
//     a[2] = LaneCount(car, 201) + LaneCount(car, 202);
//     a[3] = LaneCount(car, 214) + LaneCount(car, 213);
//     interval = (unsigned int)time(NULL) - origin;
//     max = a[0];
//     min = a[0];
//     for (i = 0; i < 3; i++)
//     {
//         if (a[i] > max)
//         {
//             max = a[i];
//         }
//     }
//     for (i = 0; i < 3; i++)
//     {
//         if (a[i] < min)
//         {
//             min = a[i];
//         }
//     }
//     if (a[0] > a[1] && a[1] > a[2] && a[2] > a[3])
//     {
//         ;
//     }
//     else if (a[0] > a[1] && a[1] > a[3] && a[3] > a[2])
//     {
//         /* code */
//     }
//     else if (a[0] > a[3] && a[3] > a[1] && a[1] > a[2])
//     {
//         /* code */
//     }
//     else if (a[0] > a[3] && a[3] > a[2] && a[2] > a[1])
//     {
//         /* code */
//     }
//     else if (a[1] > a[0] && a[0] > a[2] && a[2] > a[3])
//     {
//         /* code */
//     }
//     else if (/* condition */)
//     {
//         /* code */
//     }
// }
void intelligence(CAR *car, int **p, int n)
{
    Lane a[4];
    int i;
    int total;
    if (n == 0)
    {
        a[0].car_number = LaneCount(car, 214) + LaneCount(car, 213); //用car_number记录一个路口每个方向的车辆
        a[1].car_number = LaneCount(car, 111) + LaneCount(car, 112);
        a[2].car_number = LaneCount(car, 201) + LaneCount(car, 202);
        a[3].car_number = LaneCount(car, 103) + LaneCount(car, 104);
    }
    else if (n == 1)
    {
        a[0].car_number = LaneCount(car, 141) + LaneCount(car, 142);
        a[1].car_number = LaneCount(car, 211) + LaneCount(car, 212);
        a[2].car_number = LaneCount(car, 133) + LaneCount(car, 134);
        a[3].car_number = LaneCount(car, 223) + LaneCount(car, 224);
    }
    else if (n == 2)
    {
        a[0].car_number = LaneCount(car, 231) + LaneCount(car, 232);
        a[1].car_number = LaneCount(car, 143) + LaneCount(car, 144);
        a[2].car_number = LaneCount(car, 243) + LaneCount(car, 244);
        a[3].car_number = LaneCount(car, 151) + LaneCount(car, 152);
    }
    else if (n == 3)
    {
        a[0].car_number = LaneCount(car, 113) + LaneCount(car, 114);
        a[1].car_number = LaneCount(car, 233) + LaneCount(car, 234);
        a[2].car_number = LaneCount(car, 121) + LaneCount(car, 122);
        a[3].car_number = 0;
    }

    a[0].lane = 1;
    a[1].lane = 2;
    a[2].lane = 3;
    a[3].lane = 4;
    total = a[0].car_number + a[1].car_number + a[2].car_number + a[3].car_number; //记录路口附近的总车数，并据此判断红绿灯转换的时间间隔的模式

    if (total >= 15) //车多时，加长路口红绿灯一个周期的时间
    {
        for (i = 0; i < 3; i++)
        {
            if (a[i].car_number > a[i + 1].car_number)
            {
                p[n][i] = 1;
            }
        }
    }
    else //车少时，减少路口红绿灯一个周期的时间
    {
        for (i = 0; i < 3; i++)
        {
            if (a[i + 1].car_number > a[i].car_number)
            {
                p[n][i] = -1;
            }
        }
    }
    if (n == 3) //丁字路口的特殊处理
    {
        p[n][2] = 0;
    }
}

void AI_Dispatch1(int **a, int interval, int n)
{
    int flag;
    flag = interval % (20 + a[n][0] + a[n][1] + a[n][2]);
    if (flag >= 0 && flag < 5 + a[n][0])
    {
        DrawTrafficLight(LEFT_X + 12, UP_Y - 58, 3, GREEN);
        DrawTrafficLight(LEFT_X + 38, UP_Y - 58, 1, GREEN);
        DrawTrafficLight(LEFT_X - 58, UP_Y - 38, 3, RED);
        DrawTrafficLight(LEFT_X - 58, UP_Y - 12, 2, RED);
        DrawTrafficLight(LEFT_X + 58, UP_Y + 12, 1, RED);
        DrawTrafficLight(LEFT_X + 58, UP_Y + 38, 4, RED);
        DrawTrafficLight(LEFT_X - 38, UP_Y + 58, 2, RED);
        DrawTrafficLight(LEFT_X - 12, UP_Y + 58, 4, RED);
    }
    else if (flag >= 5 + a[n][0] && flag < 10 + a[n][1])
    {
        DrawTrafficLight(LEFT_X + 12, UP_Y - 58, 3, RED);
        DrawTrafficLight(LEFT_X + 38, UP_Y - 58, 1, RED);
        DrawTrafficLight(LEFT_X - 58, UP_Y - 38, 3, GREEN);
        DrawTrafficLight(LEFT_X - 58, UP_Y - 12, 2, GREEN);
        DrawTrafficLight(LEFT_X + 58, UP_Y + 12, 1, RED);
        DrawTrafficLight(LEFT_X + 58, UP_Y + 38, 4, RED);
        DrawTrafficLight(LEFT_X - 38, UP_Y + 58, 2, RED);
        DrawTrafficLight(LEFT_X - 12, UP_Y + 58, 4, RED);
    }
    else if (flag >= 10 + a[n][1] && flag < 15 + a[n][2])
    {
        DrawTrafficLight(LEFT_X + 12, UP_Y - 58, 3, RED);
        DrawTrafficLight(LEFT_X + 38, UP_Y - 58, 1, RED);
        DrawTrafficLight(LEFT_X - 58, UP_Y - 38, 3, RED);
        DrawTrafficLight(LEFT_X - 58, UP_Y - 12, 2, RED);
        DrawTrafficLight(LEFT_X + 58, UP_Y + 12, 1, RED);
        DrawTrafficLight(LEFT_X + 58, UP_Y + 38, 4, RED);
        DrawTrafficLight(LEFT_X - 38, UP_Y + 58, 2, GREEN);
        DrawTrafficLight(LEFT_X - 12, UP_Y + 58, 4, GREEN);
    }
    else if (flag >= 15 + a[n][2] && flag < 20 + a[n][0] + a[n][1] + a[n][2])
    {
        DrawTrafficLight(LEFT_X + 12, UP_Y - 58, 3, RED);
        DrawTrafficLight(LEFT_X + 38, UP_Y - 58, 1, RED);
        DrawTrafficLight(LEFT_X - 58, UP_Y - 38, 3, RED);
        DrawTrafficLight(LEFT_X - 58, UP_Y - 12, 2, RED);
        DrawTrafficLight(LEFT_X + 58, UP_Y + 12, 1, GREEN);
        DrawTrafficLight(LEFT_X + 58, UP_Y + 38, 4, GREEN);
        DrawTrafficLight(LEFT_X - 38, UP_Y + 58, 2, RED);
        DrawTrafficLight(LEFT_X - 12, UP_Y + 58, 4, RED);
    }
}

void AI_Dispatch2(int **a, int interval, int n)
{
    int flag;
    flag = interval % (20 + a[n][0] + a[n][1] + a[n][2]);
    if (flag >= 0 && flag < 5 + a[n][0])
    {
        DrawTrafficLight(LEFT_X + 12, DOWN_Y - 58, 3, RED);
        DrawTrafficLight(LEFT_X + 38, DOWN_Y - 58, 1, RED);
        DrawTrafficLight(LEFT_X - 58, DOWN_Y - 38, 3, GREEN);
        DrawTrafficLight(LEFT_X - 58, DOWN_Y - 12, 2, GREEN);
        DrawTrafficLight(LEFT_X + 58, DOWN_Y + 12, 1, RED);
        DrawTrafficLight(LEFT_X + 58, DOWN_Y + 38, 4, RED);
        DrawTrafficLight(LEFT_X - 38, DOWN_Y + 58, 2, RED);
        DrawTrafficLight(LEFT_X - 12, DOWN_Y + 58, 4, RED);
    }
    else if (flag >= 5 + a[n][0] && flag < 10 + a[n][1])
    {
        DrawTrafficLight(LEFT_X + 12, DOWN_Y - 58, 3, RED);
        DrawTrafficLight(LEFT_X + 38, DOWN_Y - 58, 1, RED);
        DrawTrafficLight(LEFT_X - 58, DOWN_Y - 38, 3, RED);
        DrawTrafficLight(LEFT_X - 58, DOWN_Y - 12, 2, RED);
        DrawTrafficLight(LEFT_X + 58, DOWN_Y + 12, 1, RED);
        DrawTrafficLight(LEFT_X + 58, DOWN_Y + 38, 4, RED);
        DrawTrafficLight(LEFT_X - 38, DOWN_Y + 58, 2, GREEN);
        DrawTrafficLight(LEFT_X - 12, DOWN_Y + 58, 4, GREEN);
    }
    else if (flag >= 10 + a[n][1] && flag < 15 + a[n][2])
    {
        DrawTrafficLight(LEFT_X + 12, DOWN_Y - 58, 3, RED);
        DrawTrafficLight(LEFT_X + 38, DOWN_Y - 58, 1, RED);
        DrawTrafficLight(LEFT_X - 58, DOWN_Y - 38, 3, RED);
        DrawTrafficLight(LEFT_X - 58, DOWN_Y - 12, 2, RED);
        DrawTrafficLight(LEFT_X + 58, DOWN_Y + 12, 1, GREEN);
        DrawTrafficLight(LEFT_X + 58, DOWN_Y + 38, 4, GREEN);
        DrawTrafficLight(LEFT_X - 38, DOWN_Y + 58, 2, RED);
        DrawTrafficLight(LEFT_X - 12, DOWN_Y + 58, 4, RED);
    }
    else if (flag >= 15 + a[n][2] && flag < 20 + a[n][0] + a[n][1] + a[n][2])
    {
        DrawTrafficLight(LEFT_X + 12, DOWN_Y - 58, 3, GREEN);
        DrawTrafficLight(LEFT_X + 38, DOWN_Y - 58, 1, GREEN);
        DrawTrafficLight(LEFT_X - 58, DOWN_Y - 38, 3, RED);
        DrawTrafficLight(LEFT_X - 58, DOWN_Y - 12, 2, RED);
        DrawTrafficLight(LEFT_X + 58, DOWN_Y + 12, 1, RED);
        DrawTrafficLight(LEFT_X + 58, DOWN_Y + 38, 4, RED);
        DrawTrafficLight(LEFT_X - 38, DOWN_Y + 58, 2, RED);
        DrawTrafficLight(LEFT_X - 12, DOWN_Y + 58, 4, RED);
    }
}

void AI_Dispatch3(int **a, int interval, int n)
{
    int flag;
    flag = interval % (20 + a[n][0] + a[n][1] + a[n][2]);
    if (flag >= 0 && flag < 5 + a[n][0])
    {
        DrawTrafficLight(RIGHT_X + 12, DOWN_Y - 58, 3, RED);
        DrawTrafficLight(RIGHT_X + 38, DOWN_Y - 58, 1, RED);
        DrawTrafficLight(RIGHT_X - 58, DOWN_Y - 38, 3, RED);
        DrawTrafficLight(RIGHT_X - 58, DOWN_Y - 12, 2, RED);
        DrawTrafficLight(RIGHT_X + 58, DOWN_Y + 12, 1, RED);
        DrawTrafficLight(RIGHT_X + 58, DOWN_Y + 38, 4, RED);
        DrawTrafficLight(RIGHT_X - 38, DOWN_Y + 58, 2, GREEN);
        DrawTrafficLight(RIGHT_X - 12, DOWN_Y + 58, 4, GREEN);
    }
    else if (flag >= 5 + a[n][0] && flag < 10 + a[n][1])
    {
        DrawTrafficLight(RIGHT_X + 12, DOWN_Y - 58, 3, RED);
        DrawTrafficLight(RIGHT_X + 38, DOWN_Y - 58, 1, RED);
        DrawTrafficLight(RIGHT_X - 58, DOWN_Y - 38, 3, RED);
        DrawTrafficLight(RIGHT_X - 58, DOWN_Y - 12, 2, RED);
        DrawTrafficLight(RIGHT_X + 58, DOWN_Y + 12, 1, GREEN);
        DrawTrafficLight(RIGHT_X + 58, DOWN_Y + 38, 4, GREEN);
        DrawTrafficLight(RIGHT_X - 38, DOWN_Y + 58, 2, RED);
        DrawTrafficLight(RIGHT_X - 12, DOWN_Y + 58, 4, RED);
    }
    else if (flag >= 10 + a[n][1] && flag < 15 + a[n][2])
    {
        DrawTrafficLight(RIGHT_X + 12, DOWN_Y - 58, 3, GREEN);
        DrawTrafficLight(RIGHT_X + 38, DOWN_Y - 58, 1, GREEN);
        DrawTrafficLight(RIGHT_X - 58, DOWN_Y - 38, 3, RED);
        DrawTrafficLight(RIGHT_X - 58, DOWN_Y - 12, 2, RED);
        DrawTrafficLight(RIGHT_X + 58, DOWN_Y + 12, 1, RED);
        DrawTrafficLight(RIGHT_X + 58, DOWN_Y + 38, 4, RED);
        DrawTrafficLight(RIGHT_X - 38, DOWN_Y + 58, 2, RED);
        DrawTrafficLight(RIGHT_X - 12, DOWN_Y + 58, 4, RED);
    }
    else if (flag >= 15 + a[n][2] && flag < 20 + a[n][0] + a[n][1] + a[n][2])
    {
        DrawTrafficLight(RIGHT_X + 12, DOWN_Y - 58, 3, RED);
        DrawTrafficLight(RIGHT_X + 38, DOWN_Y - 58, 1, RED);
        DrawTrafficLight(RIGHT_X - 58, DOWN_Y - 38, 3, GREEN);
        DrawTrafficLight(RIGHT_X - 58, DOWN_Y - 12, 2, GREEN);
        DrawTrafficLight(RIGHT_X + 58, DOWN_Y + 12, 1, RED);
        DrawTrafficLight(RIGHT_X + 58, DOWN_Y + 38, 4, RED);
        DrawTrafficLight(RIGHT_X - 38, DOWN_Y + 58, 2, RED);
        DrawTrafficLight(RIGHT_X - 12, DOWN_Y + 58, 4, RED);
    }
}
void AI_Dispatch4(int **a, int interval, int n)
{
    int flag;
    flag = interval % (15 + a[n][2]);
    if (flag >= 0 && flag < 5 + a[n][0])
    {
        DrawTrafficLight(RIGHT_X + 24, UP_Y - 58, 3, RED);
        DrawTrafficLight(RIGHT_X - 58, UP_Y - 38, 3, RED);
        DrawTrafficLight(RIGHT_X - 58, UP_Y - 12, 2, RED);
        DrawTrafficLight(RIGHT_X + 58, UP_Y + 24, 4, GREEN);
    }
    else if (flag >= 5 + a[n][0] && flag < 10 + a[n][1])
    {
        DrawTrafficLight(RIGHT_X + 24, UP_Y - 58, 3, GREEN);
        DrawTrafficLight(RIGHT_X - 58, UP_Y - 38, 3, RED);
        DrawTrafficLight(RIGHT_X - 58, UP_Y - 12, 2, RED);
        DrawTrafficLight(RIGHT_X + 58, UP_Y + 24, 4, RED);
    }
    else if (flag >= 10 + a[n][1] && flag < 15 + a[n][2])
    {
        DrawTrafficLight(RIGHT_X + 24, UP_Y - 58, 3, RED);
        DrawTrafficLight(RIGHT_X - 58, UP_Y - 38, 3, GREEN);
        DrawTrafficLight(RIGHT_X - 58, UP_Y - 12, 2, GREEN);
        DrawTrafficLight(RIGHT_X + 58, UP_Y + 24, 4, RED);
    }
}

void AI_Dispatch(CAR *car)
{
    static int a[4][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}; //静态变量，记录红绿灯转换的时间间隔的变化量
    static unsigned int origin = 0;                                    //静态变量，记录原始时间
    int total[4];
    int i, max;
    unsigned int interval;
    if (origin == 0)
        origin = (unsigned int)time(NULL);
    interval = (unsigned int)time(NULL) - origin;
    for (i = 0; i < 4; i++)
    {
        total[i] = a[i][0] + a[i][1] + a[i][2];
    }
    max = total[0]; //取四组数据中，红绿灯时间间隔变化最大者作为调用intellige的依据
    for (i = 0; i < 4; i++)
    {
        if (total[i] > max)
        {
            max = total[i];
        }
    }

    if (interval % (20 + max) == 0) //每隔（20+max）秒调用一次调整红绿灯转换的时间间隔的函数
    {
        intelligence(car, a, 0);
        intelligence(car, a, 1);
        intelligence(car, a, 2);
        intelligence(car, a, 3);
    }
    AI_Dispatch1(a, interval, 0); //调度红绿灯：1：左上角口
    AI_Dispatch2(a, interval, 1); //           2：左下角口
    AI_Dispatch3(a, interval, 2); //            3：右下角路口
    AI_Dispatch4(a, interval, 3); //            4：丁字路口
}