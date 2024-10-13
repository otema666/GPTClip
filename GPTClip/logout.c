#include <windows.h>
#include <winhttp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logout.h"
#include "login.h"

#define USERNAME_LEN 100

BOOL logoutRequest(const char* username) {
    wchar_t wUsername[USERNAME_LEN];
    mbstowcs(wUsername, username, strlen(username) + 1);

    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    BOOL bResults = FALSE;

    hSession = WinHttpOpen(L"gptClip logout", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) {
        MessageBox(NULL, L"Error al iniciar la sesión HTTP", L"Error", MB_OK | MB_ICONERROR);
        return FALSE;
    }

    hConnect = WinHttpConnect(hSession, L"otema666.ddns.net", INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) {
        MessageBox(NULL, L"Error al conectar con el servidor", L"Error", MB_OK | MB_ICONERROR);
        WinHttpCloseHandle(hSession);
        return FALSE;
    }

    hRequest = WinHttpOpenRequest(hConnect, L"POST", L"/gptClip/logout.php", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

    if (!hRequest) {
        MessageBox(NULL, L"No se pudo abrir la solicitud HTTP", L"Error", MB_OK | MB_ICONERROR);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return FALSE;
    }

    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wUsername, -1, NULL, 0, NULL, NULL);
    char* utf8Username = (char*)malloc(bufferSize);
    WideCharToMultiByte(CP_UTF8, 0, wUsername, -1, utf8Username, bufferSize, NULL, NULL);

    char data[200];
    snprintf(data, sizeof(data), "username=%s", utf8Username);

    LPCWSTR headers = L"Content-Type: application/x-www-form-urlencoded";

    bResults = WinHttpSendRequest(hRequest, headers, (DWORD)-1L, (LPVOID)data, strlen(data), strlen(data), 0);
    if (!bResults) {
        MessageBox(NULL, L"Error al enviar la solicitud HTTP", L"Error", MB_OK | MB_ICONERROR);
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        free(utf8Username);
        return FALSE;
    }

    bResults = WinHttpReceiveResponse(hRequest, NULL);
    if (!bResults) {
        MessageBox(NULL, L"Error al recibir la respuesta", L"Error", MB_OK | MB_ICONERROR);
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    free(utf8Username);

    return TRUE;
}
