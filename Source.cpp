#include <iostream>
#include <SDL.h>

SDL_Window* win = NULL;
SDL_Renderer* render = NULL;

int win_width = 400, win_height = 800;


void DeInit(int error)
{
	if (render != NULL) SDL_DestroyRenderer(render);
	if (win != NULL) SDL_DestroyWindow(win);
	SDL_Quit();
	exit(error);
}

void Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("Init Error! - %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}

	win = SDL_CreateWindow(
		"DoodelGump",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		win_width,
		win_height,
		SDL_WINDOW_SHOWN
	);

	if (win == NULL)
	{
		printf("Window Error! - %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}

	render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	if (render == NULL)
	{
		printf("Renderer Error! - %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}
}



int main(int arcg, char* argv[])
{
	srand(time(NULL));
	system("chcp 1251");
	Init();
	SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);

	SDL_Event ev;
	
	bool isRunning = true;
	
	bool isUpPressed = false;
	bool isDownPressed = false;
	bool isRightPressed = false;
	bool isLeftPressed = false;
	
	while (isRunning)
	{

#pragma region EVENTS
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{

			case SDL_QUIT: //Выход из приложения
			
				isRunning = false;
				break;

			case SDL_KEYDOWN: //Обработка зажатия клавиш клавиатуры
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_RIGHT: //Клавиша стрелка вправо
					isRightPressed = true;
					break;
				case SDL_SCANCODE_LEFT: //Клавиша стрелка влево
					isLeftPressed = true;
					break;
				case SDL_SCANCODE_UP: //Клавиша стрелка вверх
					isUpPressed = true;
					break;
				case SDL_SCANCODE_DOWN: //Клавиша стрелка вниз
					isDownPressed = true;
					break;
				}
				break;

			case SDL_KEYUP: //Обработка отпускания клавиш клавиатуры
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_RIGHT: //Клавиша стрелка вправо
					isRightPressed = false;
					break;
				case SDL_SCANCODE_LEFT: //Клавиша стрелка влево
					isLeftPressed = false;
					break;
				case SDL_SCANCODE_UP: //Клавиша стрелка вверх
					isUpPressed = false;
					break;
				case SDL_SCANCODE_DOWN: //Клавиша стрелка вниз
					isDownPressed = false;
					break;
				}
				break;
				
			/*case SDL_MOUSEBUTTONDOWN:
			{
				
			}*/
			}
		}

#pragma endregion


#pragma region LOGIC
		

		/*if (isUpPressed && !isDownPressed && block.y > 0) block.y -= 2;
		if (!isUpPressed && isDownPressed && ((block.y + block.h) < win_height)) block.y += 2;
		if (isRightPressed && !isLeftPressed && ((block.x + block.w) < win_width)) block.x += 2;
		if (!isRightPressed && isLeftPressed && block.x > 0) block.x -= 2;*/

#pragma endregion


#pragma region DRAWING
		SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
		SDL_RenderClear(render);



		SDL_Delay(10);

#pragma endregion

	}

	DeInit(0);
	return 0;
}