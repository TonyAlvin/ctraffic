#ifndef __draw_h_
#define __draw_h_

#include "all.h"
void DrawRoad(void);                                                             //画路
void DrawCar(CAR *car);                                                          //画车
void DrawTrafficLight(int x, int y, unsigned int direction, unsigned int color); //画交通灯
void DrawMenu(void);                                                             //画菜单栏
void ButtonRefresh(void);
// void DrawButton(BT *bt);
// void ChangeButton(void);

#endif