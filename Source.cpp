#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <Menu.h>
#include <Game.h>

#pragma region GLOBALINIT
#define PLAYER_JUMP_SPEED 40
#define FIXED_Y 750

SDL_Window* win = NULL;
SDL_Renderer* render = NULL;

Player player;
Platform platforms[10];


int win_width = 630, win_height = 950;

int mouse_x = 0;
int mouse_y = 0;

int winspeed = 0;

int mouseclick_x = 0;
int mouseclick_y = 0;

int x, y;

int Volume = 128;

bool isRightPressed = false;
bool isLeftPressed = false;
bool isEPressed = false;
bool isQPressed = false;

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

void PlayerMovement(SDL_Rect& playerposition, SDL_Rect islam2)
{
	if (((player.x + playerposition.w) >= islam2.x) && ((islam2.x + islam2.w) >= player.x))
	{
		if (((player.y + playerposition.h) < islam2.y) && ((player.y + playerposition.h - player.a) > islam2.y)) player.a = PLAYER_JUMP_SPEED;
		else player.a -= 2;
	}

	else
	{
		if ((player.y + playerposition.h - player.a) < FIXED_Y) player.a -= 2;
		else player.a = PLAYER_JUMP_SPEED;
	}
	
	player.y -= player.a;
	
	


	if (player.x > (win_width + 50)) player.x = -100;
	if (player.x < -100) player.x = win_width + 50;

	if (isRightPressed && !isLeftPressed)
	{
		player.x += 10;
		player.isFlip = true;
	}
	if (!isRightPressed && isLeftPressed)
	{
		player.x -= 10;
		player.isFlip = false;
	}
}



int main(int arcg, char* argv[])
{
	srand(time(NULL));
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

	SDL_Surface* volumesurf;
	SDL_Texture* volumetexture;

	CreateSettings(render, volumesurf, volumetexture);

	SDL_Rect volumerect = { (win_width/2)-165,0,330,109 };
	SDL_Rect EmptyVolumeBar = { (win_width / 2) - 136 , 114, 258, 52};
	SDL_Rect FilledVolumeBar = { (win_width / 2) - 135 , 115, 256, 50 };

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
	player.y = FIXED_Y;
	player.a = PLAYER_JUMP_SPEED;
	player.isJump = false;
	player.isFlip = false;
	player.score = 0;

	SDL_Surface* playersurf;
	SDL_Texture* playertexture;

	CreatePlayer(render, playersurf, playertexture);
	

	SDL_Rect playercondition = {0,0, 100, 120};
	SDL_Rect playerposition = {player.x, player.y, 100, 120};

	SDL_Rect islam = {200,400, 50,10};
	SDL_Rect islam2 = { 440,600, 50,10 };

	Mix_Chunk* jumpsfx = Mix_LoadWAV("sfx/jump.wav");

	SDL_Surface* platformsurf;
	SDL_Texture* platformtexture;

	CreatePlatforms(render, platformsurf, platformtexture);


	GeneratePlatforms(platforms);

	SDL_Rect platformcondition = { 315, 895, 115, 30 };
	SDL_Rect platformposition = { 0, 0, 115, 30 };

#pragma endregion



#pragma region MAINCYCLE

	SDL_Event ev;
	bool isRunning = true;
	bool isGame = false;
	bool Jumping = false;
	bool isSettings = false;

	Mix_PlayMusic(menumusic, -1);

	while (isRunning)
	{

	#pragma region EVENT
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT: //Выход из приложения (закрытие окна)
				if (!isGame && !isSettings) isRunning = false;
				isGame = false;
				isSettings = false;
				break;


			case SDL_MOUSEBUTTONDOWN: //Обработка зажатия ЛКМ
				if (ev.button.button == SDL_BUTTON_LEFT)
				{
					mouseclick_x = ev.button.x;
					mouseclick_y = ev.button.y;

					if ((mouseclick_x >= 120 && mouseclick_x <= 343) && (mouseclick_y >= 380 && mouseclick_y <= 462))
					{
						isGame = true;
					}

					if ((mouseclick_x >= 500 && mouseclick_x <= 615) && (mouseclick_y >= 750 && mouseclick_y <= 845))
					{
						isSettings = true;
					}
				}
				break;

			case SDL_MOUSEMOTION: //Обработка позиции курсора
					
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

			case SDL_KEYDOWN: //Обработка зажатия клавиш клавиатуры
				switch (ev.key.keysym.scancode)
				{
				
				case SDL_SCANCODE_ESCAPE: //Клавиша ESC
					isRunning = false;
					break;
				case SDL_SCANCODE_E: //Клавиша стрелка вверх
					isEPressed = true;
					break;
				case SDL_SCANCODE_A: //Клавиша стрелка влево
					isLeftPressed = true;
					break;
				case SDL_SCANCODE_Q: //Клавиша стрелка вниз
					isQPressed = true;
					break;
				case SDL_SCANCODE_D: //Клавиша стрелка вправо
					isRightPressed = true;
					break;
				}
				break;

			case SDL_KEYUP: //Обработка отпускания клавиш клавиатуры
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_E: //Клавиша стрелка вверх
					isEPressed = false;
					break;
				case SDL_SCANCODE_A: //Клавиша стрелка влево
					isLeftPressed = false;
					break;
				case SDL_SCANCODE_Q: //Клавиша стрелка вниз
					isQPressed = false;
					break;
				case SDL_SCANCODE_D: //Клавиша стрелка вправо
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
			
			if ((player.a >= PLAYER_JUMP_SPEED - 4) || (player.a <= (PLAYER_JUMP_SPEED - 4) * -1))
			{
				player.isJump = true;
			}
			else player.isJump = false;

			if (player.isJump) 
			{ 
				playercondition = { 125, 0, 100, 120 }; 
				if(player.a == PLAYER_JUMP_SPEED) Mix_PlayChannel(1, jumpsfx, 0);
			}
			else playercondition = { 0, 0, 100, 120 };
			
			playerposition = { player.x, player.y, 100, 120 };
			

			PlayerMovement(playerposition, islam);
			
			
		}

		else
		{
			Mix_ResumeMusic();

			if (isSettings)
			{
				if (isEPressed && !isQPressed) FilledVolumeBar.w += 4;
				if (!isEPressed && isQPressed) FilledVolumeBar.w -= 4;
				if (FilledVolumeBar.w > 256) FilledVolumeBar.w = 256;
				if (FilledVolumeBar.w < 0) FilledVolumeBar.w = 0;
				VolumeSettings(isEPressed,isQPressed, Volume);
			}
		}
	#pragma endregion


	#pragma region DRAWING
		SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
		SDL_RenderClear(render);
		
		if(isGame)
		{
			DrawBackground(render, bck, rectbckcondition);
			SDL_SetRenderDrawColor(render, 170, 0, 0, 255);
			SDL_RenderFillRect(render, &islam);
			SDL_RenderFillRect(render, &islam2);
			for (int i = 0; i < 10; i++)
			{
				platformposition.x = platforms[i].x;
				platformposition.y = platforms[i].y;
				DrawPlatforms(render, platformtexture, platformcondition, platformposition);

			}
			DrawPlayer(render, playertexture, playercondition, playerposition, player);

		}
		else
		{
			if (!isSettings)
			{
				MainMenuDraw(render, menu, plbutton, sttngsbutton, rectplbuttoncondition, srsrectplbutton, rectsttngsbuttoncondition, srsrectsttngsbutton, rectmenucondition, srsrectmenu);
			}
			else
			{
				DrawBackground(render, bck, rectbckcondition);
				SDL_RenderCopy(render, volumetexture,NULL, &volumerect);
				SDL_SetRenderDrawColor(render, 178, 0, 0, 255);
				SDL_RenderFillRect(render, &FilledVolumeBar);
				SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
				SDL_RenderDrawRect(render, &EmptyVolumeBar);
			}
		}

		SDL_RenderPresent(render);

		SDL_Delay(25);
	#pragma endregion 
	
	}

#pragma endregion



#pragma region DESTRUCTION
	
	MainMenuDestroy(menu, plbutton, sttngsbutton);
	DestroyBackground(bck);
	DestroyPlayer(playertexture);
	Mix_CloseAudio();
	
	DeInit(0);

#pragma endregion
	
	return 0;
}