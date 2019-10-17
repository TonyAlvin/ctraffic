#include "hanzi.h"
#include <graphics.h>
#include "io.h"
#include <FCNTL.H>

void ReadAsc16(char key, unsigned char *buf)
{
	int handle;
	long address;
	handle = open(".\\resource\\hzk\\asc16", O_RDONLY | O_BINARY);
	address = key * 16l;
	lseek(handle, address, SEEK_SET);
	read(handle, buf, 16);
	close(handle);
}

void PutAsc16(int cx, int cy, char key, int fcolor)
{
	int a, b;
	unsigned char buf[16];
	ReadAsc16(key, buf);
	for (a = 0; a < 16; a++)
		for (b = 0; b < 8; b++)
			if ((buf[a] >> 7 - b) & 1)
				putpixel(cx + a, cy + b, fcolor);
}

void PutAsc16Muliti(int cx, int cy, int xsize, int ysize, char key, int fcolor)
{
	int a, b, o, k;
	unsigned char buf[16];
	ReadAsc16(key, buf);
	for (a = 0; a < 16; a++)
		for (o = 1; o <= ysize; o++)
			for (b = 0; b < 8; b++)
				for (k = 1; k <= xsize; k++)
					if ((buf[a] >> 7 - b) & 1)
						putpixel(cx + b * xsize + k, cy + o + a * ysize, fcolor);
}
/* 显示英文和数字 */
void PutAsc(int cx, int cy, char *s, char color, int xsize, int ysize)
{
	int index;
	for (index = 0; s[index] != 0; index++)
	{
		PutAsc16Muliti(cx + index * xsize * 8, cy, xsize, ysize, s[index], color);
	}
}

void PutHZ12(int x, int y, char *s, int color, int dx, int dy, int distance, char speed) // 12*12点阵汉字的显示
{
	unsigned long offset;
	FILE *fp;
	char buffer[24], word[2] = {'\0', '\0'}; //buffer用来存储一个汉字
	int m, n, i, j, k;
	unsigned char qh, wh;
	if ((fp = fopen(".\\resource\\hzk\\hzk12", "rb")) == NULL)
	{
		printf("Can't open hzk12,Please add it");
		exit(1);
	}
	while (*s)
	{
		if ((int)(*s) > 0 && (int)(*s) < 128)
		{
			setcolor(color);
			word[0] = *s;
			outtextxy(x + 2, y + 4, word);
			s += 1;
			x += 9;
		}
		else
		{
			qh = *(s)-0xa0; //汉字区位码
			wh = *(s + 1) - 0xa0;
			offset = (94 * (qh - 1) + (wh - 1)) * 24L; //计算该汉字在字库中偏移量
			fseek(fp, offset, SEEK_SET);
			fread(buffer, 24, 1, fp); //取出汉字32字节的点阵字模存入buffer中（一个汉字）
			for (i = 0; i < 12; i++)  //将32位字节的点阵按位在屏幕上打印出来(1:打印,0:不打印),显示汉字
				for (n = 0; n < dx; n++)
					for (j = 0; j < 2; j++)
						for (k = 0; k < 8; k++)
							for (m = 0; m < dy; m++)
								if (((buffer[i * 2 + j] >> (7 - k)) & 0x1) != NULL)
									putpixel(x + 8 * j * dy + k * dy + m, y + i * dx + n, color);
			s += 2;					 //一个汉字内码占用两个字节
			x += 12 * dx + distance; //字间间隔
		}
		delay(speed); //加入延时，可以让汉字显示产生动画
	}
	fclose(fp);
}

void PutHZ16(int x, int y, char *s, int color, int dx, int dy, int distance, char speed) // 16*16点阵汉字的显示
{
	unsigned long offset;
	FILE *fp;
	char buffer[32], word[2] = {'\0', '\0'}; //buffer用来存储一个汉字
	int m, n, i, j, k;
	unsigned char qh, wh;

	if ((fp = fopen(".\\resource\\hzk\\hzk16", "rb")) == NULL)
	{
		printf("Can't open hzk16,Please add it");
		exit(1);
	}
	while (*s)
	{
		if ((int)(*s) > 0 && (int)(*s) < 128)
		{
			setcolor(color);
			word[0] = *s;
			PutAsc(x, y, word, color, dx, dx);
			s += 1;
			x += 9 * dx;
		}
		else
		{
			qh = *(s)-0xa0; //汉字区位码
			wh = *(s + 1) - 0xa0;
			offset = (94 * (qh - 1) + (wh - 1)) * 32L; //计算该汉字在字库中偏移量
			fseek(fp, offset, SEEK_SET);
			fread(buffer, 32, 1, fp); //取出汉字32字节的点阵字模存入buffer中（一个汉字）
			for (i = 0; i < 16; i++)  //将32位字节的点阵按位在屏幕上打印出来(1:打印,0:不打印),显示汉字
				for (n = 0; n < dx; n++)
					for (j = 0; j < 2; j++)
						for (k = 0; k < 8; k++)
							for (m = 0; m < dy; m++)
								if (((buffer[i * 2 + j] >> (7 - k)) & 0x1) != NULL)
									putpixel(x + 8 * j * dy + k * dy + m, y + i * dx + n, color);

			s += 2;					 //一个汉字内码占用两个字节
			x += 16 * dx + distance; //字间间隔
		}
		delay(speed); //加入延时，可以让汉字显示产生动画
	}
	fclose(fp);
}

void PutHZ24(int x, int y, char *s, int color, int size, int distance, char fillshape, char speed)
{
	FILE *fp;
	unsigned char buffer[72], qu, wei, word[2] = {'\0', '\0'}; //存取出的字模，区码，位码
	int i, j, m, n;
	long int offset;

	if ((fp = fopen(".\\resource\\hzk\\Hzk24", "rb")) == NULL) //文件读取检测
	{
		puts("cannot open file hzk24");
		delay(1000);
		exit(1);
	}
	while (*s)
	{
		if ((int)(*s) > 0 && (int)(*s) < 128)
		{
			setcolor(color);
			word[0] = *s;
			PutAsc(x, y - 1, word, color, 2, 2);
			s += 1;
			x += 15;
		}
		else
		{
			qu = *(s++) - 0xa0;
			wei = *(s++) - 0xa0;						//计算区位码
			offset = (94 * (qu - 1) + (wei - 1)) * 72L; //计算文件位置
			fseek(fp, offset, SEEK_SET);				//文件定位
			fread(buffer, 1, 72, fp);
			for (j = 0; j < 24; j++)
				for (i = 0; i < 24; i++)						   //扫描字体点阵
					if (buffer[3 * i + j / 8] & (0x80 >> (j % 8))) //判断该点是否有色
						for (m = 0; m < size; m++)
							for (n = 0; n < size; n++)										//把一个像素映射成size*size的正方形
								if ((x + size * j + m + y + size * i + n) % 3 || fillshape) //让字体有斜条纹效果
									putpixel(x + size * j + m, y + size * i + n, color);
			//else
			//	putpixel(x+size*j+m,y+size*i+n,RED);
			x += 24 * size + distance;
		}
		delay(speed); //加入延时，可以让汉字显示产生动画
	}
	fclose(fp);
}