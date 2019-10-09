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
#include "light.h"

void SolveAlarm(CAR *p);                                //����������
void MoveCar(CAR *p);                                   //ֱ���ƶ�������
void CarSingle(CAR *car, CAR *p, CAR *prep, CAR *turn); //���Ʒ�·���ڵ�����������
void StrightChangeJustment(CAR *p);                     //����ֱ�г���justment����
void LeftChangeJustment(CAR *p);                        //������ת����justment����
void RightChangeJustment(CAR *p);                       //������ת����justment����
void PreScan(CAR *car, CAR *tar);                       //ɨ��ǰ����������
void TurnCar(CAR *p);                                   //����·���ڳ�������
void TurnLeftCar(CAR *p);                               //���Ƴ�����ת
void TurnRightCar(CAR *p);                              //���Ƴ�����ת
int StrightPreScan(CAR *car, CAR *p);                   //ֱ��Ԥɨ�躯���������ж�ǰ��·���Ƿ�ѹ������·��
int GetLightStatusC(int just, int turn, int x, int y);  //��ȡʮ��·�ڵ�ǰ�����Ƿ�ͨ��
int GetLightStatusT(int just, int turn, int x, int y);  //��ȡ����·�ڵ�ǰ�����Ƿ�ͨ��

//�������������������һ����min��max�������
int RandInt(int min, int max)
{
    srand((unsigned)time(NULL));
    return rand() % (max - min + 1) + min;
}

/*********************************************
 * �������ܣ���ʼ��������
 * ������newcar:����ָ�룬dirt:·��1U��2D��3L��4R
 * ����ֵ��void
 * ******************************************/
void InitCar(CAR *newcar)
{
    int RoadR[] = {UU1, UU2, DU1, DU2}, //�洢·��������Ϣ�������������
        RoadL[] = {UD1, UD2, DD1, DD2},
        RoadU[] = {LL1, LL2},
        RoadD[] = {LR1, LR2, RR1, RR2},
        ang;
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
            break;
        case LR2:
            newcar->justment = 224;
            break;
        case RR1:
            newcar->justment = 243;
            break;
        case RR2:
            newcar->justment = 244;
            break;
        default:
            PutAsc(500, 300, "Init0 Car Error", RED, 2, 2);
        }
        break;
    case 1:
        newcar->x = 1;
        newcar->y = RoadL[RandInt(0, 3)];
        switch (newcar->y)
        {
        case UD1:
            newcar->justment = 103;
            break;
        case UD2:
            newcar->justment = 104;
            break;
        case DD1:
            newcar->justment = 133;
            break;
        case DD2:
            newcar->justment = 134;
            break;
        default:
            PutAsc(500, 300, "Init3 Car Error", RED, 2, 2);
        }
        break;
    case 2:
        newcar->x = RoadU[RandInt(0, 1)];
        newcar->y = 1;
        switch (newcar->x)
        {
        case LL1:
            newcar->justment = 201;
            break;
        case LL2:
            newcar->justment = 202;
            break;
        default:
            PutAsc(500, 300, "Init2 Car Error", RED, 2, 2);
        }
        break;
    case 3:
        newcar->x = 1023;
        newcar->y = RoadR[RandInt(0, 3)];
        switch (newcar->y)
        {
        case UU1:
            newcar->justment = 121;
            break;
        case UU2:
            newcar->justment = 122;
            break;
        case DU1:
            newcar->justment = 151;
            break;
        case DU2:
            newcar->justment = 152;
            break;
        default:
            PutAsc(500, 300, "Init1 Car Error", RED, 2, 2);
        }
        break;
    default:
        PutAsc(500, 300, "Init Car Error", RED, 2, 2);
    }
    newcar->turn[0] = RandInt(1, 3); //����ת����Ϣ
    newcar->turn[1] = RandInt(1, 3);
    newcar->turn[2] = RandInt(1, 3);
    newcar->turn[3] = RandInt(1, 3);
    newcar->std_speed = RandInt(1, 16); //���ɳ�ʼ�ٶ�
    newcar->speed = newcar->std_speed;  //��ǰ�ٶ�=��ʼ�ٶ�
    newcar->color = RandInt(2, 8);      //����ɫ���
    newcar->alarm = 0;                  //��ǰ�������ʼ��Ϊ��
    newcar->flag = 0;                   //ʻ����Ļ��ǣ���ʼ��Ϊ��
    // DrawCar(newcar);
}
//����ʱ��������ʹ�ã���ʵ������
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
 * �������ܣ������������
 * ������ ��������ͷ
 * ����ֵ����
 * *****************************************/
void CarListDispatch(CAR *car, CAR *turn)
{
    CAR *road, *current, *precurrent;
    road = car;
    NormalControl(1);
    precurrent = road;
    for (current = precurrent->next; current != NULL; current = precurrent->next)
    {
        CarSingle(car, current, precurrent, turn);
        precurrent = precurrent->next;
    }
    road = turn;
    for (current = road->next; current != NULL; current = current->next)
        TurnCar(current);
}

/**********************************************
 * ��������int JudgeCross(CAR *p)
 * ������               ��ǰ��ָ��
 * ����ֵ���ж����ĸ�·�ڷ�Χ�ڣ�0Ϊ����·��
***********************************************/
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

/**********************************************************************
 * ��������void CarSingle(CAR *car, CAR *p, CAR *prep, CAR *turn)
 * ������              ֱ��������ͷ  ��ǰ������ǰ����ǰһ����ת������ͷ
 * ����ֵ���գ�
 * �������ܣ�����ֱ�������ƣ���·�ڲ��ж��̵ƺ󽫳�����·������
 * *******************************************************************/
void CarSingle(CAR *car, CAR *p, CAR *prep, CAR *turn)
{
    int place = JudgeCross(p);
    switch (place)
    {
    case 0: //��·��ֻ���ж�ǰ�����޳���������ʻ
        PreScan(car, p);
        SolveAlarm(p);
        MoveCar(p);
        break;
    case 1:
    case 2:
    case 3: //ʮ��·��ͳһ����
        if (GetLightStatusC(p->justment, p->turn[place - 1], (p->x < 500) ? 250 : 750, (p->y < 400) ? 270 : 570))
        { //�̵���
            PreScan(turn, p);
            SolveAlarm(p);
            MoveCar(p);
            prep->next = p->next;
            p->next = turn->next;
            turn->next = p;
        } //���ͣ�����Գ���λ�ü�����������в���
        break;
    case 4: //����·�ڵ�������
        if (GetLightStatusT(p->justment, p->turn[place - 1], 750, 270))
        { //�̵���
            PreScan(turn, p);
            SolveAlarm(p);
            MoveCar(p);
            prep->next = p->next;
            p->next = turn->next;
            turn->next = p;
        } //���ͣ�����Գ���λ�ü�����������в���
        break;
    default: //����justment��������
        PutAsc(p->x, p->y, "find place error", RED, 2, 2);
    }
    DrawCar(p);
    // setfillstyle(0, BLACK);
    // setcolor(BLACK);
    // bar(300, 400, 400, 450);
    // PutAsc(300, 400, "car", RED, 2, 2);
}

/*******��ȡʮ��·�ں��̵�״̬����*************************
 * int GetLightStatusC(int just, int turn, int x, int y)
 * ������            ������ǰ���� Ҫ�ߵķ���   ·����������
 * ����ֵ��1:������ 0��������
 * *******************************************************/
int GetLightStatusC(int just, int turn, int x, int y)
{
    int flag;
    switch (turn)
    {
    case 1:
        if (just / 100 == 1)
            flag = getpixel(x - 58, y - 38);
        else
            flag = getpixel(x + 38, y - 58);
        return (flag == GREEN);
    case 2:
        if (just / 100 == 1)
            flag = getpixel(x - 12, y + 58);
        else
            flag = getpixel(x - 58, y - 12);
        return (flag == GREEN);
    case 3:
        return 1;
    default:
        PutAsc(x, y, "LightErrorC", RED, 1, 1);
        return 0;
    }
}
/*******��ȡ����·�ں��̵�״̬����***************************
 * int GetLightStatusT(int just, int turn, int x, int y)
 * ������            ������ǰ���� Ҫ�ߵķ���   ·����������
 * ����ֵ��1:������ 0��������
 * ********************************************************/
int GetLightStatusT(int just, int turn, int x, int y)
{
    int flag;
    switch (turn)
    {
    case 1:
        flag = getpixel(x - 58, y - 38);
        return (flag == GREEN);
    case 2:
        if (just / 100 == 1)
            flag = getpixel(x + 24, y - 58);
        else
            flag = getpixel(x - 58, y - 12);
        return (flag == GREEN);
    case 3:
        return 1;
    default:
        PutAsc(x, y, "LightErrorT", RED, 1, 1);
        return 0;
    }
}

void TurnCar(CAR *p)
{
    switch (p->turn[JudgeCross(p) - 1])
    {
    case 1:
        MoveCar(p);
        break;
    case 2:
        TurnLeftCar(p);
        break;
    case 3:
        TurnRightCar(p);
        break;
    default:
        PutAsc(p->x, p->y, "bad turn", RED, 1, 1);
        break;
    }
}

void TurnLeftCar(CAR *car)
{

    switch ((int)(car->angle))
    {
    case 0:
        if (car->alarm == 0)
        {
            car->x = car->x - 62 + (int)(62 * cos(0.1047));
            car->y = car->y - (int)(62 * sin(0.1047));
            car->count++;
            if (car->count == 15)
            {
                car->count = 0;
                car->angle = 270;
            }
        }
        break;
    case 90:
        if (car->alarm == 0)
        {
            car->x = car->x + (int)(62 * sin(0.1047));
            car->y = car->y - 62 + (int)(62 * cos(0.1047));
            car->count++;
            if (car->count == 15)
            {
                car->count = 0;
                car->angle = 0;
            }
        }
        break;
    case 180:
        if (car->alarm == 0)
        {
            car->x = car->x + (int)(62 * sin(0.1047));
            car->y = car->y + (int)(62 * sin(0.1047));
            car->count++;
            if (car->count == 15)
            {
                car->count = 0;
                car->angle = 90;
            }
        }
        break;
    case 270:
        if (car->alarm == 0)
        {
            car->x = car->x - (int)(62 * sin(0.1047));
            car->y = car->y - (int)(62 * sin(0.1047));
            car->count++;
            if (car->count == 15)
            {
                car->count = 0;
                car->angle = 180;
            }
        }
        break;
    default:
        break;
    }
}

void TurnRightCar(CAR *p)
{
}

int StrightPreScan(CAR *car, CAR *p)
{
    CAR *current;
    int pre = p->justment + (((int)(p->angle) % 270 == 0) ? 10 : -10);
    for (current = car->next; current != NULL; current = current->next)
        if ((current->justment == pre) && (JudgeCross(current) == JudgeCross(p)))
            return 1;
    return 0;
}

//ֱ��ɨ�躯�������ã�
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
            case 153:
            case 154:
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

void RightChangeJustment(CAR *p)
{
    switch (p->justment)
    {
    case 104:
        p->justment = 211;
        break;
    case 111:
        p->justment = 204;
        break;
    case 201:
        p->justment = 101;
        break;
    case 214:
        p->justment = 114;
        break;
    case 211:
        p->justment = 131;
        break;
    case 224:
        p->justment = 144;
        break;
    case 134:
        p->justment = 221;
        break;
    case 141:
        p->justment = 214;
        break;
    case 114:
        p->justment = 231;
        break;
    case 234:
        p->justment = 124;
        break;
    case 244:
        p->justment = 154;
        break;
    case 231:
        p->justment = 141;
        break;
    case 144:
        p->justment = 241;
        break;
    case 151:
        p->justment = 234;
        break;
    default:
        break;
    }
    return;
}

void LeftChangeJustment(CAR *p)
{
    switch (p->justment)
    {
    case 103:
        p->justment = 203;
        break;
    case 112:
        p->justment = 212;
        break;
    case 213:
        p->justment = 102;
        break;
    case 202:
        p->justment = 113;
        break;
    case 133:
        p->justment = 213;
        break;
    case 142:
        p->justment = 222;
        break;
    case 212:
        p->justment = 143;
        break;
    case 223:
        p->justment = 132;
        break;
    case 122:
        p->justment = 232;
        break;
    case 233:
        p->justment = 112;
        break;
    case 243:
        p->justment = 142;
        break;
    case 232:
        p->justment = 153;
        break;
    case 143:
        p->justment = 233;
        break;
    case 152:
        p->justment = 242;
        break;
    default:
        break;
    }
    return;
}

void StrightChangeJustment(CAR *p)
{
    p->justment += (((int)(p->angle) % 270 == 0) ? 10 : -10);
    return;
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
        PutAsc(p->x, p->y, "bad car alarm", RED, 2, 2);
        delay(1000);
        break;
    }
}