#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>
#include "mouse.h"
#include "all.h"
#include "hanzi.h"

void BtExit(void)
{
    exit(0);
}

void BtFeature(void)
{
    setfillstyle(0, BLACK);
    bar(310, 300, 699, 499);
    PutHZ24(310, 300, "����BorlandC�����ܽ�ͨ����ϵͳ", WHITE, 1, 0, 1, 0);
    PutHZ16(330, 350, "1.�����·�����ɿ��Ƴ����������", WHITE, 1, 1, 1, 0);
    PutHZ16(330, 380, "2.������ر��Ҳ࿪�ؿɵ��ں��̵�ģʽ", WHITE, 1, 1, 1, 0);
    PutHZ16(330, 410, "3.������½ǵķ�����ť�ɷ����Ż���", WHITE, 1, 1, 1, 0);
    delay(50);
    while (1)
    {
        MouseRead();
        if (LeftPress())
            break;
    }
    bar(310, 300, 699, 499);
    return;
}

void BtAbout(void)
{
    setfillstyle(0, BLACK);
    bar(354, 300, 699, 499);
    PutHZ16(450, 300, "���ߣ�", WHITE, 2, 2, 2, 0);
    PutHZ16(356, 350, "������ U201814993", WHITE, 2, 2, 2, 0);
    PutHZ16(356, 390, "������ U201812992", WHITE, 2, 2, 2, 0);
    delay(50);
    while (1)
    {
        MouseRead();
        if (LeftPress())
            break;
    }
    bar(354, 300, 699, 499);
    return;
}

// void BtIntro(void)
// {
//     void *AboutBuf;
//     AboutBuf = malloc(imagesize(150, 20, 350, 320));
//     setfillstyle(1, DARKGRAY);
//     bar(150, 20, 350, 320);
//     PutHZ16(110, 30, "�� ��", WHITE, 1, 2, 2, 20);
//     while (LeftPress() && (!MouseOnBT(&bt_gnjs)))
//         ;
//     putimage(150, 20, AboutBuf, 1);
//     free(AboutBuf);
//     return;
// }