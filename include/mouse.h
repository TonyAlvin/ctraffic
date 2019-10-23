#ifndef __mouse_h__
#define __mouse_h__

/*�����Ϣ�궨��*/
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

int LeftPress(void);                               //�������Ƿ���
int MouseOnBT(struct BUTTON *bt);                  //�������Ƿ��ڰ�ť�ϵĺ���
int MouseOnWindow(int sx, int sy, int ex, int ey); //�������Ƿ��ڴ����ϵĺ���
int MousePressIn(int x1, int y1, int x2, int y2);  //�������Ƿ��������ڰ���
void MouseXY(int *x, int *y);                      //��ȡ�������

#endif