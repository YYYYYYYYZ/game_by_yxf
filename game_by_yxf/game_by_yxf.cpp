#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

int control();
int snack_move(int temp_x, int temp_y, char* ch, struct Coordinate* c);
void edging_title();
void move(int x, int y);
void check(int x, int y);

int  goal, speed, lo, wi, eat, gameover_eat;
struct Coordinate                                                      //存储x,y
{
	int x;
	int y;
}*c;

int main()                                                             //界面
{
	printf("输入'o'开始游戏,其他键结束游戏\n");
	while (_getch() == 'o')
	{
		eat = 0, gameover_eat = 0;
		system("cls");
		printf("游戏以wsad操作,按'p'键可结束游戏\n请输入边界大小(长,宽)、速度、目标\n");
		system("pause");
		system("cls");
		printf("长(建议50左右):");
		scanf_s("%d", &lo);
		printf("宽(建议30左右):");
		scanf_s("%d", &wi);
		printf("请输入速度(应为整数2左右):");
		scanf_s("%d", &speed);
		printf("请输入目标(自然数):");
		scanf_s("%d", &goal);
		system("cls");
		printf("按任意键开始游戏");
		switch (control())
		{
		case -1:system("cls");
			printf("游戏结束-失败qwq\n目标为:%d\n最终成绩为:%d\n", goal, gameover_eat);
			break;
		default:system("cls");
			printf("游戏结束-胜利ovo\n目标为:%d\n最终成绩为:%d\n", goal, eat);
			break;
		}
		printf("输入'o'重新开始游戏,其他键结束游戏\n");
	}
}

void move(int x, int y)                                                  //移动
{
	COORD  point = { (SHORT)x ,  (SHORT)y };                             //光标要设置的位置x,y
	HANDLE  HOutput = GetStdHandle(STD_OUTPUT_HANDLE);                   //使用GetStdHandle(STD_OUTPUT_HANDLE)来获取标准输出的句柄
	SetConsoleCursorPosition(HOutput, point);                            //设置光标位置
}

int control()                                                            //蛇生成和移动
{
	Coordinate* c = (struct Coordinate*)calloc(goal + 3, 2 * sizeof(int));//动态生成结构体数组，为蛇的身躯
	(*c).x = lo / 2 + rand() % 3 + 1;
	(*c).y = wi / 2 + rand() % 3 + 1;
	(*(c + 1)).x = (*c).x + 1;
	(*(c + 2)).x = (*c).x + 2;
	char temp = 'w', temp_ch = _getch();
	char* ch = &temp_ch;
	int eat_1 = 0;
	while (1)                                                            //前后左右操作
	{
		if (eat == -1 || eat_1 == -1)
		{
			return -1;
		}
		if (eat == goal)
		{
			return 0;
		}
		switch (*ch)
		{
		case 'w': temp = 'w';
			edging_title();                                              //边界输出和计分板输出
			check((*c).x, (*c).y);                                       //吃的数量赋值和食物生成及检测和边界检测
			eat_1 = snack_move(0, -1, ch, c);
			break;
		case 's': temp = 's';
			edging_title();
			check((*c).x, (*c).y);
			eat_1 = snack_move(0, 1, ch, c);
			break;
		case 'a':  temp = 'a';
			edging_title();
			check((*c).x, (*c).y);
			eat_1 = snack_move(-1, 0, ch, c);
			break;
		case 'd': temp = 'd';
			edging_title();
			check((*c).x, (*c).y);
			eat_1 = snack_move(1, 0, ch, c);
			break;
		case 'p':gameover_eat = eat;
			return -1;
		default:
			*ch = temp;
			break;
		}
	}
	free(c);
}

void edging_title()                                                      //边界输出和计分板输出
{
	int l, w;
	for (w = 0; w < wi; w++)
	{
		if (w == 0 || w == wi - 1)
		{
			for (l = 0; l < lo; l++)
			{
				printf("*");
			}
			printf("\n");
		}
		else
		{
			printf("*");
			for (l = 0; l < lo - 2; l++)
			{
				printf(" ");
			}
			printf("*\n");
		}
	}
	move(lo + 1, 0);
	printf("goal:%d", goal);
	move(lo + 1, 1);
	printf("score:%d", eat);
}

void check(int x, int y)                                                 //食物生成及检测和边界检测
{
	static int x_food, y_food;                                           //防止被释放
	if (x >= lo - 1 || x <= 0 || y >= wi - 1 || y <= 0)                  //边缘检测
	{
		gameover_eat = eat;
		eat = -1;
	}
	if (eat == 0)
	{
		x_food = lo / 2;
		y_food = wi / 2;
		if (x == x_food && y == y_food)
		{
			srand((unsigned)time(NULL));                                 //设种子
			x_food = rand() % (lo - 3) + 2;
			y_food = rand() % (wi - 3) + 2;
			eat++;
		}
	}
	else  if (x == x_food && y == y_food)
	{
		srand((unsigned)time(NULL));                                     //设种子
		x_food = rand() % (lo - 3) + 2;
		y_food = rand() % (wi - 3) + 2;
		eat++;
	}
	move(x_food, y_food);
	printf("x");
}

int snack_move(int temp_x, int temp_y, char* ch, struct Coordinate* c)   //蛇移动
{
	for (int z = eat + 2; z > 0; z--)                                    //检测蛇有没有撞自己
	{
		if ((*c).x == (*(c + z)).x && (*c).y == (*(c + z)).y)
		{
			gameover_eat = eat;
			return -1;
		}
	}
	for (int i = eat + 2; i > 0; i--)
	{
		(*(c + i)).x = (*(c + i - 1)).x;
		(*(c + i)).y = (*(c + i - 1)).y;
	}
	(*c).x += temp_x;
	(*c).y += temp_y;
	for (int j = eat + 2; j >= 0; j--)
	{
		move((*(c + j)).x, (*(c + j)).y);
		printf("o");
	}
	move(0, 0);
	if (_kbhit())
	{
		*ch = _getch();
	}
	Sleep(speed * 50);
	system("cls");
	return 0;
}



