#include <string.h>
#include "prompts.h"

const wchar_t* prompt_test = L"Analiza detalladamente el siguiente problema tipo test y determina cuál es la respuesta correcta. Tu formato de respuesta será así: letra_correcta + ) + identificacion_respuesta, por lo que no dirás cual es el procedimiento de resolución.\nEste es el Problema:\n";
const wchar_t* prompt_LongAnswer = L"Analiza el siguiente problema y escribe una respuesta detallada y completa. Recuerda que la respuesta debe ser coherente y estar bien redactada. Si se trata de un problema matemático, tus respuestas no estarán en formato LaTeX, estarán en texto.\nProblema:\n";
const wchar_t* no_prompt = L"";