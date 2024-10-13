#include <windows.h>
#include <winhttp.h>
#include <stdio.h>
#include "resource.h"
#include "login.h"

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "winhttp.lib")
#define IDI_ICON2 130

HWND hUsernameInput;
HWND hPasswordInput;
HINSTANCE hInst;
BOOL isLoginSuccessful = FALSE;
char loggedInUsername[USERNAME_LEN] = "";

void CenterWindow(HWND hWnd) {
    RECT rc;
    GetWindowRect(hWnd, &rc);
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    SetWindowPos(hWnd, NULL, (screenWidth - width) / 2, (screenHeight - height) / 2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void SetStyles(HWND hWnd) {
    HDC hdc = GetDC(hWnd);
    RECT rect;
    GetClientRect(hWnd, &rect);
    HBRUSH brush = CreateSolidBrush(RGB(240, 240, 240));
    FillRect(hdc, &rect, brush);
    DeleteObject(brush);
    ReleaseDC(hWnd, hdc);
}

void CreateLoginWindow(HINSTANCE hInstance) {
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = LoginWndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"LoginWindow";
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));

    RegisterClassEx(&wc);

    HWND hWnd = CreateWindowEx(0, wc.lpszClassName, L"Iniciar Sesi�n", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 180,
        NULL, NULL, hInstance, NULL);

    CenterWindow(hWnd);

    SetStyles(hWnd);

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    SetFocus(hUsernameInput);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (isLoginSuccessful) {
            DestroyWindow(hWnd);
            break;
        }
    }
}

LRESULT CALLBACK LoginWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        CreateWindowW(L"STATIC", L"Usuario:", WS_VISIBLE | WS_CHILD | WS_GROUP,
            20, 20, 80, 20, hWnd, NULL, hInst, NULL);
        hUsernameInput = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL,
            110, 20, 150, 20, hWnd, (HMENU)ID_USERNAME, hInst, NULL);

        CreateWindowW(L"STATIC", L"Contrase�a:", WS_VISIBLE | WS_CHILD | WS_GROUP,
            20, 60, 80, 20, hWnd, NULL, hInst, NULL);
        hPasswordInput = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD | WS_TABSTOP | ES_AUTOHSCROLL,
            110, 60, 150, 20, hWnd, (HMENU)ID_PASSWORD, hInst, NULL);

        HWND hLoginButton = CreateWindowW(L"BUTTON", L"Iniciar sesi�n", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_TABSTOP,
            110, 100, 150, 30, hWnd, (HMENU)ID_LOGIN_BUTTON, hInst, NULL);
        SendMessage(hLoginButton, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_LOGIN_BUTTON) {
            EnableWindow(GetDlgItem(hWnd, ID_LOGIN_BUTTON), FALSE);
            SetWindowText(GetDlgItem(hWnd, ID_LOGIN_BUTTON), L"Verificando...");
            wchar_t username[USERNAME_LEN];
            wchar_t password[PASSWORD_LEN];

            GetWindowTextW(hUsernameInput, username, USERNAME_LEN);
            GetWindowTextW(hPasswordInput, password, PASSWORD_LEN);

            char usernameChar[USERNAME_LEN];
            char passwordChar[PASSWORD_LEN];
            char errorMsg[256]; // Para almacenar el mensaje de error

            WideCharToMultiByte(CP_UTF8, 0, username, -1, usernameChar, USERNAME_LEN, NULL, NULL);
            WideCharToMultiByte(CP_UTF8, 0, password, -1, passwordChar, PASSWORD_LEN, NULL, NULL);

            if (loginRequest(usernameChar, passwordChar, errorMsg)) {
                isLoginSuccessful = TRUE;
                PostMessage(hWnd, WM_CLOSE, 0, 0);
                return 0;
            }
            else {
                MessageBoxA(hWnd, errorMsg, "Error de inicio de sesi�n", MB_OK | MB_ICONERROR);
                EnableWindow(GetDlgItem(hWnd, ID_LOGIN_BUTTON), TRUE);
                SetWindowText(GetDlgItem(hWnd, ID_LOGIN_BUTTON), L"Iniciar sesi�n");
            }
        }
        break;

    case WM_DESTROY:
        if (!isLoginSuccessful) {
            exit(1);
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

BOOL loginRequest(const char* username, const char* password, char* errorMsg) {
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    BOOL isLoginSuccessful = FALSE;
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    BOOL bResults = FALSE;
    char data[USERNAME_LEN + PASSWORD_LEN + 50];

    hSession = WinHttpOpen(L"gptClip", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) {
        strcpy(errorMsg, "Error al iniciar la sesi�n HTTP");
        return FALSE;
    }

    hConnect = WinHttpConnect(hSession, L"otema666.ddns.net", INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) {
        strcpy(errorMsg, "Error al conectar con el servidor");
        WinHttpCloseHandle(hSession);
        return FALSE;
    }

    hRequest = WinHttpOpenRequest(hConnect, L"POST", L"/gptClip/login.php", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    if (!hRequest) {
        strcpy(errorMsg, "No se pudo abrir la solicitud HTTP");
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return FALSE;
    }

    snprintf(data, sizeof(data), "username=%s&password=%s", username, password);
    LPCWSTR headers = L"Content-Type: application/x-www-form-urlencoded";

    bResults = WinHttpSendRequest(hRequest, headers, (DWORD)-1L, (LPVOID)data, strlen(data), strlen(data), 0);
    if (!bResults) {
        DWORD errorCode = GetLastError();
        if (errorCode == ERROR_WINHTTP_CANNOT_CONNECT) {
            strcpy(errorMsg, "No se pudo conectar al servidor. Revisa tu conexi�n a Internet.");
        }
        else {
            strcpy(errorMsg, "Error al enviar la solicitud HTTP");
        }
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return FALSE;
    }

    bResults = WinHttpReceiveResponse(hRequest, NULL);
    if (bResults) {
        do {
            dwSize = 0;
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
                strcpy(errorMsg, "Error al consultar datos disponibles");
                break;
            }

            if (dwSize > 0) {
                pszOutBuffer = (LPSTR)malloc(dwSize + 1);
                if (!pszOutBuffer) {
                    strcpy(errorMsg, "Error de memoria");
                    break;
                }

                ZeroMemory(pszOutBuffer, dwSize + 1);

                if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
                    strcpy(errorMsg, "Error al leer los datos");
                }
                else {
                    pszOutBuffer[dwSize] = '\0';

                    if (strstr(pszOutBuffer, "\"status\":\"success\"")) {
                        isLoginSuccessful = TRUE;
                        strncpy(loggedInUsername, username, USERNAME_LEN);
                    }
                    else if (strstr(pszOutBuffer, "\"status\":\"user_not_found\"")) {
                        strcpy(errorMsg, "Usuario no encontrado");
                    }
                    else if (strstr(pszOutBuffer, "\"status\":\"password_error\"")) {
                        strcpy(errorMsg, "Contrase�a incorrecta");
                    }
                    else if (strstr(pszOutBuffer, "\"status\":\"already_logged_in\"")) {
                        strcpy(errorMsg, "Ya has iniciado sesi�n con esta cuenta");
                    }
                    else {
                        strcpy(errorMsg, "Error desconocido");
                    }
                }

                free(pszOutBuffer);
            }
        } while (dwSize > 0);
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return isLoginSuccessful;
}
