/*****创建时间： 2019.9.22********************
 *  
 * ***作者：董国庆
 * ***功能：创建车链表，车位置计算，红绿灯管理
*********************************************/

#include <stdio.h>
#include "mouse.h"
#include "draw.h"
#include "hanzi.h"
#include <time.h>
#include <stdlib.h>
#include <dos.h>

void CreatCarList(CAR **head)
{
    CAR **current = head;
    *current = (CAR *)malloc(sizeof(CAR));
}

void CreatCar(CAR **head)
{
    int j, temp = (unsigned)time(NULL);
    srand(temp * 6482);
    temp = rand();
    j = temp % 2;
    j <<= 2;
    j = temp % 16;
}

void CarDispatch(CAR *car)
{
    static int origin = clock(), dt, v;
    CAR *current, *pre, *temp;
    dt = clock() - origin;
    DrawRoad();
    for (pre = car, current = pre->next; current != NULL; current = current->next)
    {
        if (current->x > 1024 || current->x < 0 || current->y < 0 || current->y > 768)
        {
            
            pre->next = current->next;
            free(current);
        }
        else
        {
            v = current->speed << 4 >> 4;  //读取速度大小，存贮在speed的后四位，故先<<4将前四位去掉
            switch ((current->speed) >> 4) //根据车的方向判断是上下还是左右方向行驶
            {
            case 0:
                current->y += dt * v;
                break;
            case 1:
                current->x += dt * v;
                break;
            default:
                put_asc(current->x, current->y, "bad car", RED, 2, 2);
                break;
            }
            DrawCar(current);
        }
    }
}

void LightDispatch()
{
}