#ifndef _hanzi_h_
#define _hanzi_h_

#include <dos.h>
#include <stdio.h>
#include <process.h>

void PutHZ12(int x, int y, char *s, int color, int dx, int dy, int distance, char speed);            // 12*12�����ֵ���ʾ
void PutHZ16(int x, int y, char *s, int color, int dx, int dy, int distance, char speed);            // 16*16�����ֵ���ʾ
void PutHZ24(int x, int y, char *s, int color, int size, int distance, char fillshape, char speed); //24*24�����ֵ���ʾ
void PutAsc(int cx, int cy, char *s, char color, int xsize, int ysize);                              //ָ����С���ַ�

#endif