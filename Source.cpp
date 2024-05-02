#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <Menu.h>
#include <Game.h>

#pragma region GLOBALINIT

SDL_Window* win = NULL;
SDL_Renderer* render = NULL;

Player player;


int win_width = 630, win_height = 950;

int mouse_x = 0;
int mouse_y = 0;

int mouseclick_x = 0;
int mouseclick_y = 0;

int x, y;

bool isRightPressed = false;
bool isLeftPressed = false;
bool isUpPressed = false;
bool isDownPressed = false;

#pragma endregion

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

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

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

void PlayerMovement()
{ 
	


	if (player.x > (win_width + 50)) player.x = -100;
	if (player.x < -100) player.x = win_width + 50;

	if (isRightPressed && !isLeftPressed)
	{
		player.x += 5;
		player.isFlip = true;
	}
	if (!isRightPressed && isLeftPressed)
	{
		player.x -= 5;
		player.isFlip = false;
	}
}



int main(int arcg, char* argv[])
{
	system("chcp 1251");
	Init();

#pragma region MENU
	SDL_Surface* mainmenu;
	SDL_Texture* menu;

	SDL_Surface* playbutton;
	SDL_Texture* plbutton;

	SDL_Surface* settingsbutton;
	SDL_Texture* sttngsbutton;

	CreateMenu(render, mainmenu, menu, playbutton, plbutton, settingsbutton, sttngsbutton);

	SDL_Rect srsrectmenu = { 0, 0, 630, 950 };
	SDL_Rect rectmenucondition = { 641,0,630,950 };

	SDL_Rect srsrectplbutton = { 120, 380, 223, 82 };
	SDL_Rect rectplbuttoncondition = { 0,0,223,82 };

	SDL_Rect srsrectsttngsbutton = {500, 750, 115, 95 };
	SDL_Rect rectsttngsbuttoncondition = {790, 850, 115, 95};

	Mix_Music* menumusic = Mix_LoadMUS("music/nonamemusic1.mp3");
#pragma endregion



#pragma region GAME

	SDL_Surface* background;
	SDL_Texture* bck;

	CreateBackground(render, background, bck);

	SDL_Rect rectbckcondition = { 0,0,630,950 };



	player.x = (win_width / 2)-50;
	player.y = 830;
	player.isJump = false;
	player.isFlip = false;

	SDL_Surface* playersurf;
	SDL_Texture* playertexture;

	CreatePlayer(render, playersurf, playertexture);

	SDL_Rect playercondition = {0,0, 100, 120};
	SDL_Rect playerposition = {player.x, player.y, 100, 120};

#pragma endregion



#pragma region MAINCYCLE

	SDL_Event ev;
	bool isRunning = true;
	bool isGame = false;

	Mix_PlayMusic(menumusic, -1);

	while (isRunning)
	{

	#pragma region EVENT
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT: //����� �� ���������� (�������� ����)
				if (!isGame) isRunning = false;
				isGame = false;
				break;


			case SDL_MOUSEBUTTONDOWN: //��������� ������� ���
				if (ev.button.button == SDL_BUTTON_LEFT)
				{
					mouseclick_x = ev.button.x;
					mouseclick_y = ev.button.y;

					if ((mouseclick_x >= 120 && mouseclick_x <= 343) && (mouseclick_y >= 380 && mouseclick_y <= 462))
					{
						isGame = true;
					}
				}
				break;

			case SDL_MOUSEMOTION: //��������� ������� �������
					
				SDL_GetMouseState(&mouse_x, &mouse_y);

				if ((mouse_x >= 120 && mouse_x <= 343) && (mouse_y >= 380 && mouse_y <= 462))
				{
					rectplbuttoncondition.y = 81;
				}
				else
				{
					rectplbuttoncondition.y = 0;
				}
				
				if ((mouse_x >= 500 && mouse_x <= 615) && (mouse_y >= 750 && mouse_y <= 845))
				{
					rectsttngsbuttoncondition.x = 641;
					rectsttngsbuttoncondition.y = 1028;
				}
				else
				{
					rectsttngsbuttoncondition.x = 790;
					rectsttngsbuttoncondition.y = 850;
				}
				break;

			case SDL_KEYDOWN: //��������� ������� ������ ����������
				switch (ev.key.keysym.scancode)
				{
				
				case SDL_SCANCODE_ESCAPE: //������� ESC
					isRunning = false;
					break;
				case SDL_SCANCODE_W: //������� ������� �����
					isUpPressed = true;
					break;
				case SDL_SCANCODE_A: //������� ������� �����
					isLeftPressed = true;
					break;
				case SDL_SCANCODE_S: //������� ������� ����
					isDownPressed = true;
					break;
				case SDL_SCANCODE_D: //������� ������� ������
					isRightPressed = true;
					break;
				}
				break;

			case SDL_KEYUP: //��������� ���������� ������ ����������
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_W: //������� ������� �����
					isUpPressed = false;
					break;
				case SDL_SCANCODE_A: //������� ������� �����
					isLeftPressed = false;
					break;
				case SDL_SCANCODE_S: //������� ������� ����
					isDownPressed = false;
					break;
				case SDL_SCANCODE_D: //������� ������� ������
					isRightPressed = false;
					break;
				}
				break;
			}
		}

	#pragma endregion


	#pragma region LOGIC
		
		if (isGame)
		{
			Mix_PauseMusic();
			
			PlayerMovement();
			playerposition = { player.x, player.y, 100, 120 };
		}

		else
		{
			Mix_ResumeMusic();
		}
	#pragma endregion


	#pragma region DRAWING
		SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
		SDL_RenderClear(render);
		
		if(isGame)
		{
			DrawBackground(render, bck, rectbckcondition);
			DrawPlayer(render, playertexture, playercondition, playerposition, player);
		}
		else
		{
			MainMenuDraw(render, menu, plbutton, sttngsbutton, rectplbuttoncondition, srsrectplbutton, rectsttngsbuttoncondition, srsrectsttngsbutton, rectmenucondition, srsrectmenu);
		}

		SDL_RenderPresent(render);

		SDL_Delay(10);
	#pragma endregion 
	
	}

#pragma endregion



#pragma region DESTRUCTION
	
	MainMenuDestroy(menu, plbutton, sttngsbutton);
	DestroyGame(bck);
	DestroyPlayer(playertexture);
	Mix_CloseAudio();
	
	DeInit(0);

#pragma endregion
	
	return 0;
}