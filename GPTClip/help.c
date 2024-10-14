#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shlobj.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
#include "help.h"

#include <windows.h>
#include <wininet.h>
#include <stdio.h>

void launch_help_image(const char* url, const char* filename) {
    HINTERNET hInternet, hConnect;
    FILE* fp;
    DWORD bytesRead;
    BYTE buffer[4096];

    hInternet = InternetOpen(L"Help Downloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        MessageBox(NULL, L"InternetOpen failed", L"Error", MB_OK | MB_ICONERROR);
        return;
    }
    hConnect = InternetOpenUrlA(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hConnect) {
        MessageBox(NULL, L"InternetOpenUrlA failed", L"Error", MB_OK | MB_ICONERROR);
        InternetCloseHandle(hInternet);
        return;
    }

    errno_t err = fopen_s(&fp, filename, "wb");
    if (err != 0) {
        MessageBox(NULL, L"Failed to open file", L"Error", MB_OK | MB_ICONERROR);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return;
    }

    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        fwrite(buffer, 1, bytesRead, fp);
    }

    fclose(fp);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    // Show the image
    ShellExecuteA(NULL, "open", filename, NULL, NULL, SW_SHOWNORMAL);

    Sleep(1200); // Wait for 1.2 seconds before deleting
    DeleteFileA(filename);
}

// Funciones específicas para cada imagen
void launch_help_img() {
    launch_help_image("https://otema666.ddns.net/gptClip/help.jpg", "HELP.jpg");
}

void launch_helpSeleccion_img() {
    launch_help_image("https://otema666.ddns.net/gptClip/helpSeleccion.jpg", "HELP.jpg");
}

void launch_InputBox_img() {
    launch_help_image("https://otema666.ddns.net/gptClip/helpInputBox.jpg", "HELP.jpg");
}
