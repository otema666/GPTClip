#include <windows.h>
#include <winhttp.h>
#include <stdio.h>
#include <string.h>
#include "updater.h"
#include <tchar.h>
#include <psapi.h>

#pragma comment(lib, "winhttp.lib")

#define APP_VERSION "0.1"
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

    hSession = WinHttpOpen(L"gptClip updater", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    if (hSession) {
        hConnect = WinHttpConnect(hSession, SERVER_NAME, SERVER_PORT, 0);
    }

    if (hConnect) {
        hRequest = WinHttpOpenRequest(hConnect, L"GET", SERVER_PATH, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    }

    if (hRequest) {
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
            CreateErrorWindow(L"Error al consultar los datos disponibles.");
        }
        else if (dwSize > 0) {
            pszOutBuffer = (LPSTR)malloc(dwSize + 1);
            if (!pszOutBuffer) {
                printf("Memoria insuficiente.\n");
                return;
            }

            ZeroMemory(pszOutBuffer, dwSize + 1);

            if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
                CreateErrorWindow(L"Error al leer los datos.");
            }
            else {
                strncpy(server_version, pszOutBuffer, dwDownloaded);
                server_version[dwDownloaded] = '\0';
                clean_version(server_version);
            }

            free(pszOutBuffer);
        }
    }
    else {
        CreateErrorWindow(L"Error: no se pudo conectar con el servidor.");
        exit(1);
    }

    if (strcmp(APP_VERSION, server_version) != 0) {
		wchar_t message[100];
		swprintf(message, 100, L"La versi�n %hs est� disponible. Por favor, actualiza la app.", server_version);

		CreateErrorWindow(message);
        exit(1);
    }

    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);
}



void CreateErrorWindow(LPCWSTR errorMessage) {
    int result = MessageBox(NULL, errorMessage, L"�Actualizaci�n disponible!", MB_OK | MB_ICONERROR);
    if (result == IDOK) {
        ShellExecute(NULL, L"open", L"https://github.com/otema666/gptClip/releases/latest/download/gptClip.exe", NULL, NULL, SW_SHOWNORMAL);
    }
}