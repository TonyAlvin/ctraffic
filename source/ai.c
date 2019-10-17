#include <stdlib.h>
#include <graphics.h>
#include "ai.h"
#include "all.h"
#include "hanzi.h"
#include "light.h"

int LaneCount(CAR *car, int lane); //�����ڳ�������
int GetLight(int num);             //��ȡnum·�ڵĺ��̵�״̬

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
    if (num == 3) //����·�ڵ����ж�
    {
        if (getpixel(RIGHT_X - 58, UP_Y - 38) == GREEN)
            status = 21; //ˮƽ��ֱ��
        else if (getpixel(RIGHT_X - 58, UP_Y - 12) == GREEN)
            status = 12; //��ֱ������ת
        else if (getpixel(RIGHT_X + 24, UP_Y - 58))
            status = 22; //ˮƽ������ת
    }
    else
    {
        x = (num < 3) ? LEFT_X : RIGHT_X, y = (num < 1) ? UP_Y : DOWN_Y;
        if (getpixel(x - 58, y - 38) == GREEN)
            status = 21; //ˮƽ����ֱ��
        else if (getpixel(x + 38, y - 58))
            status = 11; //��ֱ����ֱ��
        else if (getpixel(x - 58, y - 12) == GREEN)
            status = 22; //ˮƽ������ת
        else if (getpixel(x - 12, y + 58))
            status = 12; //��ֱ������ת
    }
    return status;
}