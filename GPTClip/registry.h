#pragma once
#include <windows.h>

#define REGISTRY_KEY L"Software\\GPTClip"
#define REGISTRY_APIKEY L"APIKey"

void SaveAPIKeyToRegistry(const WCHAR* apiKey);
void LoadAPIKeyFromRegistry(HWND hApiKeyInput);
