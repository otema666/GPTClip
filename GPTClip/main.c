#include "framework.h"
#include "GPTClip.h"
#include <windowsx.h>
#include <shellapi.h>

#define MAX_LOADSTRING 100
#define WM_TRAYICON (WM_USER + 1)

// Variables globales:
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

// Variables para los controles
HWND hApiKeyInput, hSendNotificationsCheck, hMinimizeToTrayCheck, hShortcutInput, hPromptModeComboBox;

// Declaraciones de funciones adelantadas:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

// Para el tray icon
NOTIFYICONDATA nid = { sizeof(NOTIFYICONDATA) };
HMENU hTrayMenu;
BOOL trayIconActive = FALSE;

void AddTrayIcon(HWND hWnd);
void RemoveTrayIcon();
void ShowContextMenu(HWND hWnd, POINT pt);
void ToggleTrayIcon(HWND hWnd, BOOL enable);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
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
        CW_USEDEFAULT, 0, 400, 300, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        // Crear controles
        CreateWindowW(L"STATIC", L"API Key:", WS_VISIBLE | WS_CHILD,
            10, 10, 100, 20, hWnd, NULL, hInst, NULL);
        hApiKeyInput = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
            120, 10, 200, 20, hWnd, NULL, hInst, NULL);

        hSendNotificationsCheck = CreateWindowW(L"BUTTON", L"Send Notifications",
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 10, 40, 150, 20, hWnd, NULL, hInst, NULL);

        hMinimizeToTrayCheck = CreateWindowW(L"BUTTON", L"Minimize to Tray",
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 10, 70, 150, 20, hWnd, NULL, hInst, NULL);

        CreateWindowW(L"STATIC", L"Shortcut Key:", WS_VISIBLE | WS_CHILD,
            10, 100, 100, 20, hWnd, NULL, hInst, NULL);
        hShortcutInput = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
            120, 100, 100, 20, hWnd, NULL, hInst, NULL);

        CreateWindowW(L"STATIC", L"Prompt Mode:", WS_VISIBLE | WS_CHILD,
            10, 130, 100, 20, hWnd, NULL, hInst, NULL);
        hPromptModeComboBox = CreateWindowW(L"COMBOBOX", NULL,
            WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 120, 130, 200, 100, hWnd, NULL, hInst, NULL);
        SendMessage(hPromptModeComboBox, CB_ADDSTRING, 0, (LPARAM)L"Test Mode (Multiple Response)");
        SendMessage(hPromptModeComboBox, CB_ADDSTRING, 0, (LPARAM)L"Development Mode (Long Responses)");
        SendMessage(hPromptModeComboBox, CB_SETCURSEL, 0, 0); // Seleccionar la primera opción por defecto
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        }
        break;

    case WM_CLOSE:
        if (IsDlgButtonChecked(hWnd, hMinimizeToTrayCheck)) {
            ShowWindow(hWnd, SW_HIDE);
            AddTrayIcon(hWnd);
        }
        else {
            DestroyWindow(hWnd);
        }
        break;

    case WM_DESTROY:
        RemoveTrayIcon();
        PostQuitMessage(0);
        break;

    case WM_TRAYICON:
        if (lParam == WM_RBUTTONDOWN) {
            POINT pt;
            GetCursorPos(&pt);
            ShowContextMenu(hWnd, pt);
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void AddTrayIcon(HWND hWnd) {
    nid.hWnd = hWnd;
    nid.uID = 100;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    wcscpy_s(nid.szTip, sizeof(nid.szTip) / sizeof(wchar_t), L"GPTClip");
    nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    Shell_NotifyIcon(NIM_ADD, &nid);
    trayIconActive = TRUE;
}

void RemoveTrayIcon() {
    if (trayIconActive) {
        Shell_NotifyIcon(NIM_DELETE, &nid);
        trayIconActive = FALSE;
    }
}

void ShowContextMenu(HWND hWnd, POINT pt) {
    hTrayMenu = CreatePopupMenu();
    AppendMenu(hTrayMenu, MF_STRING, IDM_EXIT, L"Exit");
    SetForegroundWindow(hWnd);  // Necesario para asegurar que el menú se muestra correctamente
    TrackPopupMenu(hTrayMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
    DestroyMenu(hTrayMenu);
}
