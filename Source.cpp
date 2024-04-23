#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

SDL_Window* win = NULL;
SDL_Renderer* render = NULL;

int win_width = 634, win_height = 955;

int x = 0;
int y = 0;

bool isRightPressed = false;
bool isLeftPressed = false;
bool isUpPressed = false;
bool isDownPressed = false;

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
		"dOODELgUMP",
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

void WorkWithKeys()
{ 
	//�������� � �������� ���������
	if (isUpPressed && !isDownPressed) y -= 5;
	if (!isUpPressed && isDownPressed) y += 5;
	if (isRightPressed && !isLeftPressed) x += 5;
	if (!isRightPressed && isLeftPressed) x -= 5;
}


int main(int arcg, char* argv[])
{
	system("chcp 1251");
	Init();

#pragma region MENU

	SDL_Surface* surface = IMG_Load("sprites/menu.png");
	
	if (surface == NULL)
	{
		printf("Couldn`t load menu! Error: %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}

	SDL_Surface* win_surf = SDL_GetWindowSurface(win);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
	
	SDL_Rect img_rect = { 0, 0, surface->w, surface->h };
	
	SDL_FreeSurface(surface);

#pragma endregion

	SDL_Event ev;
	bool isRunning = true;

	while (isRunning)
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT: //����� �� ���������� (�������� ����)
				isRunning = false;
				break;

			case SDL_MOUSEBUTTONDOWN: //��������� ������� ���
				if (ev.button.button == SDL_BUTTON_LEFT)
				{
					x = ev.button.x;
					y = ev.button.y;
				}
				break;

			case SDL_KEYDOWN: //��������� ������� ������ ����������
				switch (ev.key.keysym.scancode)
				{
				
				case SDL_SCANCODE_ESCAPE: //������� ESC
					isRunning = false;
					break;
				
				case SDL_SCANCODE_RIGHT: //������� ������� ������
					
					isRightPressed = true;
					break;
				case SDL_SCANCODE_LEFT: //������� ������� �����
					
					isLeftPressed = true;
					break;
				case SDL_SCANCODE_UP: //������� ������� �����
					
					isUpPressed = true;
					break;
				case SDL_SCANCODE_DOWN: //������� ������� ����
					
					isDownPressed = true;
					break;
				}
				break;

			case SDL_KEYUP: //��������� ���������� ������ ����������
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_RIGHT: //������� ������� ������
					isRightPressed = false;
					break;
				case SDL_SCANCODE_LEFT: //������� ������� �����
					isLeftPressed = false;
					break;
				case SDL_SCANCODE_UP: //������� ������� �����
					isUpPressed = false;
					break;
				case SDL_SCANCODE_DOWN: //������� ������� ����
					isDownPressed = false;
					break;
				}
				break;
			}
		}

		SDL_Rect dst_rect = { x, y, img_rect.w, img_rect.h };



		SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
		SDL_RenderClear(render);

		SDL_RenderCopy(render, texture, NULL, &dst_rect);
		SDL_RenderPresent(render);



		SDL_Delay(10);
	}



	SDL_DestroyTexture(texture);
	DeInit(0);
	return 0;
}