#include <graphics.h>
#include <time.h>
#include <stdlib.h>
#include "draw.h"
#include "all.h"

void DrawLightC(int x, int y, int n)
{
    switch (n % 4 + 1)
    {
    case 1:
        DrawTrafficLight(x + 12, y - 58, 3, RED);
        DrawTrafficLight(x + 38, y - 58, 1, RED);
        DrawTrafficLight(x - 58, y - 38, 3, RED);
        DrawTrafficLight(x - 58, y - 12, 2, GREEN);
        DrawTrafficLight(x + 58, y + 12, 1, GREEN);
        DrawTrafficLight(x + 58, y + 38, 4, RED);
        DrawTrafficLight(x - 38, y + 58, 2, RED);
        DrawTrafficLight(x - 12, y + 58, 4, RED);
        break;
    case 2:
        DrawTrafficLight(x + 12, y - 58, 3, RED);
        DrawTrafficLight(x + 38, y - 58, 1, GREEN);
        DrawTrafficLight(x - 58, y - 38, 3, RED);
        DrawTrafficLight(x - 58, y - 12, 2, RED);
        DrawTrafficLight(x + 58, y + 12, 1, RED);
        DrawTrafficLight(x + 58, y + 38, 4, RED);
        DrawTrafficLight(x - 38, y + 58, 2, GREEN);
        DrawTrafficLight(x - 12, y + 58, 4, RED);
        break;
    case 3:
        DrawTrafficLight(x + 12, y - 58, 3, GREEN);
        DrawTrafficLight(x + 38, y - 58, 1, RED);
        DrawTrafficLight(x - 58, y - 38, 3, RED);
        DrawTrafficLight(x - 58, y - 12, 2, RED);
        DrawTrafficLight(x + 58, y + 12, 1, RED);
        DrawTrafficLight(x + 58, y + 38, 4, RED);
        DrawTrafficLight(x - 38, y + 58, 2, RED);
        DrawTrafficLight(x - 12, y + 58, 4, GREEN);
        break;
    case 4:
        DrawTrafficLight(x + 12, y - 58, 3, RED);
        DrawTrafficLight(x + 38, y - 58, 1, RED);
        DrawTrafficLight(x - 58, y - 38, 3, GREEN);
        DrawTrafficLight(x - 58, y - 12, 2, RED);
        DrawTrafficLight(x + 58, y + 12, 1, RED);
        DrawTrafficLight(x + 58, y + 38, 4, GREEN);
        DrawTrafficLight(x - 38, y + 58, 2, RED);
        DrawTrafficLight(x - 12, y + 58, 4, RED);
        break;
    }
}

void DrawLightT(int x, int y, int n)
{
    switch (n % 3 + 1)
    {
    case 1:
        DrawTrafficLight(x + 24, y - 58, 3, RED);
        DrawTrafficLight(x - 58, y - 38, 3, RED);
        DrawTrafficLight(x - 58, y - 12, 2, GREEN);
        DrawTrafficLight(x + 58, y + 24, 4, RED);
        break;
    case 2:
        DrawTrafficLight(x + 24, y - 58, 3, GREEN);
        DrawTrafficLight(x - 58, y - 38, 3, RED);
        DrawTrafficLight(x - 58, y - 12, 2, RED);
        DrawTrafficLight(x + 58, y + 24, 4, RED);
        break;
    case 3:
        DrawTrafficLight(x + 24, y - 58, 3, RED);
        DrawTrafficLight(x - 58, y - 38, 3, GREEN);
        DrawTrafficLight(x - 58, y - 12, 2, RED);
        DrawTrafficLight(x + 58, y + 24, 4, GREEN);
        break;
    }
}

void NormalControl(int n)
{
    static unsigned int origin = 0;
    unsigned int interval;
    if (origin == 0)
        origin = (unsigned int)time(NULL);
    interval = (unsigned int)time(NULL) - origin;
    DrawLightC(LEFT_X, UP_Y, interval / n);
    DrawLightT(RIGHT_X, UP_Y, (interval + 1) / n);
    DrawLightC(LEFT_X, DOWN_Y, (interval + 0) / n);
    DrawLightC(RIGHT_X, DOWN_Y, (interval + 3) / n);
}