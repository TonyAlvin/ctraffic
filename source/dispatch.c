/*****����ʱ�䣺 2019.9.22********************
 *  
 * ***���ߣ�������
 * ***���ܣ�������������λ�ü��㣬���̵ƹ���
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
            v = current->speed << 4 >> 4;  //��ȡ�ٶȴ�С��������speed�ĺ���λ������<<4��ǰ��λȥ��
            switch ((current->speed) >> 4) //���ݳ��ķ����ж������»������ҷ�����ʻ
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