#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <time.h>
#include "api.h"
#include "shortcut.h"
#include "ui.h"
#include "prompts/prompts.h"
#pragma warning(disable : 4996)

char* respuesta = NULL;
time_t last_hotkey_time = 0;

char* utf8_to_ansi(const char* utf8) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
    wchar_t* wstr = (wchar_t*)malloc(size_needed * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, size_needed);

    size_needed = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* ansi = (char*)malloc(size_needed);
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, ansi, size_needed, NULL, NULL);

    free(wstr);
    return ansi;
}

wchar_t* get_paper_clip() {
    if (!OpenClipboard(NULL)) {
        return NULL; // No se pudo abrir el portapapeles
    }

    HGLOBAL hGlob = GetClipboardData(CF_UNICODETEXT); // Usamos CF_UNICODETEXT
    if (hGlob == NULL) {
        CloseClipboard();
        return NULL; // No hay datos en el portapapeles
    }

    wchar_t* pBuf = (wchar_t*)GlobalLock(hGlob); // Leemos el contenido como wchar_t*
    if (pBuf == NULL) {
        CloseClipboard();
        return NULL; // No se pudo bloquear
    }

    wchar_t* clipContent = _wcsdup(pBuf); // Duplicamos el contenido

    GlobalUnlock(hGlob);
    CloseClipboard();

    return clipContent; // Retornamos el texto Unicode
}

void post_paper_clip(const wchar_t* text) {
    if (!OpenClipboard(NULL)) {
        return; // No se pudo abrir el portapapeles
    }

    EmptyClipboard();

    size_t len = wcslen(text) + 1;
    HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, len * sizeof(wchar_t));
    if (hGlob) {
        wchar_t* pBuf = (wchar_t*)GlobalLock(hGlob);
        wcscpy(pBuf, text); // Copiamos el texto Unicode
        GlobalUnlock(hGlob);

        SetClipboardData(CF_UNICODETEXT, hGlob); // Usamos CF_UNICODETEXT
    }

    CloseClipboard();
}


DWORD WINAPI monitor_keys() {
    MessageBoxW(NULL, L"La aplicacion esta preparada y ejecutada en segundo plano", L"Aplicacion lista", MB_OK);

    while (TRUE) {
        time_t current_time = time(NULL);

        if ((GetAsyncKeyState('0') & 0x8000) &&
            (GetAsyncKeyState(VK_CONTROL) & 0x8000) &&
            (GetAsyncKeyState(VK_MENU) & 0x8000) &&
            difftime(current_time, last_hotkey_time) >= 10) {

            wchar_t* question = get_paper_clip();
            wchar_t* prompt = get_prompt();
            MessageBoxW(NULL, question, L"Question", MB_OK);
            MessageBoxW(NULL, prompt, L"Prompt", MB_OK);

            LPSTR response = get_gpt_response(prompt, question);
            MessageBoxW(NULL, response, L"Respuesta correcta", MB_OK);

            if (response != NULL) {
                if (respuesta) {

                    free(respuesta);
                }
                respuesta = (char*)malloc(strlen(response) + 1);
                if (respuesta) {
                    strcpy(respuesta, response);
                }

                if (respuesta) {
                    post_paper_clip(respuesta);
                }

                free(question);
            }
            

            // Actualiza el tiempo de la última vez que se presionó el atajo
            last_hotkey_time = current_time;

            // Evita múltiples pulsaciones mientras se mantiene el atajo
            while ((GetAsyncKeyState('0') & 0x8000) &&
                (GetAsyncKeyState(VK_CONTROL) & 0x8000) &&
                (GetAsyncKeyState(VK_MENU) & 0x8000)) {
                Sleep(1000); // Espera 1 segundo
            }
        }

        Sleep(50); // Espera 50 ms antes de volver a verificar las teclas
    }

    // Libera la memoria al finalizar el hilo
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

    wchar_t buffer[100];
    SendMessage(hPromptModeComboBox, CB_GETLBTEXT, index, (LPARAM)buffer);

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
    else {
        MessageBoxW(NULL, L"Seleccione un modo de solicitud válido", L"Prompt error", MB_ICONERROR | MB_OK);
        return L"no hay";
    }
}