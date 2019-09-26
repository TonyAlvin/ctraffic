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

/*计算鼠标的样子*/
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
		for (k = 0; k < 16; k++) /*四种组合方式*/
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
	mouse_draw[1][2] = 4; /*特殊点*/
}

/*鼠标光标显示*/
void MouseOn(int x, int y)
{
	int i, j;
	int color;

	for (i = 0; i < 16; i++) /*画鼠标*/
	{
		for (j = 0; j < 16; j++)
		{
			pixel_save[i][j] = getpixel(x + j, y + i); /*保存原来的颜色*/
			if (mouse_draw[i][j] == 1)
				putpixel(x + j, y + i, 0);
			else if (mouse_draw[i][j] == 2)
				putpixel(x + j, y + i, 15);
		}
	}
}

/*隐藏鼠标*/
void MouseHide(void)
{
	int i, j, x, y, color;
	x = MS.mx;
	y = MS.my;
	for (i = 0; i < 16; i++) /*原位置异或消去*/
		for (j = 0; j < 16; j++)
		{
			if (mouse_draw[i][j] == 3 || mouse_draw[i][j] == 4)
				continue;
			color = getpixel(x + j, y + i);
			putpixel(x + j, y + i, color ^ color);
			putpixel(x + j, y + i, pixel_save[i][j]);
		}
}

/*鼠标是否加载
	MouseExist:1=加载
			0=未加载
	MouseButton:鼠标按键数目   */
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

/*鼠标状态值初始化*/
void MouseReset(void)
{
	_AX = 0x00;
	geninterrupt(0x33);
}

/*设置鼠标左右边界
	lx:左边界
	gx:右边界                 */
void MouseSetX(int lx, int rx)
{
	_CX = lx;
	_DX = rx;
	_AX = 0x07;
	geninterrupt(0x33);
}

/*设置鼠标上下边界
	uy:上边界
	dy:下边界               */
void MouseSetY(int uy, int dy)
{
	_CX = uy;
	_DX = dy;
	_AX = 0x08;
	geninterrupt(0x33);
}

/*设置鼠标当前位置
	x:横向坐标
	y:纵向坐标                 */
void MouseSetXY(int x, int y)
{
	_CX = x;
	_DX = y;
	_AX = 0x04;
	geninterrupt(0x33);
}

/*设置鼠标速度(缺省值:vx=8,vy=1)
	值越大速度越慢*/
void MouseSpeed(int vx, int vy)
{
	_CX = vx;
	_DX = vy;
	_AX = 0x0f;
	geninterrupt(0x33);
}

/*获取鼠标按下键的信息*/
/*是否按下左键
返回值:   1=按下   0=释放*/
int LeftPress(void)
{
	_AX = 0x03;
	geninterrupt(0x33);
	return (_BX & 1);
}

/*是否按下中键
	返回值同上       */
int MiddlePress(void)
{
	_AX = 0x03;
	geninterrupt(0x33);
	return (_BX & 4);
}

/*是否按下右键
	返回值同上         */
int RightPress(void)
{
	_AX = 0x03;
	geninterrupt(0x33);
	return (_BX & 2);
}

/*获取鼠标当前位置*/
void MouseGetXY(void)
{
	_AX = 0x03;
	geninterrupt(0x33);
	MS.mx = _CX;
	MS.my = _DX;
}

/******************************************
*函数名：int  MouseRead                  *
*功能：读取鼠标按键状态                   *
*参数：void                               *
******************************************/
int MouseRead(void) /*鼠标按键情况*/
{
	int x, y;
	int status;
	unsigned long int press = 0;

	int i, j, color;
	status = 0; /*默认鼠标没又移动*/

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

	if (status) /*移动情况才重新显示鼠标*/
	{
		for (i = 0; i < 16; i++) /*原位置异或消去*/
			for (j = 0; j < 16; j++)
			{
				if (mouse_draw[i][j] == 3 || mouse_draw[i][j] == 4)
					continue;
				color = getpixel(x + j, y + i);
				putpixel(x + j, y + i, color ^ color);
				putpixel(x + j, y + i, pixel_save[i][j]);
			}
		MouseOn(MS.mx, MS.my); /*新位置显示*/
	}
	if (press != 0) /*有按键得情况*/
		return press;
	return 0; /*只移动得情况*/
}

void MouseShow(void)
{
	MouseGetXY();
	MouseOn(MS.mx, MS.my);
}

void InitMouse(void)
{
	MouseMath(); /*计算鼠标形状，一开始必须使用，后面就不用了*/
	MouseSetY(0, 767);
	MouseSetX(0, 1023);
	MouseSetXY(320, 240);
	MouseOn(MS.mx, MS.my); /*第一次显示鼠标*/
}

/*检测鼠标是否在按钮上的函数*/
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

/*检测鼠标是否在窗口上的函数*/
int MouseOnWindow(int sx, int sy, int ex, int ey)
{
	if (MS.mx > sx && MS.mx < ex && MS.my > sy && MS.my < ey)
		return 1;
	else
		return 0;
}

/*检测鼠标是否在输入框上*/
int MouseOnBox(BOX *box)
{
	if (MS.mx > (box->bx0) && MS.mx < (box->bx0 + box->blen) && MS.my > (box->by0) && MS.my < (box->by0 + box->bwid))
		return 1;
	else
		return 0;
}