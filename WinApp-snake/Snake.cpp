#include "Snake.h"
#include "MsgFunctions.h"

snake::snake(int fx, int fy, HWND hWnd)
{

	hPreWnd = hWnd;													//store the handle of the parent window

	p = (POINT*)malloc(sizeof(POINT) * 2);							//Initlization of the pointer

	p[0].x = fx;													//
	p[0].y = fy;													//random pos for the first time
	dirNow = rand() %4 + 1;											// first direction: up

	switch (dirNow)
	{
	case SNAKE_DIR_DOWN:
		p[1].x = p[0].x;
		p[1].y = p[0].y - 1;
		break;
	case SNAKE_DIR_UP:
		p[1].x = p[0].x;
		p[1].y = p[0].y + 1;
		break;
	case SNAKE_DIR_LEFT:
		p[1].y = p[0].y;
		p[1].x = p[0].x + 1;
		break;
	case SNAKE_DIR_RIGHT:
		p[1].y = p[0].y;
		p[1].x = p[0].x - 1;
		break;
	}

	hdc = GetDC(hPreWnd);											//to store the window device contex
	hp = CreatePen(CP_NONE, 1, RGB(255, 0, 0));						//a pen to draw the line of the snake;
	hb = CreateSolidBrush(RGB(255, 255, 0));						//a brush to paint the head of the snake;
	SelectObject(hdc, hp);											//select hp to draw line

	SetTimer(hPreWnd, ID_TIMER_MOVE, 1000-speed, TimerProc_move);	//start the timer to move the snake
	SetTimer(hPreWnd, ID_TIMER_SHOW, 10, TimerProc_show);			//start the timer to show the snake
}

int snake::SetDirection(int dir)
{
	if (abs(dir - dirNow) != 2) {	// direction can not be reversed
		dirNow = dir;				// set the direction	
		return 0;					// return no error
	}
	return 1;						// return error
}



int snake::ShowNewSnake()
{
	UINT i;
	RECT headrect;													//the rect of the snake head

	headrect.left = p[0].x * blockLength + 1;							//set the snake head area to the rect
	headrect.top = p[0].y * blockLength + 1;
	headrect.right = (p[0].x + 1) * blockLength - 1;
	headrect.bottom = (p[0].y + 1) * blockLength - 1;

	for (i = 0; i < snakelen; i++) {								//draw the line of the snake body in a loop
		Rectangle(hdc, p[i].x*blockLength, p[i].y*blockLength, (p[i].x + 1)*blockLength, (p[i].y + 1)*blockLength);
	}

	FillRect(hdc, &headrect, hb);									//paint the head of the snake

	if(tSpeedup>0)tSpeedup--;
	if (tSlowdown>0)tSlowdown--;
	if (tDirresver>0)tDirresver--;


	return 0;														// return no error
}

int snake::Move()
{
	UINT i;

	for (i = snakelen - 1; i > 0; i--) {							//--------------------------------------
			p[i].x = p[i - 1].x;									//move the tail
			p[i].y = p[i - 1].y;									//
	}																//--------------------------------------
	
	switch (dirNow)													//using the direction to move the head of the snake
	{
	case SNAKE_DIR_DOWN:
			p[0].y++;		
		break;
	case SNAKE_DIR_UP:
			p[0].y--;
		break;
	case SNAKE_DIR_LEFT:
			p[0].x--;

		break;
	case SNAKE_DIR_RIGHT:
			p[0].x++;
		break;
	}

	InvalidateRect(hPreWnd, NULL, true);							// clear the painting

	if (p[0].x >= 20 || p[0].x < 0 || p[0].y >= 20 || p[0].y <0) {
		return 1;
	}

	for (i = snakelen - 1; i > 0; i--) {							//--------------------------------------
		if (p[0].x == p[i].x && p[0].y == p[i].y) {					//judge if the snake eat itself
			return 1;												//if(eat) return 1
		}															//---------------------------------------
	}																//do these in a loop


	return 0;														//the snake is alive, return 0
}

int snake::GrowUp()
{
	snakelen++;														//make the snake longer
	p = (POINT*)realloc(p, sizeof(POINT)*snakelen);					//Re-Initlization of the position
	
	return 0;
}

int snake::EatFood(int foodtype)
{
	switch (foodtype)
	{
	case FOOD_NORMAL:
		score++;
		break;
	case FOOD_DBSCORE:
		score += 2;
		break;
	case FOOD_HEALTHY:
		score++;
		tSpeedup = 0;
		tSlowdown = 0;
		tDirresver = 0;
		break;
	case FOOD_SLOWDOWN:
		score++;
		tSpeedup = 0;
		tSlowdown += TIME_SLOWDOWN;
		if (speed > BEG_SPEED) {
			speed -= 100;
		}
		SetTimer(hPreWnd, ID_TIMER_MOVE, 1000 - speed, TimerProc_move);	//reset the speed
		break;
	case FOOD_SPEEDUP:
		score++;
		tSlowdown = 0;
		tSpeedup = TIME_SPEEDUP;
		if(speed < 900)
			speed += 200;
		SetTimer(hPreWnd, ID_TIMER_MOVE, 1000 - speed, TimerProc_move);	//reset the speed
		break;
	case FOOD_RESVER:
		score++;
		tDirresver = TIME_REVERSER;
		break;
	}
	
	return GrowUp();
}

BOOL snake::checkeat(int x, int y)
{
	if (x == p[0].x && y == p[0].y) {
		return true;
	}
	return false;
}

void snake::SpeedUp()
{
	if (tSpeedup > 0)
		tSpeedup--;
	else
		speed = BEG_SPEED;
}

void snake::SlowDown()
{
	if (tSlowdown > 0)
		tSlowdown--;
	else
		speed = BEG_SPEED;
}

void snake::dirrever()
{
	if (tDirresver > 0)
		tDirresver--;
}

void snake::healthy()
{

}
