#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include "api.h"
#include "shortcut.h"
#include "ui.h"
#include "prompts/prompts.h"
#include "notifications.h"
#pragma warning(disable : 4996)

wchar_t* respuesta = NULL;
time_t last_hotkey_time = 0;

wchar_t* get_paper_clip() {
    if (!OpenClipboard(NULL)) {
        return NULL;
    }

    HGLOBAL hGlob = GetClipboardData(CF_UNICODETEXT);
    if (hGlob == NULL) {
        CloseClipboard();
        return NULL;
    }

    wchar_t* pBuf = (wchar_t*)GlobalLock(hGlob);
    if (pBuf == NULL) {
        CloseClipboard();
        return NULL;
    }

    wchar_t* clipContent = _wcsdup(pBuf);

    GlobalUnlock(hGlob);
    CloseClipboard();

    return clipContent;
}

void seleccionOculta() {
    const wchar_t* script = L"document.head.appendChild(Object.assign(document.createElement('style'), { textContent: '::selection { background: rgba(0, 179, 255, 0.09); color: inherit; }' }));";
	post_paper_clip(script);
}

void post_paper_clip(const wchar_t* text) {
    if (OpenClipboard(NULL)) {
        EmptyClipboard();

        HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, (wcslen(text) + 1) * sizeof(wchar_t));
        if (hGlob) {
            wchar_t* pGlobal = (wchar_t*)GlobalLock(hGlob);
            if (pGlobal) {
                wmemcpy(pGlobal, text, wcslen(text) + 1);
                GlobalUnlock(hGlob);
                SetClipboardData(CF_UNICODETEXT, hGlob);
            }
        }

        CloseClipboard();
    }
    else {
        MessageBoxW(NULL, L"No se pudo abrir el portapapeles.", L"Error", MB_OK | MB_ICONERROR);
    }
}


DWORD WINAPI monitor_keys() {
    MessageBoxW(NULL, L"La aplicacion esta preparada y ejecutada en segundo plano", L"Aplicacion lista", MB_OK);

    while (TRUE) {
        time_t current_time = time(NULL);

        if ((GetAsyncKeyState('0') & 0x8000) &&
            (GetAsyncKeyState(VK_CONTROL) & 0x8000) &&
            (GetAsyncKeyState(VK_MENU) & 0x8000) &&
            difftime(current_time, last_hotkey_time) >= 7) {

            wchar_t* question = get_paper_clip();
            wchar_t* prompt = get_prompt();
            LPSTR response = get_gpt_response(prompt, question);
            //post_paper_clip(response);

            if (response != NULL) {
                if (respuesta) {
                    free(respuesta);
                }

                respuesta = (wchar_t*)malloc((wcslen(response) + 1) * sizeof(wchar_t));
                if (respuesta) {
                    wcscpy(respuesta, response);
                    post_paper_clip(respuesta);
                    if (SendMessage(hSendNotificationsCheck, BM_GETCHECK, 0, 0) == BST_CHECKED) {
                        notificar(respuesta);
                    }
                }

                free(question);
            }

            last_hotkey_time = current_time;

            while ((GetAsyncKeyState('0') & 0x8000) &&
                (GetAsyncKeyState(VK_CONTROL) & 0x8000) &&
                (GetAsyncKeyState(VK_MENU) & 0x8000)) {
                Sleep(1000);
            }
        }

        Sleep(50);
    }

    if (respuesta) {
        free(respuesta);
    }

    return 0;
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

wchar_t* get_prompt() {
    wchar_t* promptMode = GetSelectedPromptMode();
    if (wcscmp(promptMode, L"Examen tipo test (Respuesta Múltiple)") == 0) {
        return prompt_test;
    }
    else if (wcscmp(promptMode, L"Examen tipo desarrollo (Respuestas más largas)") == 0) {
        return prompt_LongAnswer;
	}
	else if (wcscmp(promptMode, L"Desactivar prompt") == 0) {
		return no_prompt;
	}
    else {
        MessageBoxW(NULL, L"Seleccione un modo de solicitud válido", L"Prompt error", MB_ICONERROR | MB_OK);
        return L"no hay";
    }
}
