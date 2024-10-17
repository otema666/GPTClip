#include "ui.h"
#include <wchar.h>
#include <shellapi.h>
#include "login.h"

HWND hStartButton;
HWND hExitButton;
HWND hHelpButton;
HWND hSeleccionButton;
HWND hSeleccionHelpButton;
HWND hInputBoxButton;
HWND hInputBoxHelpButton;

void CreateUI(HWND hWnd) {
    HBRUSH hBrush = CreateSolidBrush(RGB(240, 240, 240));
    SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBrush);
    InvalidateRect(hWnd, NULL, TRUE);

    wchar_t saludo[100];
    swprintf_s(saludo, 100, L"Bienvenido de nuevo, %hs.", loggedInUsername);
    CreateWindowW(L"STATIC", saludo, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_NOTIFY,
        0, 10, 450, 20, hWnd, NULL, hInst, NULL);

    hSendNotificationsCheck = CreateWindowW(L"BUTTON", L"Enviar notificaciones",
        WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_NOTIFY,
        10, 110, 150, 20, hWnd, NULL, hInst, NULL);
    EnableWindow(hSendNotificationsCheck, FALSE);

    CreateWindowW(L"STATIC", L"Atajo del teclado:", WS_VISIBLE | WS_CHILD,
        10, 40, 120, 20, hWnd, NULL, hInst, NULL);
    CreateWindowW(L"STATIC", L"<CTRL + ALT + 0>", WS_VISIBLE | WS_CHILD,
        130, 40, 180, 20, hWnd, NULL, hInst, NULL);

    CreateWindowW(L"STATIC", L"Tipo de respuestas:", WS_VISIBLE | WS_CHILD,
        10, 80, 140, 20, hWnd, NULL, hInst, NULL);
    hPromptModeComboBox = CreateWindowW(L"COMBOBOX", NULL,
        WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_EX_COMPOSITED,
        150, 80, 270, 100, hWnd, NULL, hInst, NULL);
    SendMessage(hPromptModeComboBox, CB_ADDSTRING, 0, (LPARAM)L"--Seleccionar--");
    SendMessage(hPromptModeComboBox, CB_ADDSTRING, 0, (LPARAM)L"Examen tipo test (Respuesta Múltiple)");
    SendMessage(hPromptModeComboBox, CB_ADDSTRING, 0, (LPARAM)L"Examen tipo desarrollo (Respuestas más largas)");
    SendMessage(hPromptModeComboBox, CB_ADDSTRING, 0, (LPARAM)L"Desactivar prompt (Respuestas Normales)");
    SendMessage(hPromptModeComboBox, CB_SETCURSEL, 0, 0);

    HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"Segoe UI");
    SendMessage(hShortcutInput, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(hPromptModeComboBox, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(hSendNotificationsCheck, WM_SETFONT, (WPARAM)hFont, TRUE);

    hStartButton = CreateWindowW(L"BUTTON", L"Iniciar",
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_NOTIFY,
        90, 330, 100, 30, hWnd, NULL, hInst, NULL);
    hExitButton = CreateWindowW(L"BUTTON", L"Salir",
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_NOTIFY,
        230, 330, 100, 30, hWnd, NULL, hInst, NULL);
    hSeleccionButton = CreateWindowW(L"BUTTON", L"Selección oculta",
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_NOTIFY,
        10, 140, 120, 20, hWnd, NULL, hInst, NULL);
    hSeleccionHelpButton = CreateWindowW(L"BUTTON", L"?",
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_NOTIFY,
        150, 140, 20, 20, hWnd, NULL, hInst, NULL);
	hInputBoxButton = CreateWindowW(L"BUTTON", L"Caja de texto",
		WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_NOTIFY,
		10, 170, 120, 20, hWnd, NULL, hInst, NULL);
	hInputBoxHelpButton = CreateWindowW(L"BUTTON", L"?",
		WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_NOTIFY,
		150, 170, 20, 20, hWnd, NULL, hInst, NULL);
    hHelpButton = CreateWindowW(L"BUTTON", L"Ayuda",
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_NOTIFY,
        10, 200, 70, 20, hWnd, NULL, hInst, NULL);
}
