#ifndef LOGIN_H
#define LOGIN_H

#include <windows.h>

void CreateLoginWindow(HINSTANCE hInstance);
BOOL loginRequest(const char* username, const char* password);

// Variables globales
extern HWND hUsernameInput, hPasswordInput;
extern BOOL isLoginSuccessful;

#endif // LOGIN_H
