#pragma once
#include <windows.h>
#include "resource.h"

#define MAX_LOADSTRING 100

extern HWND hGetApiKeyButton;
extern HWND hStartButton;
extern HWND hExitButton;
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

extern HWND hApiKeyInput, hSendNotificationsCheck, hMinimizeToTrayCheck, hShortcutInput, hPromptModeComboBox;
extern HINSTANCE hInst;
