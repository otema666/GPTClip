#include "registry.h"

void SaveAPIKeyToRegistry(const WCHAR* apiKey) {
    HKEY hKey;
    if (RegCreateKeyExW(HKEY_CURRENT_USER, REGISTRY_KEY, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        RegSetValueExW(hKey, REGISTRY_APIKEY, 0, REG_SZ, (BYTE*)apiKey, (lstrlenW(apiKey) + 1) * sizeof(WCHAR));
        RegCloseKey(hKey);
    }
}

void LoadAPIKeyFromRegistry(HWND hApiKeyInput) {
    HKEY hKey;
    WCHAR apiKey[256] = L"";
    DWORD bufferSize = sizeof(apiKey);

    if (RegOpenKeyExW(HKEY_CURRENT_USER, REGISTRY_KEY, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        RegQueryValueExW(hKey, REGISTRY_APIKEY, NULL, NULL, (LPBYTE)apiKey, &bufferSize);
        SetWindowTextW(hApiKeyInput, apiKey);
        RegCloseKey(hKey);
    }
}
