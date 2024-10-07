#include "ui.h"
#include <shellapi.h>

HWND hGetApiKeyButton;
HWND hStartButton;
HWND hExitButton;
void CreateUI(HWND hWnd) {
    CreateWindowW(L"STATIC", L"API Key:", WS_VISIBLE | WS_CHILD, 10, 10, 100, 20, hWnd, NULL, hInst, NULL);
    hApiKeyInput = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 120, 10, 180, 20, hWnd, NULL, hInst, NULL);

    hGetApiKeyButton = CreateWindowW(L"BUTTON", L"Obtener API Key",
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        310, 5, 110, 30, hWnd, NULL, hInst, NULL);

    hSendNotificationsCheck = CreateWindowW(L"BUTTON", L"Enviar notificaciones", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 10, 40, 150, 20, hWnd, NULL, hInst, NULL);

    CreateWindowW(L"STATIC", L"Atajo del teclado:", WS_VISIBLE | WS_CHILD, 10, 80, 120, 20, hWnd, NULL, hInst, NULL);
    hShortcutInput = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 130, 80, 180, 20, hWnd, NULL, hInst, NULL);

    CreateWindowW(L"STATIC", L"Tipo de respuestas:", WS_VISIBLE | WS_CHILD, 10, 110, 140, 20, hWnd, NULL, hInst, NULL);
    hPromptModeComboBox = CreateWindowW(L"COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 150, 110, 260, 100, hWnd, NULL, hInst, NULL);
    SendMessage(hPromptModeComboBox, CB_ADDSTRING, 0, (LPARAM)L"--Seleccionar--");
    SendMessage(hPromptModeComboBox, CB_ADDSTRING, 0, (LPARAM)L"Examen tipo test (Respuesta Múltiple)");
    SendMessage(hPromptModeComboBox, CB_ADDSTRING, 0, (LPARAM)L"Examen tipo desarrollo (Respuestas más largas)");
    SendMessage(hPromptModeComboBox, CB_SETCURSEL, 0, 0);

    HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"Segoe UI");
    SendMessage(hApiKeyInput, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(hShortcutInput, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(hPromptModeComboBox, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(hGetApiKeyButton, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(hSendNotificationsCheck, WM_SETFONT, (WPARAM)hFont, TRUE);

    hStartButton = CreateWindowW(L"BUTTON", L"Iniciar",
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        90, 330, 100, 30, hWnd, NULL, hInst, NULL);

    hExitButton = CreateWindowW(L"BUTTON", L"Salir",
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        230, 330, 100, 30, hWnd, NULL, hInst, NULL);
}
