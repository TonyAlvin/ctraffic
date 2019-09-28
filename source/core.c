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

void CountRunTimes(void)
{
    int times = 0;
    FILE *fp;
    if ((fp = fopen(".\\resource\\data.dat", "rb+")) == NULL)
    {
        printf("open data.dat fail~");
        delay(3000);
        exit(1);
    }
    fscanf(fp, "###%d###", &times);
    printf("No. %d Run", times);
    times++;
    rewind(fp);
    fprintf(fp, "###%d###", times);
    fclose(fp);
    delay(1000);
    return;
}
int main()
{
    CountRunTimes();
    SVGA_Init();
    //Set_Pal_File(".\\resource\\svga\\win.act");
    setbkcolor(BLACK);
    InitMouse();
    // DrawTrafficLight(200, 300, 1, RED);
    DrawRoad();
    DrawMenu();
    PutHZ24(500, 700, "³ÂÕ×ÀÊ111¹þ¹þ¹þ", WHITE, 1, 2, 0, 0);
    PutHZ16(500, 730, "³ÂÕ×ÀÊ111¹þ¹þ¹þ", WHITE, 1, 1, 0, 0);
    PutHZ12(500, 750, "³ÂÕ×ÀÊ111¹þ¹þ¹þ", WHITE, 1, 1, 0, 0);
    DrawTrafficLight(100, 200, 2, RED);
    while (1)
    {
        MouseRead();
        ButtonRefresh();
    }
}