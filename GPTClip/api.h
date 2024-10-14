#pragma once

LPWSTR get_gpt_response(LPCWSTR prompt, LPCWSTR question);

wchar_t* url_encode(const wchar_t* src);