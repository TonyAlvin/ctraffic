#include <stdio.h>
#include <stdlib.h>
#include <mouse.h>
#include "all.h"
#include <graphics.h>
#include "hanzi.h"

void BtExit(void)
{
    exit(0);
}

void BtAbout(void)
{
    static about = 0;
    void *AboutBuf;
    AboutBuf = malloc(imagesize(100, 20, 300, 320));
    setfillstyle(1, DARKGRAY);
    bar(100, 20, 300, 320);
    PutHZ16(110, 30, "³ÂÕ×ÀÉ ¶­¹úÇì", WHITE, 1, 2, 2, 20);
    about = 1;
    while (!MouseOnBT(&bt_about))
        ;
    putimage(100, 20, AboutBuf, 1);
    free(AboutBuf);
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