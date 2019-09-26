#ifndef __all_h__
#define __all_h__

#include "BtFeatures.h"
/*************键盘操作宏定义******************/
/******功能键******/
#define F1 15104
#define F2 15360
#define F3 15616
#define F4 15872
#define ESC 283
#define ENTER 7181
#define START 14624 //BLANK空格  仿真时开始
#define PAUSE 14624 //BLANK too
#define REPLAY 4978 //r 键仿真时重播replay
#define BACK 3592   //backspace返回和删除
#define DELETE 21248
#define LEFT 19200
#define RIGHT 19712
#define TAB 3849
/******数字键******/
#define ZERO 2864
#define ONE 561
#define TWO 818
#define THREE 1075
#define FOUR 1332
#define FIVE 1589
#define SIX 1846
#define SEVEN 2103
#define EIGHT 2360
#define NINE 2617

/******Menu界面初始和结束坐标******/
#define STARTX 0
#define ENDX 1029
#define STARTY 0
#define ENDY 767

/*****Button DX, DY按钮边缘距离修正值*******/
#define BTDX 3
#define BTDY 2

/**************控制车的宏定义****************/
#define P 3.1415926 //圆周率
#define A 1.3       //车子的尺寸比例系数
#define D 13        //车子中心到车头车尾的绝对距离
#define WC 5        //停车线的邻域宽
#define RATE 3      //车子的速度比率基准
#define ALARMDIS 32 //车中心相距的警戒距离
#define DIS 37      //短警戒距离(*考虑到转角及车身长，这是最长的*)
#define DIL 74      //长警戒距离

/**********按钮结构***********/
struct BUTTON
{
    int btx1;     //首x坐标
    int bty1;     //首y坐标
    int btx2;     //尾x坐标
    int bty2;     //尾y坐标
    char *btname; //按钮名字
    char *btmsg;  //提示信息
    void (*feature)(void);
};
typedef struct BUTTON BT;
/***********输入框结构************/
struct INPUT_BOX
{
    int bx0;    //首x坐标
    int by0;    //首y坐标
    int blen;   //长
    int bwid;   //宽
    int bn[2];  //存储数据
    int bstate; //输入框状态
};
typedef struct INPUT_BOX BOX;

/***************车的结构****************/
struct CarStruct
{
    int x;         //车子中心当前x坐标
    int y;         //车子中心当前y坐标
    int color;     //车子颜色
    int cnum;      //车牌号
    int alarm;     //警戒距离警报
    int dirt1;     //车子所在的初始路向
    int dirt2;     //车子转向的路向
    int speed;     //车子当前速度
    int std_speed; //车子本来的速度
    int spflag;    //超速标志位
    int justment;  //首位为所在路，次位为所在道
    int count;     //在转弯时计算记步
    int flag;      //用于标志车子是否驶出桌面
    double angle;  //车子当前的角度，正北为零度，逆时针
};
typedef struct CarStruct CAR;

/*各按钮的定义**非全局变量不改变值**只是按钮信息的定义*/
static BT bt_czsm = {STARTX + 10, STARTY + 8, STARTX + 10 + 71, STARTY + 8 + 11, "操作说明F1", "关于本系统操作的介绍", BtIntro};
static BT bt_gnjs = {STARTX + 81 + 20, STARTY + 8, STARTX + 81 + 20 + 71, STARTY + 19, "功能介绍F2", "关于本系统功能的简介", BtIntro};
static BT bt_about = {STARTX + 172 + 20, STARTY + 8, STARTX + 172 + 20 + 71, STARTY + 19, "关于团队F3", "制作人员的简介", BtAbout};
static BT bt_exit = {ENDX - 33 - 36, STARTY + 8, ENDX - 10, STARTY + 19, "退出ESC", "按下将退出系统", BtExit};

#endif