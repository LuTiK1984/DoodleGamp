#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

SDL_Window* win = NULL;
SDL_Renderer* render = NULL;

int win_width = 800, win_height = 600;
double sx;
double sy;

int sixSize = 100;
int radius[1000] = { 0 };

int x = 0;
int y = 0;


bool isRightPressed = false;
bool isLeftPressed = false;
bool isUpPressed = false;
bool isDownPressed = false;

double angle = 0;

void DeInit(int error)
{
	if (render != NULL) SDL_DestroyRenderer(render);
	if (win != NULL) SDL_DestroyWindow(win);
	IMG_Quit();
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

	int res = IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	if (res == 0)
	{
		printf("Init Error! - %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}

	if (res & IMG_INIT_PNG) printf("Inintialized PNG library.\n"); else printf("Inintialize failed PNG library.\n");
	if (res & IMG_INIT_JPG) printf("Inintialized JPG library.\n"); else printf("Inintialize failed JPG library.\n");

	win = SDL_CreateWindow(
		"KVAJABLYABUSHKA",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		win_width,
		win_height,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
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

void WorkWithKeys()
{
	//Действия с зажатыми клавишами
	if (isUpPressed && !isDownPressed) y += 5;
	if (!isUpPressed && isDownPressed) y -= 5;
	if (isRightPressed && !isLeftPressed) x += 5;
	if (!isRightPressed && isLeftPressed) x -= 5;
}


int main(int arcg, char* argv[])
{
	system("chcp 1251");
	Init();

	SDL_Surface* surface = IMG_Load("jaba.jpg");
	if (surface == NULL)
	{
		printf("Couldn`t load Jaba! Error: %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}

	SDL_Surface* win_surf = SDL_GetWindowSurface(win);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
	SDL_Rect img_rect = { 0, 0, surface->w, surface->h };
	SDL_FreeSurface(surface);

	SDL_Event ev;
	bool isRunning = true;


	SDL_Rect dst_rect = { 0, 0, 0, 0 };

	while (isRunning)
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT: //Выход из приложения (закрытие окна)
				isRunning = false;
				break;

			case SDL_WINDOWEVENT:
				if (ev.window.event == SDL_WINDOWEVENT_RESIZED) //Изменения размеров окна
				{
					win_width = ev.window.data1;
					win_height = ev.window.data2;
				}
				break;

			case SDL_MOUSEBUTTONDOWN: //Обработка зажатия ЛКМ
				if (ev.button.button == SDL_BUTTON_LEFT)
				{
					x = ev.button.x;
					y = ev.button.y;
				}
				break;

			case SDL_KEYDOWN: //Обработка зажатия клавиш клавиатуры
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE: //Клавиша ESC
					isRunning = false;
					break;
				case SDL_SCANCODE_RIGHT: //Клавиша стрелка вправо
					x += 10;
					//isRightPressed = true;
					break;
				case SDL_SCANCODE_LEFT: //Клавиша стрелка влево
					x -= 10;
					//isLeftPressed = true;
					break;
				case SDL_SCANCODE_UP: //Клавиша стрелка вверх
					y -= 10;
					//isUpPressed = true;
					break;
				case SDL_SCANCODE_DOWN: //Клавиша стрелка вниз
					y += 10;
					//isDownPressed = true;
					break;
				case SDL_SCANCODE_PAGEUP:
					angle += 15;
					break;
				case SDL_SCANCODE_PAGEDOWN:
					angle -= 15;
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
			}
		}

		dst_rect = { x, y, img_rect.w, img_rect.h };

		SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
		SDL_RenderClear(render);

		//SDL_RenderCopy(render, texture, NULL, &dst_rect);
		SDL_RenderCopyEx(render, texture, NULL, &dst_rect, angle, NULL, SDL_FLIP_NONE);
		SDL_RenderPresent(render);



		SDL_Delay(10);
	}



	SDL_DestroyTexture(texture);
	DeInit(0);
	return 0;
}