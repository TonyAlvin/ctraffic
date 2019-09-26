#ifndef _hanzi_h_
#define _hanzi_h_

#include <dos.h>
#include <stdio.h>
#include <process.h>

void PutHZ12(int x, int y, char *s, int color, int dx, int dy, int distance, char speed);            // 12*12点阵汉字的显示
void PutHZ16(int x, int y, char *s, int color, int dx, int dy, int distance, char speed);            // 16*16点阵汉字的显示
void PutHZ24(int x, int y, char *s, int color, int size, int distance, char fillshape, char speed); //24*24点阵汉字的显示
void PutAsc(int cx, int cy, char *s, char color, int xsize, int ysize);                              //指定大小的字符

#endif