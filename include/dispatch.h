#ifndef _dispatch_h_
#define _dispatch_h_

void InitCar(CAR *newcar);                             //��ʼ������
void CreatCarList(CAR **head, int n);                  //������������
void CarListDispatch(CAR *car, CAR *turn, int CarNum); //���������������
void InitCarNum(int *num);                             //��ʼ������������������
void SetCarNum(int *num);                              //���ڳ���������������ԣ�

#endif