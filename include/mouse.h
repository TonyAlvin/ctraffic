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
int MouseOnBox(struct INPUT_BOX *box);             //检测鼠标是否在输入框上

#endif