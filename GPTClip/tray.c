#include <wchar.h>
#include "tray.h"
#include "resource.h"
#include "prompts/prompts.h"
#pragma warning(disable : 4996)

NOTIFYICONDATA nid = { sizeof(NOTIFYICONDATA) };
BOOL trayIconActive = FALSE;
HMENU hTrayMenu;

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

    // Submenú para seleccionar el prompt
    HMENU hPromptSubMenu = CreatePopupMenu();
    AppendMenu(hPromptSubMenu, MF_STRING, IDM_PROMPT_TEST, L"Tipo test");
    AppendMenu(hPromptSubMenu, MF_STRING, IDM_PROMPT_DEV, L"Tipo desarrollo");
    AppendMenu(hPromptSubMenu, MF_STRING, IDM_PROMPT_OFF, L"Desactivar");

    // Agregar el submenú de prompt al menú principal del tray
    AppendMenu(hTrayMenu, MF_POPUP, (UINT_PTR)hPromptSubMenu, L"Seleccionar prompt");

    // Marcar la opción seleccionada actual
    CheckMenuItem(hPromptSubMenu, currentPromptSelection(), MF_BYCOMMAND | MF_CHECKED);

    // Otras opciones del menú del tray
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

    case IDM_PROMPT_TEST:
        SendMessage(hPromptModeComboBox, CB_SETCURSEL, 1, 0);
        break;
    case IDM_PROMPT_DEV:
        SendMessage(hPromptModeComboBox, CB_SETCURSEL, 2, 0);
        break;
    case IDM_PROMPT_OFF:
        SendMessage(hPromptModeComboBox, CB_SETCURSEL, 3, 0);
        break;
    }
}

int currentPromptSelection() {
    wchar_t* promptMode = GetSelectedPromptMode();
    if (wcscmp(promptMode, L"Examen tipo test (Respuesta Múltiple)") == 0) {
        return IDM_PROMPT_TEST;
    }
    else if (wcscmp(promptMode, L"Examen tipo desarrollo (Respuestas más largas)") == 0) {
        return IDM_PROMPT_DEV;
    }
    else if (wcscmp(promptMode, L"Desactivar prompt (Respuestas Normales)") == 0) {
        return IDM_PROMPT_OFF;
    }

}

wchar_t* GetSelectedPromptMode() {
    int index = SendMessage(hPromptModeComboBox, CB_GETCURSEL, 0, 0);
    if (index == CB_ERR) {
        MessageBoxW(NULL, L"Error al obtener el índice del ComboBox", L"Error", MB_OK);
        return NULL;
    }

    wchar_t buffer[101];
    SendMessage(hPromptModeComboBox, CB_GETLBTEXT, index, (LPARAM)buffer);
    buffer[100] = L'\0';

    if (wcslen(buffer) == 0) {
        MessageBoxW(NULL, L"No se obtuvo texto del ComboBox", L"Error", MB_OK);
        return NULL;
    }

    return wcsdup(buffer);
}

