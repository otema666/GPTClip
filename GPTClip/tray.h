#pragma once
#include <windows.h>
#include <shellapi.h>
#include "main.h"

#define WM_TRAYICON (WM_USER + 1)
#define IDM_SHOW 1002
#define IDM_EXIT 40001

void AddTrayIcon(HWND hWnd);
void RemoveTrayIcon();
void ShowContextMenu(HWND hWnd, POINT pt);
void HandleTrayMenuCommand(WPARAM wParam, HWND hWnd);