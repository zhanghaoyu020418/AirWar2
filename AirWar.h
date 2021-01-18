#pragma once

#include "graphics.h"
#include <iostream>
#include "AirPlane.h"
#include <stdlib.h>
#include <time.h>
#include <conio.h>   
#include <Windows.h>
using namespace std;
IMAGE img[5];//IMAGE是专门存放图片的数组

Node MyPlane;                   //我方飞机的节点创建
LL EnemyPlane;                  //敌方飞机的链表创建
LL Bullet;                      //子弹链表的创建



//用宏定义来规定图片的大小
#define BGWIDTH 400               //背景宽度
#define BGHEIGHT 600              //背景高度
#define MyPlaneWIDYH 60           //我方飞机宽度
#define MyPlaneHEIGHT 80		  //我方飞机高度
#define EnemyPlaneWIDYH 50        //敌方飞机宽度
#define EnemyPlaneHEIGHT 40       //敌方飞机高度
#define BulletWIDYH 16            //子弹宽度
#define BulletHEIGHT 25           //子弹高度


//上传图片
void init()
{
	srand((unsigned)time(NULL));   //生成随机种子
	//将图片上传
	loadimage(&img[0], "res\\背景.jpg", BGWIDTH, BGHEIGHT);
	loadimage(&img[1], "res\\飞机.jpg", MyPlaneWIDYH, MyPlaneHEIGHT);
	loadimage(&img[2], "res\\敌机.jpg", EnemyPlaneWIDYH, EnemyPlaneHEIGHT);
	loadimage(&img[3], "res\\子弹.jpg", BulletWIDYH, BulletHEIGHT);

	MyPlane.x = BGWIDTH / 2 - MyPlaneWIDYH / 2;
	MyPlane.y = BGHEIGHT - MyPlaneHEIGHT - 30;
	MyPlane.speed = 10;
	MyPlane.next = NULL;

	Bullet.head = NULL;
	Bullet.head = NULL;

	EnemyPlane.head = NULL;
	EnemyPlane.rear = NULL;

}

//初始化界面函数
void DrawMap()
{
	BeginBatchDraw();              //开始批量绘制图片

	putimage(0, 0, &img[0]);       //在窗口绘制图片
	putimage(MyPlane.x, MyPlane.y, &img[1]);

	for (Node* temp1 = EnemyPlane.head; temp1; temp1 = temp1->next)
	{
		putimage(temp1->x, temp1->y, &img[2]);
	}
	

	for (Node* temp2 = Bullet.head; temp2; temp2 = temp2->next)
	{
		putimage(temp2->x, temp2->y, &img[3]);
		//遍历整个子弹链表
	}



	EndBatchDraw();                 //结束批量绘制图片
}

//子弹生成函数
void createbullet()
{
	linklist_insert(&Bullet,
		MyPlane.x + MyPlaneWIDYH / 2 - BulletWIDYH / 2,
		MyPlane.y - BulletHEIGHT,
		10);
}

//子弹移动函数
void move_Bullet()
{
//	Sleep(5);
	for (Node* temp = Bullet.head; temp; temp = temp->next)
	{
		temp->y -= temp->speed;     //遍历整个子弹的链表，将每个子弹节点的移动位置+speed
	}
}

//子弹的销毁函数
void Bullet_delete()
{
	for (Node* temp = Bullet.head; temp; temp = temp->next)
	{
		if (temp->y <= -BulletHEIGHT)
		{                            //遍历整个子弹链表，逐个释放掉
			linklist_delete(&Bullet, temp);
			return;                  //这里必须要return，因为temp已经被释放掉了，没有指向了      
		}
	}
}

//子弹的移动和销毁
void complex2()
{
	move_Bullet();              //实现子弹的移动
	Bullet_delete();            //实现子弹的释放   
}

//敌机的生成函数
void EnemyPlane_create()
{
	int num = 0;
	num = rand() % 1000;
	if(num % 125 == 0)
		linklist_insert(&EnemyPlane, 
			            rand() % (BGWIDTH - EnemyPlaneWIDYH) - 1, 
			            EnemyPlaneHEIGHT, 
			            1);
}

//敌机的移动函数
void move_EnemyPlane()
{
	Sleep(5);
	for (Node* temp = EnemyPlane.head; temp; temp = temp->next)
	{
		temp->y += temp->speed;
	}
}

//敌机的销毁函数
void EnemyPlane_delete()
{
	for (Node* temp = EnemyPlane.head; temp; temp = temp->next)
	{
		if (temp->y >= BGHEIGHT + EnemyPlaneHEIGHT)
		{
			linklist_delete(&EnemyPlane, temp);
			return;
		}
	}
}

//敌机的生成，移动，销毁
void complex1()
{
	EnemyPlane_create();
	move_EnemyPlane();
	EnemyPlane_delete();
}

//玩游戏函数
void playgame()
{
	if (_kbhit())                   //判断键盘是否按下       
									//必须要有这个判断这样就不会被_getch()卡住
	{
		switch (_getch())           //获取一个字符，但是不用回车，也不会回显字符
		{
		case 'W':
		case 'w':
		case 72:
			if (MyPlane.y >= 0)
				MyPlane.y -= MyPlane.speed;
			break;                  //判读输入的字符表示控制键，72是上的键码
		case 'S':
		case 's':
		case 80:
			if (MyPlane.y <= BGHEIGHT - MyPlaneHEIGHT + 20)
				MyPlane.y += MyPlane.speed;
			break;                  //判读输入的字符表示控制键，80是上的键码
		case 'D':
		case 'd':
		case 77:
			if (MyPlane.x <= BGWIDTH - MyPlaneWIDYH - 5)
				MyPlane.x += MyPlane.speed;
			break;                  //判读输入的字符表示控制键，77是右的键码
		case 'a':
		case 'A':
		case 75:
			if (MyPlane.x >= 5)
				MyPlane.x -= MyPlane.speed;
			break;                  //判读输入的字符表示控制键，75是左的键码
		case ' ':
			createbullet();
		}
	}
}

//判断如果子弹打中敌机，则子弹和敌机都销毁
void hack()
{
	for (Node* temp1 = Bullet.head; temp1; temp1 = temp1->next)
	{
		for (Node* temp2 = EnemyPlane.head; temp2; temp2 = temp2->next)
		{
			if (temp1->y == temp2->y + EnemyPlaneHEIGHT && (temp1->x - (temp2->x - BulletWIDYH))>0)
			{
				linklist_delete(&Bullet, temp1);
				linklist_delete(&EnemyPlane, temp2);
				return;
			}
		}
	}
}


//开始游戏函数
void start()
{
	initgraph(BGWIDTH, BGHEIGHT);   //初始化窗口

	init();                         //加载图片，飞机坐标的初始化
	while (1)
	{

		DrawMap();                  //在窗口贴图，画好地图
		playgame();                 //从键盘上读入移动信息，开始玩游戏
		complex2();                 //子弹的移动，销毁
		complex1();                 //敌机的生成，移动，销毁
		hack();
	}

	//	getchar();                       //使窗口停下来
	closegraph();                    //关闭窗口
}