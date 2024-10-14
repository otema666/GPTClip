#include <windows.h>
#include <winhttp.h>
#include "api.h"
#include <tchar.h>
#pragma comment(lib, "winhttp.lib")

#define SERVER_NAME L"otema666.ddns.net"
#define SERVER_PORT 443
#pragma warning(disable : 4996)

wchar_t* url_encode(const wchar_t* src) {
    size_t len = wcslen(src);
    wchar_t* encoded = (wchar_t*)malloc(len * 3 * sizeof(wchar_t)); // tamaño suficiente para caracteres codificados
    if (!encoded) return NULL;

    wchar_t* dst = encoded;
    for (size_t i = 0; i < len; i++) {
        if (iswalnum(src[i])) {
            *dst++ = src[i];
        }
        else {
            // Codifica el carácter como %XX
            swprintf(dst, 4, L"%%%02X", src[i]);
            dst += 3;
        }
    }
    *dst = L'\0';  // Terminador nulo

    return encoded;
}

LPWSTR get_gpt_response(LPCWSTR prompt, LPCWSTR question) {
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    BOOL bResults = FALSE;
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer = NULL;
    wchar_t* response = NULL;
    size_t responseLen = 0;

    wchar_t* headers = L"Content-Type: application/x-www-form-urlencoded";
    wchar_t* postDataFormat = L"prompt=%s&question=%s";

    // Codificar los parámetros prompt y question
    wchar_t* encodedPrompt = url_encode(prompt);
    wchar_t* encodedQuestion = url_encode(question);

    if (!encodedPrompt || !encodedQuestion) {
        MessageBox(NULL, L"Error al codificar los datos.", L"Error", MB_OK | MB_ICONERROR);
        free(encodedPrompt);
        free(encodedQuestion);
        return NULL;
    }

    size_t total_len = wcslen(encodedPrompt) + wcslen(encodedQuestion) + wcslen(L"prompt=&question=") + 1;
    wchar_t* postData = (wchar_t*)malloc(total_len * sizeof(wchar_t));
    if (postData == NULL) {
        MessageBox(NULL, L"Error al asignar memoria para el cuerpo de la solicitud.", L"Error de asignación", MB_OK | MB_ICONERROR);
        free(encodedPrompt);
        free(encodedQuestion);
        return NULL;
    }

    // Crear el cuerpo de datos
    swprintf(postData, total_len, postDataFormat, encodedPrompt, encodedQuestion);

    // Convertir el cuerpo a UTF-8 antes de enviarlo
    int postDataSize = WideCharToMultiByte(CP_UTF8, 0, postData, -1, NULL, 0, NULL, NULL);
    char* postDataUTF8 = (char*)malloc(postDataSize);
    WideCharToMultiByte(CP_UTF8, 0, postData, -1, postDataUTF8, postDataSize, NULL, NULL);

    hSession = WinHttpOpen(L"gptClip", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    if (hSession) {
        hConnect = WinHttpConnect(hSession, L"otema666.ddns.net", 443, 0);
    }

    if (hConnect) {
        hRequest = WinHttpOpenRequest(hConnect, L"POST", L"/gptClip/gptApi.php", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    }

    if (hRequest) {
        // Enviar los datos en formato UTF-8
        bResults = WinHttpSendRequest(hRequest, headers, (DWORD)-1, postDataUTF8, postDataSize - 1, postDataSize - 1, 0);
    }

    if (bResults) {
        bResults = WinHttpReceiveResponse(hRequest, NULL);
    }

    if (bResults) {
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
    free(postData);
    free(postDataUTF8);
    free(encodedPrompt);
    free(encodedQuestion);

    return response;
}

