#include <windows.h>
extern char* respuesta;
extern HWND hPromptModeComboBox;

wchar_t* get_paper_clip();
wchar_t* get_prompt();

void post_paper_clip(const char* text);

DWORD WINAPI monitor_keys();

