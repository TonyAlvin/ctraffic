#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <bios.h>
#include "mouse.h"
#include "all.h"

extern struct MOUSE
{
	int mx;
	int my;
	int mstate;
};
struct MOUSE MS;

static int up[16][16];
static int down[16][16];
static int mouse_draw[16][16];
static int pixel_save[16][16];
static int MouseExist;
static int MouseButton;

/*������������*/
void MouseMath(void)
{
	int i, j, jj, k;
	long UpNum[16] = {
		0x3fff, 0x1fff, 0x0fff, 0x07ff,
		0x03ff, 0x01ff, 0x00ff, 0x007f,
		0x003f, 0x00ff, 0x01ff, 0x10ff,
		0x30ff, 0xf87f, 0xf87f, 0xfc3f};
	long DownNum[16] = {
		0x0000, 0x7c00, 0x6000, 0x7000,
		0x7800, 0x7c00, 0x7e00, 0x7f00,
		0x7f80, 0x7e00, 0x7c00, 0x4600,
		0x0600, 0x0300, 0x0300, 0x0180};
	for (i = 0; i < 16; i++)
	{
		j = jj = 15;
		while (UpNum[i] != 0)
		{
			up[i][j] = UpNum[i] % 2;
			j--;
			UpNum[i] /= 2;
		}
		while (DownNum[i] != 0)
		{
			down[i][jj--] = DownNum[i] % 2;
			DownNum[i] /= 2;
		}
		for (k = j; k >= 0; k--)
			up[i][k] = 0;
		for (k = jj; k >= 0; k--)
			down[i][k] = 0;
		for (k = 0; k < 16; k++) /*������Ϸ�ʽ*/
		{
			if (up[i][k] == 0 && down[i][k] == 0)
				mouse_draw[i][k] = 1;
			else if (up[i][k] == 0 && down[i][k] == 1)
				mouse_draw[i][k] = 2;
			else if (up[i][k] == 1 && down[i][k] == 0)
				mouse_draw[i][k] = 3;
			else
				mouse_draw[i][k] = 4;
		}
	}
	mouse_draw[1][2] = 4; /*�����*/
}

/*�������ʾ*/
void MouseOn(int x, int y)
{
	int i, j;
	int color;

	for (i = 0; i < 16; i++) /*�����*/
	{
		for (j = 0; j < 16; j++)
		{
			pixel_save[i][j] = getpixel(x + j, y + i); /*����ԭ������ɫ*/
			if (mouse_draw[i][j] == 1)
				putpixel(x + j, y + i, 0);
			else if (mouse_draw[i][j] == 2)
				putpixel(x + j, y + i, 15);
		}
	}
}

/*�������*/
void MouseHide(void)
{
	int i, j, x, y, color;
	x = MS.mx;
	y = MS.my;
	for (i = 0; i < 16; i++) /*ԭλ�������ȥ*/
		for (j = 0; j < 16; j++)
		{
			if (mouse_draw[i][j] == 3 || mouse_draw[i][j] == 4)
				continue;
			color = getpixel(x + j, y + i);
			putpixel(x + j, y + i, color ^ color);
			putpixel(x + j, y + i, pixel_save[i][j]);
		}
}

/*����Ƿ����
	MouseExist:1=����
			0=δ����
	MouseButton:��갴����Ŀ   */
void MouseLoad(void)
{
	_AX = 0x00;
	geninterrupt(0x33);
	MouseExist = _AX;
	if (MouseExist == 0)
	{
		printf("can't load mouse!");
		delay(3000);
		exit(0);
	}
	MouseButton = _BX;
}

/*���״ֵ̬��ʼ��*/
void MouseReset(void)
{
	_AX = 0x00;
	geninterrupt(0x33);
}

/*����������ұ߽�
	lx:��߽�
	gx:�ұ߽�                 */
void MouseSetX(int lx, int rx)
{
	_CX = lx;
	_DX = rx;
	_AX = 0x07;
	geninterrupt(0x33);
}

/*����������±߽�
	uy:�ϱ߽�
	dy:�±߽�               */
void MouseSetY(int uy, int dy)
{
	_CX = uy;
	_DX = dy;
	_AX = 0x08;
	geninterrupt(0x33);
}

/*������굱ǰλ��
	x:��������
	y:��������                 */
void MouseSetXY(int x, int y)
{
	_CX = x;
	_DX = y;
	_AX = 0x04;
	geninterrupt(0x33);
}

/*��������ٶ�(ȱʡֵ:vx=8,vy=1)
	ֵԽ���ٶ�Խ��*/
void MouseSpeed(int vx, int vy)
{
	_CX = vx;
	_DX = vy;
	_AX = 0x0f;
	geninterrupt(0x33);
}

/*��ȡ��갴�¼�����Ϣ*/
/*�Ƿ������
����ֵ:   1=����   0=�ͷ�*/
int LeftPress(void)
{
	_AX = 0x03;
	geninterrupt(0x33);
	return (_BX & 1);
}

/*�Ƿ����м�
	����ֵͬ��       */
int MiddlePress(void)
{
	_AX = 0x03;
	geninterrupt(0x33);
	return (_BX & 4);
}

/*�Ƿ����Ҽ�
	����ֵͬ��         */
int RightPress(void)
{
	_AX = 0x03;
	geninterrupt(0x33);
	return (_BX & 2);
}

/*��ȡ��굱ǰλ��*/
void MouseGetXY(void)
{
	_AX = 0x03;
	geninterrupt(0x33);
	MS.mx = _CX;
	MS.my = _DX;
}

/******************************************
*��������int  MouseRead                  *
*���ܣ���ȡ��갴��״̬                   *
*������void                               *
******************************************/
int MouseRead(void) /*��갴�����*/
{
	int x, y;
	int status;
	unsigned long int press = 0;

	int i, j, color;
	status = 0; /*Ĭ�����û���ƶ�*/

	x = MS.mx;
	y = MS.my;
	while (x == MS.mx && y == MS.my && status == 0 && press == 0)
	{
		if (kbhit())
		{
			press = bioskey(0);
		}

		if (LeftPress())
			press = 1;

		else if (RightPress())
			press = 2;
		else if (LeftPress() && RightPress())
			press = 3;

		MouseGetXY();
		if (MS.mx != x || MS.my != y)
			status = 1;
	}

	if (status) /*�ƶ������������ʾ���*/
	{
		for (i = 0; i < 16; i++) /*ԭλ�������ȥ*/
			for (j = 0; j < 16; j++)
			{
				if (mouse_draw[i][j] == 3 || mouse_draw[i][j] == 4)
					continue;
				color = getpixel(x + j, y + i);
				putpixel(x + j, y + i, color ^ color);
				putpixel(x + j, y + i, pixel_save[i][j]);
			}
		MouseOn(MS.mx, MS.my); /*��λ����ʾ*/
	}
	if (press != 0) /*�а��������*/
		return press;
	return 0; /*ֻ�ƶ������*/
}

void MouseShow(void)
{
	MouseGetXY();
	MouseOn(MS.mx, MS.my);
}

void InitMouse(void)
{
	MouseMath(); /*���������״��һ��ʼ����ʹ�ã�����Ͳ�����*/
	MouseSetY(0, 767);
	MouseSetX(0, 1023);
	MouseSetXY(320, 240);
	MouseOn(MS.mx, MS.my); /*��һ����ʾ���*/
}

/*�������Ƿ��ڰ�ť�ϵĺ���*/
int MouseOnBT(BT *bt)
{
	if (MS.mx > (bt->btx1 - BTDX) && MS.mx < (bt->btx2 + BTDX) && MS.my > (bt->bty1 - BTDY) && MS.my < (bt->bty2 + BTDY))
	{
		if (LeftPress())
			return 2;
		else
			return 1;
	}
	else
		return 0;
}

/*�������Ƿ��ڴ����ϵĺ���*/
int MouseOnWindow(int sx, int sy, int ex, int ey)
{
	if (MS.mx > sx && MS.mx < ex && MS.my > sy && MS.my < ey)
		return 1;
	else
		return 0;
}

/*�������Ƿ����������*/
int MouseOnBox(BOX *box)
{
	if (MS.mx > (box->bx0) && MS.mx < (box->bx0 + box->blen) && MS.my > (box->by0) && MS.my < (box->by0 + box->bwid))
		return 1;
	else
		return 0;
}