#ifndef __draw_h_
#define __draw_h_

#include "all.h"
void DrawRoad(void);                                                             //��·
void DrawCar(CAR *car);                                                          //����
void DrawTrafficLight(int x, int y, unsigned int direction, unsigned int color); //����ͨ��
void DrawMenu(void);                                                             //���˵���
void ButtonRefresh(void);
// void DrawButton(BT *bt);
// void ChangeButton(void);

#endif