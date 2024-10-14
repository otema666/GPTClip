#include <windows.h>
#include "notifications.h"

void notificar(wchar_t* str) {
    const size_t MAX_INFO_LENGTH = 256;
    const size_t MAX_TITLE_LENGTH = 64;

    if (wcslen(str) >= MAX_INFO_LENGTH) {
        return;
    }

    NOTIFYICONDATA nid = { 0 };
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = NULL;
    nid.uID = 100;
    nid.uFlags = NIF_INFO;

    wcscpy_s(nid.szInfo, MAX_INFO_LENGTH, str);
    wcscpy_s(nid.szInfoTitle, MAX_TITLE_LENGTH, L"Respuesta:");
    nid.dwInfoFlags = NIIF_INFO;

    Shell_NotifyIcon(NIM_ADD, &nid);
    Shell_NotifyIcon(NIM_MODIFY, &nid);

    Sleep(3000);

    Shell_NotifyIcon(NIM_DELETE, &nid);
}