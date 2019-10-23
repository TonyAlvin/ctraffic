#include <stdio.h>
#include <math.h>
#include "draw.h"
#include <graphics.h>
#include "hanzi.h"
#include "mouse.h"
#include "BtFeatures.h"
#include "all.h"

void RefreshRoad(void)
{
     //画路面
     setfillstyle(0, BLACK);
     bar(0, 121, 1024, 144);
     bar(0, 146, 1024, 169);
     bar(0, 171, 1024, 194);
     bar(0, 196, 1024, 219);
     bar(0, 521, 1024, 544);
     bar(0, 546, 1024, 569);
     bar(0, 571, 1024, 594);
     bar(0, 596, 1024, 619);
     bar(201, 25, 224, 767);
     bar(226, 25, 249, 767);
     bar(251, 20, 274, 767);
     bar(276, 20, 299, 767);
     bar(701, 219, 724, 767);
     bar(726, 219, 749, 767);
     bar(751, 219, 774, 767);
     bar(776, 219, 799, 767);
}

void DrawRoad(void)
{
     //画路面
     setfillstyle(0, BLACK);
     bar(0, 121, 1024, 219);
     bar(0, 521, 1024, 619);
     bar(201, 20, 299, 767);
     bar(701, 219, 799, 767);
     //画路中黄实线
     setcolor(YELLOW);
     line(1, 170, 200, 170);
     line(300, 170, 700, 170);
     line(800, 170, 1024, 170);
     line(1, 570, 200, 570);
     line(300, 570, 700, 570);
     line(800, 570, 1024, 570);
     line(250, 20, 250, 120);
     line(250, 220, 250, 520);
     line(250, 620, 250, 767);
     line(750, 220, 750, 520);
     line(750, 620, 750, 767);
     //画路边缘线
     setlinestyle(0, 0, 1);
     setcolor(WHITE);
     line(1, 120, 200, 120);
     line(300, 120, 1023, 120);
     line(1, 220, 200, 220);
     line(300, 220, 700, 220);
     line(800, 220, 1023, 220);
     line(1, 520, 200, 520);
     line(300, 520, 700, 520);
     line(800, 520, 1023, 520);
     line(1, 620, 200, 620);
     line(300, 620, 700, 620);
     line(800, 620, 1023, 620);
     line(200, 21, 200, 120);
     line(300, 21, 300, 120);
     //画白实线
     line(200, 220, 200, 520);
     line(300, 220, 300, 520);
     line(700, 220, 700, 520);
     line(800, 220, 800, 520);
     line(200, 620, 200, 767);
     line(300, 620, 300, 767);
     line(700, 620, 700, 767);
     line(800, 620, 800, 767);
     line(120, 195, 200, 195);
     line(200, 170, 200, 220);
     line(620, 195, 700, 195);
     line(700, 170, 700, 220);
     line(300, 145, 380, 145);
     line(300, 120, 300, 170);
     line(800, 145, 880, 145);
     line(800, 120, 800, 170);
     line(120, 595, 200, 595);
     line(200, 570, 200, 620);
     line(300, 520, 300, 570);
     line(300, 545, 380, 545);
     line(620, 595, 700, 595);
     line(700, 570, 700, 620);
     line(800, 545, 880, 545);
     line(800, 520, 800, 570);
     line(225, 40, 225, 120);
     line(200, 120, 250, 120);
     line(275, 220, 275, 300);
     line(250, 220, 300, 220);
     line(225, 440, 225, 520);
     line(200, 520, 250, 520);
     line(275, 620, 275, 700);
     line(250, 620, 300, 620);
     line(775, 220, 775, 300);
     line(750, 220, 800, 220);
     line(725, 440, 725, 520);
     line(775, 620, 775, 700);
     line(750, 620, 800, 620);
     line(700, 520, 750, 520);
     //白虚线
     setlinestyle(3, 0, 1);
     line(1, 145, 200, 145);
     line(1, 195, 120, 195);
     line(380, 145, 700, 145);
     line(300, 195, 620, 195);
     line(800, 195, 1024, 195);
     line(880, 145, 1024, 145);
     line(1, 545, 200, 545);
     line(1, 595, 120, 595);
     line(380, 545, 700, 545);
     line(300, 595, 620, 595);
     line(800, 595, 1024, 595);
     line(880, 545, 1024, 545);
     line(275, 20, 275, 120);
     line(225, 20, 225, 40);
     line(225, 220, 225, 440);
     line(275, 300, 275, 520);
     line(275, 700, 275, 767);
     line(225, 620, 225, 767);
     line(725, 220, 725, 440);
     line(775, 300, 775, 520);
     line(775, 700, 775, 767);
     line(725, 620, 725, 767);
}

/**********************************************
 * 函数名：DrawCar(void)
 * 功能：画出车辆，（可旋转）
 * 返回值：空
 * *******************************************/
void DrawCar(CAR *car)
{
     int i;
     int x = car->x, y = car->y;
     int color = car->color;
     double a = (car->angle) * PI / 180;
     int tou[16], wei[16], ch[8], ding[8], d1[6], d2[6], d3[6], d4[6];
     if (car->color == 0)
          return;
     //车头
     tou[0] = x + A * 8.6 * cos(54.5 * PI / 180 + a);
     tou[1] = y - A * 8.6 * sin(54.5 * PI / 180 + a);
     tou[2] = x + A * 10.3 * cos(60.9 * PI / 180 + a);
     tou[3] = y - A * 10.3 * sin(60.9 * PI / 180 + a);
     tou[4] = x + A * 9.49 * cos(71.5 * PI / 180 + a);
     tou[5] = y - A * 9.49 * sin(71.5 * PI / 180 + a);
     tou[6] = x + A * 12.37 * cos(75.95 * PI / 180 + a);
     tou[7] = y - A * 12.37 * sin(75.95 * PI / 180 + a);
     tou[8] = x - A * 12.37 * cos(75.95 * PI / 180 - a);
     tou[9] = y - A * 12.37 * sin(75.95 * PI / 180 - a);
     tou[10] = x - A * 9.49 * cos(71.5 * PI / 180 - a);
     tou[11] = y - A * 9.49 * sin(71.5 * PI / 180 - a);
     tou[12] = x - A * 10.3 * cos(60.9 * PI / 180 - a);
     tou[13] = y - A * 10.3 * sin(60.9 * PI / 180 - a);
     tou[14] = x - A * 8.6 * cos(54.5 * PI / 180 - a);
     tou[15] = y - A * 8.6 * sin(54.5 * PI / 180 - a);

     //车尾
     wei[0] = x + A * 11.18 * cos(63.44 * PI / 180 - a);
     wei[1] = y + A * 11.18 * sin(63.44 * PI / 180 - a);
     wei[2] = x + A * 12.08 * cos(65.556 * PI / 180 - a);
     wei[3] = y + A * 12.08 * sin(65.556 * PI / 180 - a);
     wei[4] = x + A * 11.4 * cos(74.777 * PI / 180 - a);
     wei[5] = y + A * 11.4 * sin(74.777 * PI / 180 - a);
     wei[6] = x + A * 13.34 * cos(77.036 * PI / 180 - a);
     wei[7] = y + A * 13.34 * sin(77.036 * PI / 180 - a);
     wei[8] = x - A * 13.34 * cos(77.036 * PI / 180 + a);
     wei[9] = y + A * 13.34 * sin(77.036 * PI / 180 + a);
     wei[10] = x - A * 11.4 * cos(74.777 * PI / 180 + a);
     wei[11] = y + A * 11.4 * sin(74.777 * PI / 180 + a);
     wei[12] = x - A * 12.08 * cos(65.556 * PI / 180 + a);
     wei[13] = y + A * 12.08 * sin(65.556 * PI / 180 + a);
     wei[14] = x - A * 11.18 * cos(63.44 * PI / 180 + a);
     wei[15] = y + A * 11.18 * sin(63.44 * PI / 180 + a);

     //车窗矩形
     ch[0] = x + A * 8.6 * cos(54.5 * PI / 180 + a);
     ch[1] = y - A * 8.6 * sin(54.5 * PI / 180 + a);
     ch[2] = x - A * 8.6 * cos(54.5 * PI / 180 - a);
     ch[3] = y - A * 8.6 * sin(54.5 * PI / 180 - a);
     ch[4] = x - A * 11.18 * cos(63.44 * PI / 180 + a);
     ch[5] = y + A * 11.18 * sin(63.44 * PI / 180 + a);
     ch[6] = x + A * 11.18 * cos(63.44 * PI / 180 - a);
     ch[7] = y + A * 11.18 * sin(63.44 * PI / 180 - a);

     //车顶矩形
     ding[0] = x + A * 3.606 * cos(33.69 * PI / 180 + a);
     ding[1] = y - A * 3.606 * sin(33.69 * PI / 180 + a);
     ding[2] = x - A * 3.606 * cos(33.69 * PI / 180 - a);
     ding[3] = y - A * 3.606 * sin(33.69 * PI / 180 - a);
     ding[4] = x - A * 8.544 * cos(69.444 * PI / 180 + a);
     ding[5] = y + A * 8.544 * sin(69.444 * PI / 180 + a);
     ding[6] = x + A * 8.544 * cos(69.444 * PI / 180 - a);
     ding[7] = y + A * 8.544 * sin(69.444 * PI / 180 - a);

     //前车灯三角形
     d1[0] = x + A * 10.3 * cos(60.9 * PI / 180 + a);
     d1[1] = y - A * 10.3 * sin(60.9 * PI / 180 + a);
     d1[2] = x + A * 9.49 * cos(71.5 * PI / 180 + a);
     d1[3] = y - A * 9.49 * sin(71.5 * PI / 180 + a);
     d1[4] = x + A * 12.37 * cos(75.95 * PI / 180 + a);
     d1[5] = y - A * 12.37 * sin(75.95 * PI / 180 + a);

     //前车灯三角形
     d2[0] = x - A * 12.37 * cos(75.95 * PI / 180 - a);
     d2[1] = y - A * 12.37 * sin(75.95 * PI / 180 - a);
     d2[2] = x - A * 9.49 * cos(71.5 * PI / 180 - a);
     d2[3] = y - A * 9.49 * sin(71.5 * PI / 180 - a);
     d2[4] = x - A * 10.3 * cos(60.9 * PI / 180 - a);
     d2[5] = y - A * 10.3 * sin(60.9 * PI / 180 - a);

     //后车灯三角形
     d3[0] = x + A * 12.08 * cos(65.556 * PI / 180 - a);
     d3[1] = y + A * 12.08 * sin(65.556 * PI / 180 - a);
     d3[2] = x + A * 11.4 * cos(74.777 * PI / 180 - a);
     d3[3] = y + A * 11.4 * sin(74.777 * PI / 180 - a);
     d3[4] = x + A * 13.34 * cos(77.036 * PI / 180 - a);
     d3[5] = y + A * 13.34 * sin(77.036 * PI / 180 - a);

     //后车灯三角形
     d4[0] = x - A * 13.34 * cos(77.036 * PI / 180 + a);
     d4[1] = y + A * 13.34 * sin(77.036 * PI / 180 + a);
     d4[2] = x - A * 11.4 * cos(74.777 * PI / 180 + a);
     d4[3] = y + A * 11.4 * sin(74.777 * PI / 180 + a);
     d4[4] = x - A * 12.08 * cos(65.556 * PI / 180 + a);
     d4[5] = y + A * 12.08 * sin(65.556 * PI / 180 + a);

     setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
     setcolor(color);
     setfillstyle(1, WHITE); //画车灯
     fillpoly(3, d1);
     fillpoly(3, d2);
     fillpoly(3, d3);
     fillpoly(3, d4);
     setfillstyle(1, LIGHTBLUE); //画车窗
     fillpoly(4, ch);
     setfillstyle(1, color); //画车顶，车头和车尾，车顶必须在车窗之后画
     fillpoly(4, ding);
     fillpoly(8, tou);
     fillpoly(8, wei);

     //在车窗上画轮廓线，把车窗分为四个
     line(x + A * 3.606 * cos(33.69 * PI / 180 + a), y - A * 3.606 * sin(33.69 * PI / 180 + a),
          x + A * 8.6 * cos(54.5 * PI / 180 + a), y - A * 8.6 * sin(54.5 * PI / 180 + a));
     line(x - A * 3.606 * cos(33.69 * PI / 180 - a), y - A * 3.606 * sin(33.69 * PI / 180 - a),
          x - A * 8.6 * cos(54.5 * PI / 180 - a), y - A * 8.6 * sin(54.5 * PI / 180 - a));
     line(x + A * 4.243 * cos(45 * PI / 180 - a), y + A * 4.243 * sin(45 * PI / 180 - a),
          x + A * 5.83 * cos(30.97 * PI / 180 - a), y + A * 5.83 * sin(30.97 * PI / 180 - a));
     line(x - A * 4.243 * cos(45 * PI / 180 + a), y + A * 4.243 * sin(45 * PI / 180 + a),
          x - A * 5.83 * cos(30.97 * PI / 180 + a), y + A * 5.83 * sin(30.97 * PI / 180 + a));
     line(x + A * 8.544 * cos(69.444 * PI / 180 - a), y + A * 8.544 * sin(69.444 * PI / 180 - a),
          x + A * 11.18 * cos(63.44 * PI / 180 - a), y + A * 11.18 * sin(63.44 * PI / 180 - a));
     line(x - A * 8.544 * cos(69.444 * PI / 180 + a), y + A * 8.544 * sin(69.444 * PI / 180 + a),
          x - A * 11.18 * cos(63.44 * PI / 180 + a), y + A * 11.18 * sin(63.44 * PI / 180 + a));
}

/*
作用：画转向灯函数
void DrawTrafficLight(int x, int y,unsigned int direction, unsigned int color)
					中心坐标                  方向               颜色
调用：无
返回：无*/
void DrawTrafficLight(int x, int y, unsigned int direction, unsigned int color)
{
     int i;
     setfillstyle(1, DARKGRAY);
     bar(x - 9, y - 9, x + 9, y + 9);
     switch (direction)
     {
     case 1:
          setfillstyle(1, color);
          bar(x - 3, y - 1, x + 2, y + 6);
          setlinestyle(0, 0, 1);
          setcolor(color);
          for (i = 6; i != 0; i--)
               line(x - i, y - 7 + i, x + i, y - 7 + i);
          break;
     case 2:
          setfillstyle(1, color);
          bar(x - 3, y - 6, x + 3, y + 1);
          setlinestyle(0, 0, 1);
          setcolor(color);
          for (i = 6; i != 0; i--)
               line(x - i, y + 7 - i, x + i, y + 7 - i);
          break;
     case 3:
          setfillstyle(1, color);
          bar(x - 1, y - 2, x + 6, y + 3);
          setlinestyle(0, 0, 1);
          setcolor(color);
          for (i = 6; i != 0; i--)
               line(x - i, y + 7 - i, x - i, y - 6 + i);
          break;
     case 4:
          setfillstyle(1, color);
          bar(x - 6, y - 2, x + 1, y + 3);
          setlinestyle(0, 0, 1);
          setcolor(color);
          for (i = 6; i != 0; i--)
               line(x + i, y + 7 - i, x + i, y - 6 + i);
          break;
     default:
          break;
     }
     return;
}

/***********画通用menu界面的函数***********/
void draw_bk(void)
{
     setlinestyle(0, 0, 1);
     setbkcolor(BLACK);
     //界面名行
     setfillstyle(1, DARKGRAY);
     bar(STARTX + 2, STARTY + 2, ENDX - 2, STARTY + 29);
     setfillstyle(1, LIGHTGRAY);
     bar(STARTX + 2, STARTY, ENDX - 2, STARTY + 24);
}

void draw_btlight(const BT *bt, int state)
{
     setlinestyle(0, 0, 1);
     if (state == 0) //鼠标不在按钮上
     {
          setcolor(WHITE);
          line(bt->btx1 - BTDX, bt->bty1 - BTDY, bt->btx1 - BTDX, bt->bty2 + BTDY);
          line(bt->btx1 - BTDX, bt->bty1 - BTDY, bt->btx2 + BTDX, bt->bty1 - BTDY);
          setcolor(BLACK);
          line(bt->btx2 + BTDX, bt->bty1 - BTDY, bt->btx2 + BTDX, bt->bty2 + BTDY);
          line(bt->btx1 - BTDX, bt->bty2 + BTDY, bt->btx2 + BTDX, bt->bty2 + BTDY);
     }
     if (state == 1) //鼠标在按钮上但没按下
     {
          setcolor(BLACK);
          line(bt->btx1 - BTDX, bt->bty1 - BTDY, bt->btx2 + BTDX, bt->bty1 - BTDY);
          line(bt->btx1 - BTDX, bt->bty2 + BTDY, bt->btx2 + BTDX, bt->bty2 + BTDY);
          line(bt->btx1 - BTDX, bt->bty1 - BTDY, bt->btx1 - BTDX, bt->bty2 + BTDY);
          line(bt->btx2 + BTDX, bt->bty1 - BTDY, bt->btx2 + BTDX, bt->bty2 + BTDY);
     }
     if (state == 2) //鼠标在按钮上且按下
     {
          setcolor(BLACK);
          line(bt->btx1 - BTDX, bt->bty1 - BTDY, bt->btx1 - BTDX, bt->bty2 + BTDY);
          line(bt->btx1 - BTDX, bt->bty1 - BTDY, bt->btx2 + BTDX, bt->bty1 - BTDY);
          setcolor(WHITE);
          line(bt->btx2 + BTDX, bt->bty1 - BTDY, bt->btx2 + BTDX, bt->bty2 + BTDY);
          line(bt->btx1 - BTDX, bt->bty2 + BTDY, bt->btx2 + BTDX, bt->bty2 + BTDY);
          (bt->feature)();
     }
}

void draw_bt(const BT *bt, int color)
{
     PutHZ12(bt->btx1, bt->bty1, bt->btname, color, 1, 1, 2, 0);
     draw_btlight(bt, MouseOnBT(bt));
}

void DrawMenu(void)
{
     draw_bk();
     // draw_bt(&bt_czsm, DARKGRAY);
     draw_bt(&bt_gnjs, DARKGRAY);
     draw_bt(&bt_about, DARKGRAY);
     draw_bt(&bt_exit, DARKGRAY);
}

void ButtonRefresh(void)
{
     // draw_bt(&bt_czsm, DARKGRAY);
     draw_bt(&bt_gnjs, DARKGRAY);
     draw_bt(&bt_about, DARKGRAY);
     draw_bt(&bt_exit, DARKGRAY);
}
