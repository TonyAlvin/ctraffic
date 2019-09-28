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
#include "mouse.h"
#include "draw.h"
#include "hanzi.h"
#include <string.h>
#include "all.h"
#include "dispatch.h"

int RandInt(void)
{
    int temp = (unsigned)time(NULL);
    srand(temp * 6482);
    temp = rand();
    return temp;
}

void InitCar(CAR *newcar)
{
    char s[10];
    int j, temp;
    temp = RandInt();
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
    }
    (*current)->next = NULL;
}

void CarDispatch(CAR *car)
{
    // char s[10];
    static int dt, origin = 0;
    int v;
    CAR *current, *pre, *temp;
    if (origin == 0)
        origin = clock();
    dt = clock() - origin;
    DrawRoad();
    for (pre = car, current = pre->next; current != NULL; current = current->next)
    {
        // itoa(current->speed >> 4, s, 10);
        // PutAsc(20, 220, s, RED, 2, 2);
        if (current->x > 1024 || current->x < 0 || current->y < 0 || current->y > 768)
        {
            pre->next = current->next;
            free(current);
        }
        else
        {
            switch ((int)current->angle / 90) //根据车的方向判断是上下还是左右方向行驶
            {
            case 0:
            case 2:
                current->y += v / 150;
                break;
            case 1:
            case 3:
                current->x += v / 150;
                break;
            default:
                PutAsc(current->x, current->y, "bad car", RED, 2, 2);
                break;
            }
            DrawCar(current);
        }
    }
}

void GetTime(void)
{
}