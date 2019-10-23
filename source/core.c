/*******************************************
 * *         *****主函数文件******
 * ***作者：董国庆
 * ***创建时间：2019.7.10
 * ****************************************/
#include "svgasub.h"
#include <graphics.h>
#include "mouse.h"
#include "all.h"
#include "menu.h"

extern int MouseX;
extern int MouseY;
extern int MouseB;

int main()
{
    CountRunTimes(); //记录运行次数
    SVGA_Init();     //初始化图形界面
    //设置windows调色板，可以开启16位色模式，本程序使用16色模式，不设置调色板
    // Set_Pal_File(".\\resource\\svga\\win.act");
    setbkcolor(BLACK); //设置背景色
    InitMouse();       //初始化鼠标
    menu();            //进入菜单选项
    return 0;
}