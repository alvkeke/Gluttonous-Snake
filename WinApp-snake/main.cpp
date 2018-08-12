
#include "main.h"
#include "MsgFunctions.h"
#include "Snake.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR args, int argc) {
	WNDCLASS wnc;												//window class
	MSG msg;													//message struct
	HWND hWnd;													//main window handle
	int iWndLeft, iWndTop;										//to store the position of the window

	wnc.cbClsExtra = NULL;				
	wnc.cbWndExtra = NULL;
	wnc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	// Background color of the window
	wnc.hCursor = LoadCursor(NULL, IDC_ARROW);					// Cursor of the window
	wnc.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// Application Icon
	wnc.hInstance = hInstance;
	wnc.lpfnWndProc = &mainProc;								// hInstance of the Application 
	wnc.lpszClassName = WND_CLS_NAME;							// classname
	wnc.lpszMenuName = NULL;
	wnc.style = CS_VREDRAW | CS_HREDRAW;


	if (!RegisterClass(&wnc)) {
		MessageBox(0, "Error in Register Window Class.", "ERROR", MB_ICONERROR);
		return -1;
	}

	iWndLeft = (GetSystemMetrics(SM_CXFULLSCREEN)-WND_WID)/2;
	iWndTop = (GetSystemMetrics(SM_CYFULLSCREEN) - WND_WID) / 2;
	// Make sure the window at the center of the screen
	hWnd = CreateWindow(WND_CLS_NAME, WND_TITLE, CW_USEDEFAULT, iWndLeft, iWndTop, WND_WID, WND_WID+ WND_RESER, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, 0, 0, 0)) {							//message loop
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK mainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg)
	{
	case WM_CLOSE:
		srand(time(NULL));
		OnClose(hWnd, msg, wParam, lParam); 
		break;
	case WM_CREATE:
		OnCreate(hWnd, msg, wParam, lParam);
		break;
	case WM_KEYDOWN:
		OnKeyDown(hWnd, msg, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		PostMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, NULL);

	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


