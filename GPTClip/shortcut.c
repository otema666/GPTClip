#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include "api.h"
#include "shortcut.h"
#include "ui.h"
#include "prompts/prompts.h"
#include "notifications.h"
#include "tray.h"
#pragma warning(disable : 4996)

wchar_t* respuesta = NULL;
time_t last_hotkey_time = 0;

wchar_t* get_paper_clip() {
    if (!OpenClipboard(NULL)) {
        return NULL;
    }

    HGLOBAL hGlob = GetClipboardData(CF_UNICODETEXT);
    if (hGlob == NULL) {
        CloseClipboard();
        return NULL;
    }

    wchar_t* pBuf = (wchar_t*)GlobalLock(hGlob);
    if (pBuf == NULL) {
        CloseClipboard();
        return NULL;
    }

    wchar_t* clipContent = _wcsdup(pBuf);

    GlobalUnlock(hGlob);
    CloseClipboard();

    return clipContent;
}

void seleccionOculta() {
    const wchar_t* script = L"document.head.appendChild(Object.assign(document.createElement('style'), { textContent: '::selection { background: rgba(0, 179, 255, 0.09); color: inherit; }' }));";
    post_paper_clip(script);
}

void inputBox() {
    const wchar_t* script = L"let container=document.createElement('div');container.style.position='fixed';container.style.top='50%';container.style.left='50%';container.style.width='140px';container.style.height='100px';container.style.opacity='0.4';container.style.backgroundColor='rgba(0, 0, 0, 0.5)';container.style.borderRadius='10px';container.style.marginTop='-90px';container.style.marginLeft='-150px';document.body.appendChild(container);let titleBar=document.createElement('div');titleBar.style.width='100%';titleBar.style.height='30px';titleBar.style.backgroundColor='gray';titleBar.style.cursor='move';titleBar.style.display='flex';titleBar.style.alignItems='center';titleBar.style.justifyContent='flex-end';titleBar.style.borderTopLeftRadius='10px';titleBar.style.borderTopRightRadius='10px';titleBar.style.boxShadow='0 2px 5px rgba(0, 0, 0, 0.5)';function createButton(label,onClick){let buttonContainer=document.createElement('div');buttonContainer.style.display='flex';buttonContainer.style.alignItems='center';buttonContainer.style.marginRight='10px';buttonContainer.style.borderRadius='5px';buttonContainer.style.boxShadow='0 2px 5px rgba(0, 0, 0, 0.3)';buttonContainer.style.overflow='hidden';let button=document.createElement('button');button.innerText=label;button.style.width='30px';button.style.height='30px';button.style.backgroundColor='transparent';button.style.border='none';button.style.color='white';button.style.cursor='pointer';button.style.fontSize='16px';button.style.padding='0';button.onclick=onClick;button.onmouseover=()=>button.style.color='lightgray';button.onmouseout=()=>button.style.color='white';buttonContainer.appendChild(button);return buttonContainer;}let minimizeButton=createButton('-',function(){if(textArea.style.display==='none'){textArea.style.display='block';container.style.height='100px';container.style.opacity='0.4';}else{textArea.style.display='none';container.style.height='30px';container.style.opacity='0.2';}});let closeButton=createButton('X',function(){document.body.removeChild(container);});titleBar.appendChild(minimizeButton);titleBar.appendChild(closeButton);let textArea=document.createElement('textarea');textArea.style.width='100%';textArea.style.height='calc(100% -30px)';textArea.style.border='none';textArea.style.padding='1px';textArea.style.boxSizing='border-box';textArea.style.color='black';textArea.placeholder='Escribe aquí...';textArea.style.resize='none';textArea.style.outline='none';textArea.spellcheck=false;container.appendChild(titleBar);container.appendChild(textArea);titleBar.onmousedown=function(event){let shiftX=event.clientX-container.getBoundingClientRect().left;let shiftY=event.clientY-container.getBoundingClientRect().top;function moveAt(pageX,pageY){container.style.left=pageX-shiftX+'px';container.style.top=pageY-shiftY+'px';container.style.marginLeft='0';container.style.marginTop='0';}function onMouseMove(event){moveAt(event.pageX,event.pageY);}document.addEventListener('mousemove',onMouseMove);document.onmouseup=function(){document.removeEventListener('mousemove',onMouseMove);document.onmouseup=null;};};titleBar.ondragstart=function(){return false;};function toggleContainer(){if(document.body.contains(container)){document.body.removeChild(container);}else{document.body.appendChild(container);}}document.addEventListener('keydown',function(event){if(event.ctrlKey&&event.shiftKey&&event.key==='F'){event.preventDefault();toggleContainer();}});container.onmouseenter=()=>{if(textArea.style.display==='none'){container.style.opacity='0.4';}};container.onmouseleave=()=>{if(textArea.style.display==='none'){container.style.opacity='0.2';}};";
    post_paper_clip(script);
}

void post_paper_clip(const wchar_t* text) {
    if (OpenClipboard(NULL)) {
        EmptyClipboard();

        HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, (wcslen(text) + 1) * sizeof(wchar_t));
        if (hGlob) {
            wchar_t* pGlobal = (wchar_t*)GlobalLock(hGlob);
            if (pGlobal) {
                wmemcpy(pGlobal, text, wcslen(text) + 1);
                GlobalUnlock(hGlob);
                SetClipboardData(CF_UNICODETEXT, hGlob);
            }
        }

        CloseClipboard();
    }
    else {
        MessageBoxW(NULL, L"No se pudo abrir el portapapeles.", L"Error", MB_OK | MB_ICONERROR);
    }
}


DWORD WINAPI monitor_keys() {
    MessageBoxW(NULL, L"La aplicacion esta preparada y ejecutada en segundo plano", L"Aplicacion lista", MB_OK);

    while (TRUE) {
        time_t current_time = time(NULL);

        if ((GetAsyncKeyState('0') & 0x8000) &&
            (GetAsyncKeyState(VK_CONTROL) & 0x8000) &&
            (GetAsyncKeyState(VK_MENU) & 0x8000) &&
            difftime(current_time, last_hotkey_time) >= 7) {

            wchar_t* question = get_paper_clip();
            wchar_t* prompt = get_prompt();
            LPSTR response = get_gpt_response(prompt, question);
            //post_paper_clip(response);

            if (response != NULL) {
                if (respuesta) {
                    free(respuesta);
                }

                respuesta = (wchar_t*)malloc((wcslen(response) + 1) * sizeof(wchar_t));
                if (respuesta) {
                    wcscpy(respuesta, response);
                    post_paper_clip(respuesta);
                    if (SendMessage(hSendNotificationsCheck, BM_GETCHECK, 0, 0) == BST_CHECKED) {
                        notificar(respuesta);
                    }
                }

                free(question);
            }

            last_hotkey_time = current_time;

            while ((GetAsyncKeyState('0') & 0x8000) &&
                (GetAsyncKeyState(VK_CONTROL) & 0x8000) &&
                (GetAsyncKeyState(VK_MENU) & 0x8000)) {
                Sleep(1000);
            }
        }

        Sleep(50);
    }

    if (respuesta) {
        free(respuesta);
    }

    return 0;
}

wchar_t* get_prompt() {
    wchar_t* promptMode = GetSelectedPromptMode();
    if (wcscmp(promptMode, L"Examen tipo test (Respuesta Múltiple)") == 0) {
        return prompt_test;
    }
    else if (wcscmp(promptMode, L"Examen tipo desarrollo (Respuestas más largas)") == 0) {
        return prompt_LongAnswer;
    }
    else if (wcscmp(promptMode, L"Desactivar prompt (Respuestas Normales)") == 0) {
        return no_prompt;
    }
    else {
        MessageBoxW(NULL, L"Seleccione un modo de solicitud válido", L"Prompt error", MB_ICONERROR | MB_OK);
        return L"no hay";
    }
}