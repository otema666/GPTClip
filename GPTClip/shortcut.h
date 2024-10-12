#include <windows.h>
extern wchar_t* respuesta;
extern HWND hPromptModeComboBox;

wchar_t* get_paper_clip();
wchar_t* get_prompt();

void post_paper_clip(const wchar_t* text);

DWORD WINAPI monitor_keys();

