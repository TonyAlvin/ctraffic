#include <stdio.h>
#include <stdlib.h>
#include "svgasub.h"
#include <graphics.h>
#include "light.h"
#include "mouse.h"
#include "hanzi.h"
#include "draw.h"
#include "dispatch.h"
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
    CAR *a, *b = NULL;
    SVGA_Init();
    CountRunTimes();
    CreatCarList(&a,10);
    //Set_Pal_File(".\\resource\\svga\\win.act");
    setbkcolor(BLACK);
    InitMouse();
    DrawRoad();
    DrawMenu();
    while (1)
    {
        NormalControl(1);
        MouseRead();
        ButtonRefresh();
        CarListDispatch(a,b);
    }
}