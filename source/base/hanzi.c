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
/* ��ʾӢ�ĺ����� */
void PutAsc(int cx, int cy, char *s, char color, int xsize, int ysize)
{
	int index;
	for (index = 0; s[index] != 0; index++)
	{
		PutAsc16Muliti(cx + index * xsize * 8, cy, xsize, ysize, s[index], color);
	}
}

void PutHZ12(int x, int y, char *s, int color, int dx, int dy, int distance, char speed) // 12*12�����ֵ���ʾ
{
	unsigned long offset;
	FILE *fp;
	char buffer[24], word[2] = {'\0', '\0'}; //buffer�����洢һ������
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
			qh = *(s)-0xa0; //������λ��
			wh = *(s + 1) - 0xa0;
			offset = (94 * (qh - 1) + (wh - 1)) * 24L; //����ú������ֿ���ƫ����
			fseek(fp, offset, SEEK_SET);
			fread(buffer, 24, 1, fp); //ȡ������32�ֽڵĵ�����ģ����buffer�У�һ�����֣�
			for (i = 0; i < 12; i++)  //��32λ�ֽڵĵ���λ����Ļ�ϴ�ӡ����(1:��ӡ,0:����ӡ),��ʾ����
				for (n = 0; n < dx; n++)
					for (j = 0; j < 2; j++)
						for (k = 0; k < 8; k++)
							for (m = 0; m < dy; m++)
								if (((buffer[i * 2 + j] >> (7 - k)) & 0x1) != NULL)
									putpixel(x + 8 * j * dy + k * dy + m, y + i * dx + n, color);
			s += 2;					 //һ����������ռ�������ֽ�
			x += 12 * dx + distance; //�ּ���
		}
		delay(speed); //������ʱ�������ú�����ʾ��������
	}
	fclose(fp);
}

void PutHZ16(int x, int y, char *s, int color, int dx, int dy, int distance, char speed) // 16*16�����ֵ���ʾ
{
	unsigned long offset;
	FILE *fp;
	char buffer[32], word[2] = {'\0', '\0'}; //buffer�����洢һ������
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
			qh = *(s)-0xa0; //������λ��
			wh = *(s + 1) - 0xa0;
			offset = (94 * (qh - 1) + (wh - 1)) * 32L; //����ú������ֿ���ƫ����
			fseek(fp, offset, SEEK_SET);
			fread(buffer, 32, 1, fp); //ȡ������32�ֽڵĵ�����ģ����buffer�У�һ�����֣�
			for (i = 0; i < 16; i++)  //��32λ�ֽڵĵ���λ����Ļ�ϴ�ӡ����(1:��ӡ,0:����ӡ),��ʾ����
				for (n = 0; n < dx; n++)
					for (j = 0; j < 2; j++)
						for (k = 0; k < 8; k++)
							for (m = 0; m < dy; m++)
								if (((buffer[i * 2 + j] >> (7 - k)) & 0x1) != NULL)
									putpixel(x + 8 * j * dy + k * dy + m, y + i * dx + n, color);

			s += 2;					 //һ����������ռ�������ֽ�
			x += 16 * dx + distance; //�ּ���
		}
		delay(speed); //������ʱ�������ú�����ʾ��������
	}
	fclose(fp);
}

void PutHZ24(int x, int y, char *s, int color, int size, int distance, char fillshape, char speed)
{
	FILE *fp;
	unsigned char buffer[72], qu, wei, word[2] = {'\0', '\0'}; //��ȡ������ģ�����룬λ��
	int i, j, m, n;
	long int offset;

	if ((fp = fopen(".\\resource\\hzk\\Hzk24", "rb")) == NULL) //�ļ���ȡ���
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
			wei = *(s++) - 0xa0;						//������λ��
			offset = (94 * (qu - 1) + (wei - 1)) * 72L; //�����ļ�λ��
			fseek(fp, offset, SEEK_SET);				//�ļ���λ
			fread(buffer, 1, 72, fp);
			for (j = 0; j < 24; j++)
				for (i = 0; i < 24; i++)						   //ɨ���������
					if (buffer[3 * i + j / 8] & (0x80 >> (j % 8))) //�жϸõ��Ƿ���ɫ
						for (m = 0; m < size; m++)
							for (n = 0; n < size; n++)										//��һ������ӳ���size*size��������
								if ((x + size * j + m + y + size * i + n) % 3 || fillshape) //��������б����Ч��
									putpixel(x + size * j + m, y + size * i + n, color);
			//else
			//	putpixel(x+size*j+m,y+size*i+n,RED);
			x += 24 * size + distance;
		}
		delay(speed); //������ʱ�������ú�����ʾ��������
	}
	fclose(fp);
}