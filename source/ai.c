#include <stdlib.h>
#include <graphics.h>
#include "ai.h"
#include "all.h"
#include "hanzi.h"
#include "light.h"

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