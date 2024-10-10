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
char loggedInUsername[USERNAME_LEN] = "";  // Definición de la variable

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

    HWND hWnd = CreateWindowEx(0, wc.lpszClassName, L"Iniciar Sesión", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
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

        CreateWindowW(L"STATIC", L"Contraseña:", WS_VISIBLE | WS_CHILD | WS_GROUP,
            20, 60, 80, 20, hWnd, NULL, hInst, NULL);
        hPasswordInput = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD | WS_TABSTOP | ES_AUTOHSCROLL,
            110, 60, 150, 20, hWnd, (HMENU)ID_PASSWORD, hInst, NULL);

        HWND hLoginButton = CreateWindowW(L"BUTTON", L"Iniciar sesión", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_TABSTOP,
            110, 100, 150, 30, hWnd, (HMENU)ID_LOGIN_BUTTON, hInst, NULL);
        SendMessage(hLoginButton, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_LOGIN_BUTTON) {
            wchar_t username[USERNAME_LEN];
            wchar_t password[PASSWORD_LEN];

            GetWindowTextW(hUsernameInput, username, USERNAME_LEN);
            GetWindowTextW(hPasswordInput, password, PASSWORD_LEN);

            char usernameChar[USERNAME_LEN];
            char passwordChar[PASSWORD_LEN];

            WideCharToMultiByte(CP_UTF8, 0, username, -1, usernameChar, USERNAME_LEN, NULL, NULL);
            WideCharToMultiByte(CP_UTF8, 0, password, -1, passwordChar, PASSWORD_LEN, NULL, NULL);

            if (loginRequest(usernameChar, passwordChar)) {
                isLoginSuccessful = TRUE;
                PostMessage(hWnd, WM_CLOSE, 0, 0);
                return 0;
            }
            else {
                MessageBox(hWnd, L"Credenciales incorrectas", L"Error", MB_OK | MB_ICONERROR);
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

BOOL loginRequest(const char* username, const char* password) {
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    BOOL isLoginSuccessful = FALSE;
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    BOOL bResults = FALSE;
    char data[USERNAME_LEN + PASSWORD_LEN + 50];

    hSession = WinHttpOpen(L"gptClip", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
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

    hRequest = WinHttpOpenRequest(hConnect, L"POST", L"/gptClip/login.php", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    if (!hRequest) {
        MessageBox(NULL, L"No se pudo abrir la solicitud HTTP", L"Error", MB_OK | MB_ICONERROR);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return FALSE;
    }

    snprintf(data, sizeof(data), "username=%s&password=%s", username, password);

    LPCWSTR headers = L"Content-Type: application/x-www-form-urlencoded";

    bResults = WinHttpSendRequest(hRequest, headers, (DWORD)-1L, (LPVOID)data, strlen(data), strlen(data), 0);
    if (bResults) {
        bResults = WinHttpReceiveResponse(hRequest, NULL);

        if (bResults) {
            do {
                dwSize = 0;
                if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
                    MessageBox(NULL, L"Error al consultar datos disponibles", L"Error", MB_OK | MB_ICONERROR);
                    break;
                }

                if (dwSize > 0) {
                    pszOutBuffer = (LPSTR)malloc(dwSize + 1);
                    if (!pszOutBuffer) {
                        MessageBox(NULL, L"Error de memoria", L"Error", MB_OK | MB_ICONERROR);
                        break;
                    }

                    ZeroMemory(pszOutBuffer, dwSize + 1);

                    if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
                        MessageBox(NULL, L"Error al leer los datos", L"Error", MB_OK | MB_ICONERROR);
                    }
                    else {
                        pszOutBuffer[dwSize] = '\0';

                        if (strstr(pszOutBuffer, "\"status\":\"success\"")) {
                            isLoginSuccessful = TRUE;
                            strncpy(loggedInUsername, username, USERNAME_LEN);

                        }
                        
                    }

                    free(pszOutBuffer);
                }
            } while (dwSize > 0);
        }
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return isLoginSuccessful;
}
