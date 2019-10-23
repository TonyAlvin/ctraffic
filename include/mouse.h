#ifndef __mouse_h__
#define __mouse_h__

/*鼠标信息宏定义*/
#define WAITING 0xff00
#define LEFTPRESS 0xff01
#define LEFTCLICK 0xff10
#define LEFTDRAG 0xff19
#define RIGHTPRESS 0xff02
#define RIGHTCLICK 0xff20
#define RIGHTDRAG 0xff2a
#define MIDDLEPRESS 0xff04
#define MIDDLECLICK 0xff40
#define MIDDLEDRAG 0xff4c
#define MOUSEMOVE 0xff08

void InitMouse(void);
void MouseHide(void);
void MouseShow(void);
int MouseRead(void);

int LeftPress(void);                               //检测左键是否按下
int MouseOnBT(struct BUTTON *bt);                  //检测鼠标是否在按钮上的函数
int MouseOnWindow(int sx, int sy, int ex, int ey); //检测鼠标是否在窗口上的函数
int MousePressIn(int x1, int y1, int x2, int y2);  //检测鼠标是否在区域内按下
void MouseXY(int *x, int *y);                      //获取鼠标坐标

#endif