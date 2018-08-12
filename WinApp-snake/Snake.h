#pragma once
#include <windows.h>

#define SNAKE_DIR_UP 1
#define SNAKE_DIR_RIGHT 2
#define SNAKE_DIR_DOWN 3
#define SNAKE_DIR_LEFT 4

#define FOOD_NORMAL 0
#define FOOD_SPEEDUP 1
#define FOOD_SLOWDOWN 2
#define FOOD_RESVER 3
#define FOOD_DBSCORE 4
#define FOOD_HEALTHY 5


#define ID_TIMER_MOVE 1100
#define ID_TIMER_SHOW 1101

#define BEG_SPEED 100

#define TSPE_RESET 4000
#define TIME_SPEEDUP 2000
#define TIME_SLOWDOWN 1500
#define TIME_REVERSER 1000

class snake {
private:
	HWND hPreWnd;			//Handle of the main window

	HPEN hp;				//handle of the pen to draw the line of the snake
	HDC hdc;				//handle of the device contex of the window
	HBRUSH hb;				//handle of the brush using to brush the head of the snake

	int dirNow;				//the diraction of the snake
	UINT snakelen = 2;		//the length of the snake
	UINT score = 0;			//the score you have got

	POINT *p;				//using to store the positions of the snake's body

	

public:
	snake(int fx, int fy, HWND hPreWnd);	//the constructor of the snake
	int SetDirection(int dir);				//Set the new direction
	int ShowNewSnake();						//Flash the painting of the snake
	int Move();								//move the snake
	int GrowUp();							//Grow Longer
	int EatFood(int foodtype);				//eat food

	BOOL checkeat(int x, int y);

	UINT GetScore() { return score; };		//return the score you have got
	void SpeedUp();
	void SlowDown();
	void dirrever();
	void healthy();

	int blockLength = 40;					//the block of the snake
	int speed = BEG_SPEED;						//the speed of the snake [0-900]~[Slowest-Fastest]
	
	int tSpeedup = 0, tSlowdown = 0, tDirresver = 0;

};


struct food
{
	int x, y;
	int type = 0;
	int tSpe = TSPE_RESET;

};

