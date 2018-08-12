#include "MsgFunctions.h"
#include "Snake.h"
#include "main.h"


snake *s;	//A pointer of a snake

food foodNor, foodSpe;

void OnClose(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	PostQuitMessage(0);		//Post Quit Message to the Application to exit.
}

void OnCreate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	
	int rx, ry;						//the first place of the snake
	
	foodNor.x = rand() % 19;
	foodNor.y = rand() % 19;
	foodSpe.x = (rand() % 49) - 30;
	foodSpe.y = (rand() % 49) - 30;
	foodSpe.type = (rand() % 5) + 1;
	foodSpe.tSpe = TSPE_RESET;

	rx = rand() % 19;				//random : 0-19
	ry = rand() % 19;
	


	s = new snake(rx, ry, hWnd);	//new a snake
	s->blockLength = WND_WID / 20;
}

void OnKeyDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_DOWN:
		if (s->tDirresver>0)
			s->SetDirection(SNAKE_DIR_UP);
		else
			s->SetDirection(SNAKE_DIR_DOWN);
		break;
	case VK_UP:
		if (s->tDirresver>0)
			s->SetDirection(SNAKE_DIR_DOWN);
		else
			s->SetDirection(SNAKE_DIR_UP);
		break;
	case VK_RIGHT:
		if (s->tDirresver > 0)
			s->SetDirection(SNAKE_DIR_LEFT);
		else
			s->SetDirection(SNAKE_DIR_RIGHT);
		break;
	case VK_LEFT:
		if (s->tDirresver > 0)
			s->SetDirection(SNAKE_DIR_RIGHT);
		else
			s->SetDirection(SNAKE_DIR_LEFT);	//to change the diection
		break;
	case 'P':								//to pause the game
		if (!KillTimer(hWnd, ID_TIMER_MOVE))SetTimer(hWnd, ID_TIMER_MOVE, 1000 - s->speed, TimerProc_move);
		break;
	case VK_ESCAPE:
		PostQuitMessage(0);
		break;/*
	case VK_SPACE:							//for test,to lengthen the snake
		s->GrowUp();
		break;*/
	}
}


void CALLBACK TimerProc_move(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
{
	if (s->Move()) {						//while snake eat itself
		KillTimer(hWnd, ID_TIMER_MOVE);		//stop the game timer;
		KillTimer(hWnd, ID_TIMER_SHOW);
		UINT score = s->GetScore();
		std::string s = std::string("You Are Dead, You Scored :").append(std::to_string(score));
		MessageBox(hWnd, s.c_str(),"GameOver",0);
		PostQuitMessage(0);					//exit the application
	}
	
	if (s->checkeat(foodNor.x, foodNor.y)) {
		s->EatFood(foodNor.type);
		foodNor.x = rand() % 19;
		foodNor.y = rand() % 19;
	}

	if (s->checkeat(foodSpe.x, foodSpe.y)) {
		s->EatFood(foodSpe.type);
		foodSpe.x = (rand() % 49) - 30;
		foodSpe.y = (rand() % 49) - 30;
		foodSpe.type = (rand() % 5) + 1;
		foodSpe.tSpe = TSPE_RESET;
	}

	if (foodSpe.x < 0 || foodSpe.y < 0) {
		foodSpe.x = (rand() % 49) - 30;
		foodSpe.y = (rand() % 49) - 30;
		foodSpe.type = (rand() % 5) + 1;
		foodSpe.tSpe = TSPE_RESET;
	}

}

void CALLBACK TimerProc_show(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
{
	
	DrawTable(hWnd);
	ShowFood(hWnd, &foodNor);
	ShowFood(hWnd, &foodSpe);
	s->ShowNewSnake();			//Flash the body of the snake
	s->SpeedUp();
	s->SlowDown();
	s->dirrever();

	if (foodSpe.tSpe-- <= 0) {
		foodSpe.x = -1;
		foodSpe.y = -1;
		foodSpe.type = (rand() % 5) + 1;
		foodSpe.tSpe = TSPE_RESET;
	}

}

void DrawTable(HWND hWnd)
{
	static HBRUSH hb = CreateSolidBrush(RGB(0, 0, 0));
	static HPEN hp = CreatePen(CP_NONE, 2, RGB(0, 0, 255));
	HDC hdc = GetDC(hWnd);
	RECT rtxt;

	SelectObject(hdc, hp);
	SelectObject(hdc, hb);

	Rectangle(hdc, 0, 0, WND_WID, WND_WID);

	rtxt.left = 10;
	rtxt.top = WND_WID+10;
	rtxt.right = rtxt.left + 100;
	rtxt.bottom = WND_WID + WND_RESER;
	std::string sScore = std::string("Score:").append(std::to_string(s->GetScore()));
	DrawText(hdc, sScore.c_str(), sScore.length(), &rtxt, DT_LEFT | DT_VCENTER);

	rtxt.left = rtxt.right + 10;
	rtxt.top = WND_WID + 10;
	rtxt.right = rtxt.left + 200;
	rtxt.bottom = WND_WID + WND_RESER;
	std::string sSUp = std::string("SpeedUp time:").append(std::to_string(s->tSpeedup));
	DrawText(hdc, sSUp.c_str(), sSUp.length(), &rtxt, DT_LEFT | DT_VCENTER);

	rtxt.left = rtxt.right + 10;
	rtxt.top = WND_WID + 10;
	rtxt.right = rtxt.left + 200;
	rtxt.bottom = WND_WID + WND_RESER;
	std::string sSdn = std::string("SlowDown time:").append(std::to_string(s->tSlowdown));
	DrawText(hdc, sSdn.c_str(), sSdn.length(), &rtxt, DT_LEFT | DT_VCENTER);

	rtxt.left = rtxt.right + 10;
	rtxt.top = WND_WID + 10;
	rtxt.right = rtxt.left + 200;
	rtxt.bottom = WND_WID + WND_RESER;
	std::string sRev = std::string("Direction Reverse time:").append(std::to_string(s->tDirresver));
	DrawText(hdc, sRev.c_str(), sRev.length(), &rtxt, DT_LEFT | DT_VCENTER);

	ReleaseDC(hWnd, hdc);

}

void ShowFood(HWND hWnd, struct food * f)
{
	HDC hdc = GetDC(hWnd);

	HPEN hp_normal = CreatePen(PS_SOLID, 6, RGB(255, 255, 255));	//white normal food
	HPEN hp_healthy = CreatePen(PS_SOLID, 6, RGB(0, 255, 0));		//green healthy food
	HPEN hp_speed = CreatePen(PS_SOLID, 6, RGB(255, 0, 0));			//red for speed up food
	HPEN hp_slow = CreatePen(PS_SOLID, 6, RGB(0, 0, 255));			//blue for slow
	HPEN hp_dbscro = CreatePen(PS_SOLID, 6, RGB(255, 131, 250));	//pink for double score
	HPEN hp_rev = CreatePen(PS_SOLID, 6, RGB(255, 255, 0));			//yellow for reveser

	switch (f->type)
	{
	case FOOD_NORMAL:
		SelectObject(hdc, hp_normal);
		break;
	case FOOD_HEALTHY:
		SelectObject(hdc, hp_healthy);
		break;
	case FOOD_DBSCORE:
		SelectObject(hdc, hp_dbscro);
		break;
	case FOOD_RESVER:
		SelectObject(hdc, hp_rev);
		break;
	case FOOD_SPEEDUP:
		SelectObject(hdc, hp_speed);
		break;
	case FOOD_SLOWDOWN:
		SelectObject(hdc, hp_slow);
		break;
	}
	if(f->x >= 0 && f->y >= 0)
		Rectangle(hdc, f->x * 40, f->y * 40, (f->x + 1)* 40, (f->y + 1) * 40 );

	DeleteObject(hp_normal);
	DeleteObject(hp_healthy);
	DeleteObject(hp_speed);
	DeleteObject(hp_slow);
	DeleteObject(hp_dbscro);
	DeleteObject(hp_rev);

	ReleaseDC(hWnd, hdc);
}
