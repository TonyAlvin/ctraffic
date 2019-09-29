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
    static int i = 1;
    int temp = (unsigned)time(NULL);
    if (i == 1)
        i = temp;
    srand(i * 648);
    i *= 234;
    temp = rand();
    return temp;
}
<<<<<<< HEAD
111
=======
222
>>>>>>> alvin
void InitCar(CAR *newcar)
{
    // char s[10];
    int j, temp, RoadL[] = {UD1, UD2, DD1, DD2},
                 RoadR[] = {UU1, UU2, DU1, DU2},
                 RoadU[] = {LL1, LL2},
                 RoadD[] = {LR1, LR2, RR1, RR2};
    temp = RandInt();
    j = RandInt();
    j *= 234;
    newcar->angle = (temp % 4) * 90;
    switch ((int)(newcar->angle) / 90)
    {
    case 0:
        j %= 4;
        newcar->x = RoadD[j];
        newcar->y = 767;
        break;
    case 1:
        j %= 4;
        newcar->x = 1023;
        newcar->y = RoadL[j];
        break;
    case 2:
        j %= 2;
        newcar->x = RoadU[j];
        newcar->y = 26;
        break;
    case 3:
        j %= 4;
        newcar->x = 1;
        newcar->y = RoadR[j % 4];
        break;
    }
    newcar->speed = j % 16;
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
    RefreshRoad();
    // DrawRoad();
    for (pre = car, current = pre->next; current != NULL; current = current->next)
    {
        // itoa(current->speed >> 4, s, 10);
        // PutAsc(20, 220, s, RED, 2, 2);
        if (current->x > 1024 || current->x < 0 || current->y < 34 || current->y > 768)
        {
            pre->next = current->next;
            free(current);
        }
        else
        {
            switch ((int)current->angle / 90) //根据车的方向判断是上下还是左右方向行驶
            {
            case 0:
                current->y -= v / 500;
                break;
            case 1:
                current->x -= v / 500;
                break;
            case 2:
                current->y += v / 300;
                break;
            case 3:
                current->x += v / 300;
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