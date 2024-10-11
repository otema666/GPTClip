#pragma once

LPWSTR get_gpt_response(LPCWSTR prompt, LPCWSTR question);

char* url_encode(const wchar_t* str);
