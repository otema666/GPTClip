#include "tray.h"
#include "resource.h"

NOTIFYICONDATA nid = { sizeof(NOTIFYICONDATA) };
BOOL trayIconActive = FALSE;
HMENU hTrayMenu;
#define IDM_EXIT 1001
#define IDM_SHOW 1002

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
    AppendMenu(hTrayMenu, MF_STRING, IDM_SHOW, L"Abrir");
    AppendMenu(hTrayMenu, MF_STRING, IDM_EXIT, L"Salir");
    SetForegroundWindow(hWnd);
    TrackPopupMenu(hTrayMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
    DestroyMenu(hTrayMenu);
}

void HandleTrayMenuCommand(WPARAM wParam, HWND hWnd) {
    switch (LOWORD(wParam)) {
    case IDM_SHOW:
        ShowWindow(hWnd, SW_SHOW);
        RemoveTrayIcon();
        break;
    case IDM_EXIT:
        RemoveTrayIcon();
        DestroyWindow(hWnd);
        break;
    }
}