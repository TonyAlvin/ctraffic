/*****����ʱ�䣺 2019.9.22********************
 *  
 * ***���ߣ�������
 * ***���ܣ�������������λ�ü��㣬���̵ƹ���
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
/*********************************************
 * �������ܣ���ʼ��������
 * ������newcar:����ָ�룬dirt:·��1U��2D��3L��4R
 * ����ֵ��void
 * ******************************************/
void InitCar(CAR *newcar, int dirt)
{
}

void CreatCarList(CAR **head, int n, int dirt)
{
    int i;
    CAR **current = head;
    *current = (CAR *)malloc(sizeof(CAR));
    for (i = 0; i < n; i++)
    {
        current = &((*current)->next);
        *current = (CAR *)malloc(sizeof(CAR));
        InitCar(*current, dirt);
    }
    (*current)->next = NULL;
}

/*******************************************
 * �������ܣ������������
 * ������ ��������ͷ
 * ����ֵ����
 * *****************************************/
void CarListDispatch(CAR *car[], CAR *turn)
{
    int i = 0;
    CAR *road, *current;
    road = *car;
    for (current = car->next; current != NULL; current = current->next)
        CarSingle(current);
    road = *(car + 1);
    for (current = car->next; current != NULL; current = current->next)
        TurnCar(currsent);
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

void CarSingle(CAR *p)
{
    switch (JudgeCross(p))
    {
    case 0:
        PreScan(p);
        SolveAlarm(p);
        MoveCar(p);
        break;
    case 1:
    case 2:
    case 3:

        break;
    case 4:

        break;
    default:
        PutAsc(p->x, p->y, "find place error", RED, 2, 2);
    }
}

void TurnCar(CAR *p)
{
}

void TurnLeftCar(CAR *p)
{
    int road = p->justment / 100, lane = p->justment % 10;
}

void TurnRightCar(CAR *p)
{
}

//ֱ��ɨ�躯�������ã�
void PreScan(CAR *car[], CAR *tar)
{
    CAR *current;
    tar->alarm = 0;
    for (current = *car; current != NULL; current = current->next)
    {
        if (tar->justment = current->justment)
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
                    tar->alarm = 0; //�������ݶȻ�������̾�����ͣ�����о���������ʻ���������Գ�ʼ���ٶ���ʻ
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
            case 151:
            case 152:
                if ((current->x > tar->x) && ((current->x - DIS) < tar->x))
                    tar->alarm = 2;
                else if (((current->x - DIS) >= tar->x) && ((current->x - DIL) < tar->x) && (tar->alarm != 2))
                    tar->alarm = 1;
                else if (((current->x - DIL) >= tar->x) && (tar->alarm != 0))
                    tar->alarm = 0; //�������ݶȻ�������̾�����ͣ�����о���������ʻ���������Գ�ʼ���ٶ���ʻ
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
                    tar->alarm = 0; //�������ݶȻ�������̾�����ͣ�����о���������ʻ���������Գ�ʼ���ٶ���ʻ
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
                    tar->alarm = 0; //�������ݶȻ�������̾�����ͣ�����о���������ʻ���������Գ�ʼ���ٶ���ʻ
                break;
            default:
                PutAsc(tar->x, tar->y, "bad justment", RED, 2, 2);
                break;
            }
    }
}

void GoStrightCar(CAR *p)
{
}

//ֱ����ʻʱ���㳵��λ��
//���������CAR*����ָ��
//����ֵ����
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

//����ǰ�������ĺ���(����)
void SolveAlarm(CAR *p)
{
    switch (p->alarm)
    {
    case 0:                      //û�о���
        p->speed = p->std_speed; //��Ϊ��ʼ�����ٶ�
        break;
    case 1:                  //������о��ڣ��о���
        p->speed = 1 * RATE; //��Ϊ����
        break;
    case 2:           //�������̾����ڣ��о���
        p->speed = 0; //ͣ��
    default:
        PutAsc(p->x, p->y, "bad car alarm");
        delay(1000);
        break;
    }
}