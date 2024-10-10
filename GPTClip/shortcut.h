#include <windows.h>
extern char* respuesta;
extern HWND hPromptModeComboBox;

char* get_paper_clip();
char* get_prompt();

void post_paper_clip(const char* text);

DWORD WINAPI monitor_keys();

