#pragma once
#include <Windows.h>
#include <time.h>
#include <string>

//HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam

void OnClose(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void OnCreate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void OnKeyDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void CALLBACK TimerProc_move(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime);
void CALLBACK TimerProc_show(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime);
void DrawTable(HWND);

void ShowFood(HWND hWnd, struct food *f);