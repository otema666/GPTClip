#include <windows.h>
#include <winhttp.h>
#include "api.h"
#include <tchar.h>
#pragma comment(lib, "winhttp.lib")

#define SERVER_NAME L"otema666.ddns.net"
#define SERVER_PORT 443
#pragma warning(disable : 4996)

LPWSTR get_gpt_response(LPCWSTR prompt, LPCWSTR question) {
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    BOOL bResults = FALSE;
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer = NULL;
    DWORD dwStatusCode = 0;
    DWORD dwStatusCodeSize = sizeof(dwStatusCode);
    wchar_t* response = (wchar_t*)malloc(512 * sizeof(wchar_t)); // Ajusta el tamaño si es necesario

    if (response == NULL) {
        MessageBox(NULL, L"Memoria insuficiente.", L"Error de memoria", MB_OK | MB_ICONERROR);
        return NULL;
    }

    size_t prompt_len = wcslen(prompt);
    size_t question_len = wcslen(question);
    size_t total_len = prompt_len + question_len + wcslen(L"gptClip/gptApi.php?prompt=&question=") + 1; // +1 para el terminador nulo

    // Asignar memoria dinámica
    wchar_t* path = (wchar_t*)malloc(total_len * sizeof(wchar_t));
    if (path == NULL) {
        MessageBox(NULL, L"Error al asignar memoria.", L"Error de asignación memoria", MB_OK | MB_ICONERROR);

        free(response);
        return NULL; // Manejo de error
    }

    // Formatear la cadena
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
                // Convertir de UTF-8 (char*) a UTF-16 (wchar_t*)
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

                    // Copiar al buffer original
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

char* url_encode(const wchar_t* str) {
    const char* hex = "0123456789ABCDEF";
    size_t len = wcslen(str);
    char* encoded = (char*)malloc(len * 3 + 1); // Cada carácter puede ocupar hasta 3 bytes
    if (!encoded) return NULL;

    char* p = encoded;
    for (size_t i = 0; i < len; i++) {
        wchar_t c = str[i];
        if ((c >= L'A' && c <= L'Z') || (c >= L'a' && c <= L'z') || (c >= L'0' && c <= L'9')) {
            *p++ = (char)c; // Caracteres seguros se copian directamente
        }
        else {
            *p++ = '%';
            *p++ = hex[(c >> 4) & 0xF];
            *p++ = hex[c & 0xF];
        }
    }
    *p = '\0'; // Terminación nula
    return encoded;
}
