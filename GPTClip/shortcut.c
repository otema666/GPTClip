#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include "api.h"
#include "shortcut.h"
#include "ui.h"
#include "prompts/prompts.h"
#pragma warning(disable : 4996)

char* respuesta = NULL;

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

char* get_paper_clip() {
    if (!OpenClipboard(NULL)) {
        return NULL; // No se pudo abrir el portapapeles
    }

    HGLOBAL hGlob = GetClipboardData(CF_TEXT);
    if (hGlob == NULL) {
        CloseClipboard();
        return NULL; // No hay datos en el portapapeles
    }

    LPSTR pBuf = (LPSTR)GlobalLock(hGlob);
    if (pBuf == NULL) {
        CloseClipboard();
        return NULL; // No se pudo bloquear
    }

    char* clipContent = utf8_to_ansi(pBuf);

    GlobalUnlock(hGlob);
    CloseClipboard();

    return clipContent;
}

void post_paper_clip(const char* text) {
    if (!OpenClipboard(NULL)) {
        return; // No se pudo abrir el portapapeles
    }

    EmptyClipboard();

    HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, strlen(text) + 1);
    if (hGlob) {
        LPSTR pBuf = (LPSTR)GlobalLock(hGlob);
        strcpy(pBuf, text);
        GlobalUnlock(hGlob);

        SetClipboardData(CF_TEXT, hGlob);
    }

    CloseClipboard();
}

DWORD WINAPI monitor_keys() {
    while (TRUE) {
        if ((GetAsyncKeyState('0') & 0x8000) &&
            (GetAsyncKeyState(VK_CONTROL) & 0x8000) &&
            (GetAsyncKeyState(VK_MENU) & 0x8000)) {

            char* question = get_paper_clip();
            
            char* prompt = get_prompt();
            
            LPSTR response = get_gpt_response(prompt, question);
            
			// please wtf is this shit
            if (response != NULL) {
                // Libera la memoria anterior si ya existe
                if (respuesta) {
                    free(respuesta);
                }
                // Asigna memoria para la nueva respuesta
                respuesta = (char*)malloc(strlen(response) + 1); // +1 para el terminador nulo
                if (respuesta) {
                    strcpy(respuesta, response); // Copia la respuesta al buffer

                }

                // Copia la respuesta al portapapeles
                if (respuesta) {
                    post_paper_clip(respuesta); // Coloca la respuesta en el portapapeles
                }

                // Liberar la memoria de la pregunta obtenida del portapapeles
                free(question);
            }

            while ((GetAsyncKeyState('0') & 0x8000) &&
                (GetAsyncKeyState(VK_CONTROL) & 0x8000) &&
                (GetAsyncKeyState(VK_MENU) & 0x8000)) {
                Sleep(100);
            }
        }
        Sleep(100);
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


char* get_prompt() {
    wchar_t* promptMode = GetSelectedPromptMode();
    if (wcscmp(promptMode, L"Examen tipo test (Respuesta Múltiple)") == 0) {
        return prompt_test;
    }
    else if (wcscmp(promptMode, L"Examen tipo desarrollo (Respuestas más largas)") == 0) {
		return prompt_LongAnswer;
    }
    else {
        int result = MessageBox(NULL, L"Seleccione un modo de solicitud válido", L"Prompt error", MB_ICONERROR | MB_OK);
        return "no hay";
    }

    // Convertir promptMode de wchar_t* a char*
    // Asumiendo que promptMode es un valor válido
    int len = wcslen(promptMode);
    char* prompt = (char*)malloc(len + 1);
    if (prompt) {
        wcstombs(prompt, promptMode, len + 1); // Convertir a char*
    }

    return prompt;
}