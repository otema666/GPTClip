import asyncio
import sys
from duck_chat import DuckChat

async def main(prompt, pregunta):
    pregunta_chat = f"{prompt}\n{pregunta}"
    
    async with DuckChat() as chat:
        print(await chat.ask_question(pregunta_chat))
        await asyncio.sleep(1)


if __name__ == "__main__":
    prompt = sys.argv[1]
    pregunta = sys.argv[2]
    asyncio.run(main(prompt, pregunta))
    
    