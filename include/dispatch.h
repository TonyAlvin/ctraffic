#ifndef _dispatch_h_
#define _dispatch_h_

void InitCar(CAR *newcar);
void CreatCarList(CAR **head, int n);
void CarListDispatch(CAR *car, CAR *turn);
void TurnLeftCar(CAR *car);
int GetLightStatusT(int just, int turn, int x, int y);

#endif