/*****创建时间： 2019.9.18********************
 *  
 * ***作者：陈兆朗
 * ***功能：红绿灯常时控制，绘制十字路口丁字路口红绿灯
 * ***最后修改时间：2019.10.2
*********************************************/
#include <graphics.h>
#include <time.h>
#include <stdlib.h>
#include "draw.h"
#include "all.h"

/***********************************************
 * ***函数名void DrawLightC(int x, int y, int n)
 * ***参数                路口中心坐标  红绿灯状态
 * ***返回值 void  ***函数功能：画十字路口红绿灯
 * ***函数说明：n为1水平左转，2竖直直行，3竖直左转，4水平直行
 * *********************************************/
void DrawLightC(int x, int y, int n)
{
    switch (n % 4 + 1)
    {
    case 1:
        DrawTrafficLight(x + 12, y - 58, 3, RED);
        DrawTrafficLight(x + 38, y - 58, 1, RED);
        DrawTrafficLight(x - 58, y - 38, 3, RED);
        DrawTrafficLight(x - 58, y - 12, 2, GREEN);
        DrawTrafficLight(x + 58, y + 12, 1, GREEN);
        DrawTrafficLight(x + 58, y + 38, 4, RED);
        DrawTrafficLight(x - 38, y + 58, 2, RED);
        DrawTrafficLight(x - 12, y + 58, 4, RED);
        break;
    case 2:
        DrawTrafficLight(x + 12, y - 58, 3, RED);
        DrawTrafficLight(x + 38, y - 58, 1, GREEN);
        DrawTrafficLight(x - 58, y - 38, 3, RED);
        DrawTrafficLight(x - 58, y - 12, 2, RED);
        DrawTrafficLight(x + 58, y + 12, 1, RED);
        DrawTrafficLight(x + 58, y + 38, 4, RED);
        DrawTrafficLight(x - 38, y + 58, 2, GREEN);
        DrawTrafficLight(x - 12, y + 58, 4, RED);
        break;
    case 3:
        DrawTrafficLight(x + 12, y - 58, 3, GREEN);
        DrawTrafficLight(x + 38, y - 58, 1, RED);
        DrawTrafficLight(x - 58, y - 38, 3, RED);
        DrawTrafficLight(x - 58, y - 12, 2, RED);
        DrawTrafficLight(x + 58, y + 12, 1, RED);
        DrawTrafficLight(x + 58, y + 38, 4, RED);
        DrawTrafficLight(x - 38, y + 58, 2, RED);
        DrawTrafficLight(x - 12, y + 58, 4, GREEN);
        break;
    case 4:
        DrawTrafficLight(x + 12, y - 58, 3, RED);
        DrawTrafficLight(x + 38, y - 58, 1, RED);
        DrawTrafficLight(x - 58, y - 38, 3, GREEN);
        DrawTrafficLight(x - 58, y - 12, 2, RED);
        DrawTrafficLight(x + 58, y + 12, 1, RED);
        DrawTrafficLight(x + 58, y + 38, 4, GREEN);
        DrawTrafficLight(x - 38, y + 58, 2, RED);
        DrawTrafficLight(x - 12, y + 58, 4, RED);
        break;
    }
}

/***********************************************
 * ***函数名void DrawLightT(int x, int y, int n)
 * ***参数                路口中心坐标  红绿灯状态
 * ***返回值 void  ***函数功能：画丁字路口红绿灯
 * ***函数说明：n为1水平左转，2竖直左转，3水平直行
 * *********************************************/
void DrawLightT(int x, int y, int n)
{
    switch (n % 3 + 1)
    {
    case 1:
        DrawTrafficLight(x + 24, y - 58, 3, RED);
        DrawTrafficLight(x - 58, y - 38, 3, RED);
        DrawTrafficLight(x - 58, y - 12, 2, GREEN);
        DrawTrafficLight(x + 58, y + 24, 4, RED);
        break;
    case 2:
        DrawTrafficLight(x + 24, y - 58, 3, GREEN);
        DrawTrafficLight(x - 58, y - 38, 3, RED);
        DrawTrafficLight(x - 58, y - 12, 2, RED);
        DrawTrafficLight(x + 58, y + 24, 4, RED);
        break;
    case 3:
        DrawTrafficLight(x + 24, y - 58, 3, RED);
        DrawTrafficLight(x - 58, y - 38, 3, GREEN);
        DrawTrafficLight(x - 58, y - 12, 2, RED);
        DrawTrafficLight(x + 58, y + 24, 4, GREEN);
        break;
    }
}

/***********************************************
 * ***函数名void NormalControl(int n)
 * ***参数                每种灯的时间长度
 * ***返回值 void
 * ***函数功能：红绿灯的常时控制
 * *********************************************/
void NormalControl(int t)
{
    static unsigned int origin = 0;
    unsigned int interval;
    if (origin == 0)
        origin = (unsigned int)time(NULL);
    interval = (unsigned int)time(NULL) - origin;
    DrawLightC(LEFT_X, UP_Y, interval / t);
    DrawLightT(RIGHT_X, UP_Y, (interval + 1) / t);
    DrawLightC(LEFT_X, DOWN_Y, (interval + 0) / t);
    DrawLightC(RIGHT_X, DOWN_Y, (interval + 3) / t);
}