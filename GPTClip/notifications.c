#include <windows.h>
#include "notifications.h"

void notificar(wchar_t* str) {
    NOTIFYICONDATA nid = { 0 };
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = NULL;
    nid.uID = 100;
    nid.uFlags = NIF_INFO;

    wcscpy_s(nid.szInfo, sizeof(nid.szInfo) / sizeof(nid.szInfo[0]), str);
    wcscpy_s(nid.szInfoTitle, sizeof(nid.szInfoTitle) / sizeof(nid.szInfoTitle[0]), L"Notificación");
    nid.dwInfoFlags = NIIF_INFO;

    Shell_NotifyIcon(NIM_ADD, &nid);

    Shell_NotifyIcon(NIM_MODIFY, &nid);

    Sleep(3000);

    Shell_NotifyIcon(NIM_DELETE, &nid);
}