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

void BtAbout(void)
{
    PutHZ16(450, 300, "×÷Õß", WHITE, 2, 2, 2, 0);
    PutHZ16(360, 350, "¶­¹úÇì U201814993", WHITE, 2, 2, 2, 0);
    PutHZ16(360, 390, "³ÂÕ×ÀÊ U201812992", WHITE, 2, 2, 2, 0);
    delay(50);
    while (1)
    {
        MouseRead();
        if (LeftPress())
            break;
    }
    setfillstyle(0, BLACK);
    bar(360, 300, 650, 440);
    return;
}

void BtIntro(void)
{
    void *AboutBuf;
    AboutBuf = malloc(imagesize(150, 20, 350, 320));
    setfillstyle(1, DARKGRAY);
    bar(150, 20, 350, 320);
    PutHZ16(110, 30, "³Â Çì", WHITE, 1, 2, 2, 20);
    while (LeftPress() && (!MouseOnBT(&bt_gnjs)))
        ;
    putimage(150, 20, AboutBuf, 1);
    free(AboutBuf);
    return;
}