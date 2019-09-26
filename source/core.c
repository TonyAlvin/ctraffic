#include <stdio.h>
#include <stdlib.h>
#include "svgasub.h"
#include <graphics.h>
#include "mouse.h"
#include "hanzi.h"
#include "draw.h"
#include "all.h"

extern int MouseX;
extern int MouseY;
extern int MouseB;

int main()
{
    SVGA_Init();
    //Set_Pal_File(".\\resource\\svga\\win.act");
    setbkcolor(BLACK);
    InitMouse();
    // DrawTrafficLight(200, 300, 1, RED);
    DrawRoad();
    DrawMenu();
    PutHZ24(500, 700, "������111������", WHITE, 1, 2, 0, 0);
    PutHZ16(500, 730, "������111������", WHITE, 1, 1, 0, 0);
    PutHZ12(500, 750, "������111������", WHITE, 1, 1, 0, 0);
    while (1)
    {
        MouseRead();
        ButtonRefresh();
    }
}