#include <string.h>
#include "prompts.h"

const wchar_t* prompt_test = L"Analiza detalladamente el siguiente problema tipo test y determina cu�l es la respuesta correcta. Tu formato de respuesta ser� as�: letra_correcta + ) + identificacion_respuesta, por lo que no dir�s cual es el procedimiento de resoluci�n.\nEste es el Problema:\n";
const wchar_t* prompt_LongAnswer = L"Analiza el siguiente problema y escribe una respuesta detallada y completa. Recuerda que la respuesta debe ser coherente y estar bien redactada. Si se trata de un problema matem�tico, tus respuestas no estar�n en formato LaTeX, estar�n en texto.\nProblema:\n";
const wchar_t* no_prompt = L"";