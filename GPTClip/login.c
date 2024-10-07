#include "login.h"
#include <windows.h>
#include <stdio.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

#define ID_USERNAME 101
#define ID_PASSWORD 102
#define ID_LOGIN_BUTTON 103
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

// Definiciones de tamaño de buffer
#define USERNAME_LEN 50
#define PASSWORD_LEN 50
#define RESPONSE_LEN 256

#define SERVER_URL "https://otema666.ddns.net/gptClip/login.php"

HWND hUsernameInput, hPasswordInput;
HINSTANCE hInst;
BOOL isLoginSuccessful = FALSE; // Variable para indicar si el login fue exitoso

// Prototipo de la función para manejar la ventana
LRESULT CALLBACK LoginWndProc(HWND, UINT, WPARAM, LPARAM);

// Función para enviar la solicitud de inicio de sesión
BOOL loginRequest(const char* username, const char* password);

// Función para crear la ventana de login
void CreateLoginWindow(HINSTANCE hInstance) {
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = LoginWndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"LoginWindow";

    RegisterClassEx(&wc);

    HWND hWnd = CreateWindowEx(0, wc.lpszClassName, L"Iniciar Sesión", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 180,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        // Verificamos si el login fue exitoso y cerramos la ventana de login
        if (isLoginSuccessful) {
            DestroyWindow(hWnd);  // Solo cerramos la ventana de login
            break;
        }
    }
}

// Ventana de procesamiento de mensajes
LRESULT CALLBACK LoginWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        CreateWindowW(L"STATIC", L"Usuario:", WS_VISIBLE | WS_CHILD,
            20, 20, 80, 20, hWnd, NULL, hInst, NULL);
        hUsernameInput = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
            110, 20, 150, 20, hWnd, (HMENU)ID_USERNAME, hInst, NULL);

        CreateWindowW(L"STATIC", L"Contraseña:", WS_VISIBLE | WS_CHILD,
            20, 60, 80, 20, hWnd, NULL, hInst, NULL);
        hPasswordInput = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
            110, 60, 150, 20, hWnd, (HMENU)ID_PASSWORD, hInst, NULL);

        CreateWindowW(L"BUTTON", L"Login", WS_VISIBLE | WS_CHILD | WS_BORDER,
            110, 100, 150, 30, hWnd, (HMENU)ID_LOGIN_BUTTON, hInst, NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_LOGIN_BUTTON) {
            wchar_t username[USERNAME_LEN];
            wchar_t password[PASSWORD_LEN];

            GetWindowTextW(hUsernameInput, username, USERNAME_LEN);
            GetWindowTextW(hPasswordInput, password, PASSWORD_LEN);

            // Convertimos las credenciales de wide char a char
            char usernameC[USERNAME_LEN], passwordC[PASSWORD_LEN];
            wcstombs(usernameC, username, USERNAME_LEN);
            wcstombs(passwordC, password, PASSWORD_LEN);

            // Llamada a la función que envía los datos de login al servidor PHP
            if (loginRequest(usernameC, passwordC)) {
                isLoginSuccessful = TRUE;  // Login exitoso
				return 0;
            }
            else {
                MessageBox(hWnd, L"Credenciales incorrectas", L"Error", MB_OK | MB_ICONERROR);

            }
        }
        break;

    case WM_DESTROY:
        if (isLoginSuccessful) {
            break;
        }
        else {
			exit(1);
        }
            
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

BOOL loginRequest(const char* username, const char* password) {
	return TRUE;
}
