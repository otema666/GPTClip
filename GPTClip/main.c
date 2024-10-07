#include "main.h"
#include "tray.h"
#include "registry.h"
#include "resource.h"
#include "login.h"
#include "ui.h"

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

HWND hApiKeyInput, hSendNotificationsCheck, hMinimizeToTrayCheck, hShortcutInput, hPromptModeComboBox;
HWND hGetApiKeyButton;
HWND hStartButton;
HWND hExitButton;
HINSTANCE hInst; // Variable para almacenar la instancia de la aplicación

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
	//crear ventana de login y esperar a que el usuario inicie sesión
	CreateLoginWindow(hInstance); // Función en `login.c`

    
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GPTCLIP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GPTCLIP));

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GPTCLIP);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));
    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance;

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 450, 430, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    LoadAPIKeyFromRegistry(hApiKeyInput); // Función en `registry.c`

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    WCHAR apiKey[256];

    switch (message) {
    case WM_CREATE:
        CreateUI(hWnd); // Función en `ui.c`
        break;

    case WM_CLOSE:
        ShowWindow(hWnd, SW_HIDE);
        AddTrayIcon(hWnd);
        break;

    
    case WM_COMMAND:

        if ((HWND)lParam == hGetApiKeyButton) {
            ShellExecute(NULL, L"open", L"https://platform.openai.com/api-keys", NULL, NULL, SW_SHOWNORMAL);
        }
        else if ((HWND)lParam == hStartButton) {
            HandleTrayMenuCommand(wParam, hWnd);

        }
        else if ((HWND)lParam == hExitButton) {
            DestroyWindow(hWnd);
        }
        else {
            HandleTrayMenuCommand(wParam, hWnd);
        }
        break;

    case WM_DESTROY:
        GetWindowTextW(hApiKeyInput, apiKey, 256);  
        SaveAPIKeyToRegistry(apiKey); // Función en `registry.c`

        RemoveTrayIcon(); // Función en `tray.c`
        PostQuitMessage(0);
        break;

    case WM_TRAYICON:
        if (lParam == WM_RBUTTONDOWN) {
            POINT pt;
            GetCursorPos(&pt);
            ShowContextMenu(hWnd, pt); // Función en `tray.c`
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}