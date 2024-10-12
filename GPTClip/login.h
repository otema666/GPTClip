#ifndef LOGIN_H
#define LOGIN_H


#define USERNAME_LEN 50
#define PASSWORD_LEN 50
#define ID_USERNAME 101
#define ID_PASSWORD 102
#define ID_LOGIN_BUTTON 103

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS


extern BOOL isLoginSuccessful;
extern char loggedInUsername[USERNAME_LEN];

LRESULT CALLBACK LoginWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL loginRequest(const char* username, const char* password);
void CenterWindow(HWND hWnd);

#endif // LOGIN_H
