#include <windows.h>
#include <winhttp.h>
#include "api.h"
#include <tchar.h>
#pragma comment(lib, "winhttp.lib")

#define SERVER_NAME L"otema666.ddns.net"
#define SERVER_PORT 443
#pragma warning(disable : 4996)

/*
LPWSTR get_gpt_response(LPCWSTR prompt, LPCWSTR question) {
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    BOOL bResults = FALSE;
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer = NULL;
    DWORD dwStatusCode = 0;
    DWORD dwStatusCodeSize = sizeof(dwStatusCode);
    wchar_t* response = (wchar_t*)malloc(512 * sizeof(wchar_t));

    if (response == NULL) {
        MessageBox(NULL, L"Memoria insuficiente.", L"Error de memoria", MB_OK | MB_ICONERROR);
        return NULL;
    }

    size_t prompt_len = wcslen(prompt);
    size_t question_len = wcslen(question);
    size_t total_len = prompt_len + question_len + wcslen(L"gptClip/gptApi.php?prompt=&question=") + 1;

    wchar_t* path = (wchar_t*)malloc(total_len * sizeof(wchar_t));
    if (path == NULL) {
        MessageBox(NULL, L"Error al asignar memoria.", L"Error de asignación memoria", MB_OK | MB_ICONERROR);

        free(response);
        return NULL;
    }

    swprintf(path, total_len, L"gptClip/gptApi.php?prompt=%ls&question=%ls", prompt, question);

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
            MessageBox(NULL, L"Error al consultar los datos disponibles.", L"Error de datos", MB_OK | MB_ICONERROR);
        }
        else if (dwSize > 0) {
            pszOutBuffer = (LPSTR)malloc(dwSize + 1);
            if (!pszOutBuffer) {
                MessageBox(NULL, L"Memoria insuficiente.", L"Error de memoria", MB_OK | MB_ICONERROR);
                free(response);
                return NULL;
            }

            ZeroMemory(pszOutBuffer, dwSize + 1);

            if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
                MessageBox(NULL, L"Error al leer los datos.", L"Error", MB_OK | MB_ICONERROR);
                free(pszOutBuffer);
                free(response);
                return NULL;
            }
            else {
                int requiredSize = MultiByteToWideChar(CP_UTF8, 0, pszOutBuffer, dwDownloaded, NULL, 0);
                if (requiredSize > 0) {
                    wchar_t* wideResponse = (wchar_t*)malloc((requiredSize + 1) * sizeof(wchar_t));
                    if (!wideResponse) {
                        MessageBox(NULL, L"Memoria insuficiente para respuesta.", L"Error de memoria", MB_OK | MB_ICONERROR);
                        free(pszOutBuffer);
                        free(response);
                        return NULL;
                    }
                    MultiByteToWideChar(CP_UTF8, 0, pszOutBuffer, dwDownloaded, wideResponse, requiredSize);
                    wideResponse[requiredSize] = L'\0';

                    wcscpy(response, wideResponse);
                    free(wideResponse);
                }
                else {
                    MessageBox(NULL, L"Error al convertir los datos a wchar_t.", L"Error", MB_OK | MB_ICONERROR);
                }
            }

            free(pszOutBuffer);
            return response;
        }
    }
    else {
        MessageBox(NULL, L"No se pudo conectar con el servidor.", L"Error", MB_OK | MB_ICONERROR);
        free(response);
        return NULL;
    }

    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);
    free(response);
    return NULL;
}
*/


LPWSTR get_gpt_response(LPCWSTR prompt, LPCWSTR question) {
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    BOOL bResults = FALSE;
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer = NULL;
    DWORD dwStatusCode = 0;
    DWORD dwStatusCodeSize = sizeof(dwStatusCode);
    wchar_t* response = NULL;
    size_t responseLen = 0;

    size_t prompt_len = wcslen(prompt);
    size_t question_len = wcslen(question);
    size_t total_len = prompt_len + question_len + wcslen(L"gptClip/gptApi.php?prompt=&question=") + 1;

    wchar_t* path = (wchar_t*)malloc(total_len * sizeof(wchar_t));
    if (path == NULL) {
        MessageBox(NULL, L"Error al asignar memoria.", L"Error de asignación memoria", MB_OK | MB_ICONERROR);
        return NULL;
    }

    swprintf(path, total_len, L"gptClip/gptApi.php?prompt=%ls&question=%ls", prompt, question);

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
        do {
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
                MessageBox(NULL, L"Error al consultar los datos disponibles.", L"Error de datos", MB_OK | MB_ICONERROR);
                free(response);
                response = NULL;
                break;
            }

            if (dwSize > 0) {
                pszOutBuffer = (LPSTR)malloc(dwSize + 1);
                if (!pszOutBuffer) {
                    MessageBox(NULL, L"Memoria insuficiente.", L"Error de memoria", MB_OK | MB_ICONERROR);
                    free(response);
                    response = NULL;
                    break;
                }

                ZeroMemory(pszOutBuffer, dwSize + 1);

                if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
                    MessageBox(NULL, L"Error al leer los datos.", L"Error", MB_OK | MB_ICONERROR);
                    free(pszOutBuffer);
                    free(response);
                    response = NULL;
                    break;
                }

                // Convertir el buffer leído a wchar_t
                int requiredSize = MultiByteToWideChar(CP_UTF8, 0, pszOutBuffer, dwDownloaded, NULL, 0);
                if (requiredSize > 0) {
                    wchar_t* tempResponse = (wchar_t*)realloc(response, (responseLen + requiredSize + 1) * sizeof(wchar_t));
                    if (!tempResponse) {
                        MessageBox(NULL, L"Memoria insuficiente para respuesta.", L"Error de memoria", MB_OK | MB_ICONERROR);
                        free(pszOutBuffer);
                        free(response);
                        response = NULL;
                        break;
                    }
                    response = tempResponse;

                    MultiByteToWideChar(CP_UTF8, 0, pszOutBuffer, dwDownloaded, response + responseLen, requiredSize);
                    responseLen += requiredSize;
                    response[responseLen] = L'\0';  // Null terminator
                }
                else {
                    MessageBox(NULL, L"Error al convertir los datos a wchar_t.", L"Error", MB_OK | MB_ICONERROR);
                }

                free(pszOutBuffer);
            }
        } while (dwSize > 0);

    }
    else {
        MessageBox(NULL, L"No se pudo conectar con el servidor.", L"Error", MB_OK | MB_ICONERROR);
    }

    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);
    free(path);

    return response;
}
