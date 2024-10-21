#include <windows.h>
#include <winhttp.h>
#include <stdio.h>
#include <string.h>
#include "updater.h"
#include <tchar.h>
#include <psapi.h>

#pragma comment(lib, "winhttp.lib")

#define APP_VERSION "2.3"
#define SERVER_NAME L"otema666.ddns.net"
#define SERVER_PATH L"gptClip/version.txt"
#define SERVER_PORT 443
#pragma warning(disable : 4996)

void clean_version(char* version) {
    char* end = version + strlen(version) - 1;
    while (end > version && (*end == '\n' || *end == '\r' || *end == ' ')) {
        *end = '\0';
        end--;
    }
}

void check_for_updates() {
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    BOOL bResults = FALSE;
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer = NULL;
    char server_version[11] = { 0 };
    DWORD dwStatusCode = 0;
    DWORD dwStatusCodeSize = sizeof(dwStatusCode);

    int timeout = 3000;

    hSession = WinHttpOpen(L"gptClip updater", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    if (hSession) {
        hConnect = WinHttpConnect(hSession, SERVER_NAME, SERVER_PORT, 0);
    }

    if (hConnect) {
        hRequest = WinHttpOpenRequest(hConnect, L"GET", SERVER_PATH, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    }

    if (hRequest) {
        WinHttpSetTimeouts(hRequest, timeout, timeout, timeout, timeout);

        bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
    }

    if (bResults) {
        bResults = WinHttpReceiveResponse(hRequest, NULL);
    }

    if (bResults) {
        WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, NULL, &dwStatusCode, &dwStatusCodeSize, NULL);
    }

    if (dwStatusCode == 200) {
        if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
            MessageBox(NULL, L"Error de red", L"GPTClip Error", MB_OK | MB_ICONERROR);
        }
        else if (dwSize > 0) {
            pszOutBuffer = (LPSTR)malloc(dwSize + 1);
            if (!pszOutBuffer) {
                printf("Memoria insuficiente.\n");
                return;
            }

            ZeroMemory(pszOutBuffer, dwSize + 1);

            if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
                MessageBox(NULL, L"Error de red", L"GPTClip Error", MB_OK | MB_ICONERROR);
            }
            else {
                strncpy(server_version, pszOutBuffer, dwDownloaded);
                server_version[dwDownloaded] = '\0';
                clean_version(server_version);
            }

            free(pszOutBuffer);
        }
    }

    if (!bResults) {
        MessageBox(NULL, L"El servidor no tiene conexión!", L"Error de Red", MB_OK | MB_ICONERROR);
        exit(1);
    }

    if (strcmp(APP_VERSION, server_version) != 0) {
        wchar_t message[100];
        swprintf(message, 100, L"La versión %hs está disponible. Por favor, presiona Aceptar para actualizar la app.", server_version);

        CreateErrorWindow(message);
        exit(1);
    }

    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);
}



void CreateErrorWindow(LPCWSTR errorMessage) {
    int result = MessageBox(NULL, errorMessage, L"¡Actualización disponible!", MB_OKCANCEL | MB_ICONERROR);

    if (result == IDOK) {
        ShellExecute(NULL, L"open", L"https://github.com/otema666/gptClip/releases/latest/download/GPTClip.exe", NULL, NULL, SW_SHOWNORMAL);
    }

    exit(1);
}
