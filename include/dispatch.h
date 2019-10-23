#ifndef _dispatch_h_
#define _dispatch_h_

void InitCar(CAR *newcar);                             //初始化车辆
void CreatCarList(CAR **head, int n);                  //创建车辆链表
void CarListDispatch(CAR *car, CAR *turn, int CarNum); //车辆链表整体调度
void InitCarNum(int *num);                             //初始化车辆数量调整滑块
void SetCarNum(int *num);                              //调节车辆生成数量（相对）

#endif