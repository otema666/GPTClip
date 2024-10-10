#include <windows.h>
#include <winhttp.h>
#include "api.h"
#include <tchar.h>
#pragma comment(lib, "winhttp.lib")

#define SERVER_NAME L"otema666.ddns.net"
#define SERVER_PORT 443
#pragma warning(disable : 4996)

LPSTR get_gpt_response(LPCWSTR prompt, LPCWSTR question) {
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    BOOL bResults = FALSE;
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer = NULL;
    DWORD dwStatusCode = 0;
    DWORD dwStatusCodeSize = sizeof(dwStatusCode);
    char* response = (char*)malloc(256);

    if (response == NULL) {
        CreateErrorWindow(L"Memoria insuficiente.");
        return NULL;
    }

    wchar_t path[256];
    swprintf(path, 256, L"gptClip/gptApi.php?prompt=%s&question=%s", prompt, question);

    hSession = WinHttpOpen(L"gptClip", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    if (hSession) {
        hConnect = WinHttpConnect(hSession, SERVER_NAME, SERVER_PORT, 0);
    }

    if (hConnect) {
        hRequest = WinHttpOpenRequest(hConnect, L"GET", path, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
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
                CreateErrorWindow(L"Memoria insuficiente.");
                free(response);
                return NULL;
            }

            ZeroMemory(pszOutBuffer, dwSize + 1);

            if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
                CreateErrorWindow(L"Error al leer los datos.");
                free(pszOutBuffer);
                free(response);
                return NULL;
            }
            else {
                strncpy(response, pszOutBuffer, dwDownloaded);
                response[dwDownloaded] = '\0';
            }

            free(pszOutBuffer);
            return response;
        }
    }
    else {
        CreateErrorWindow(L"Error: no se pudo conectar con el servidor.");
        free(response);
        return NULL;
    }

    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);
    free(response);
    return NULL;
}
