#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Menu.h"
#include "Game.h"

#pragma region GLOBALINIT
#define PLAYER_JUMP_SPEED 44
#define FIXED_Y 750
#define NUM_OF_PLATFORMS 10
#define NUM_OF_FLOATING_PLATFORMS 5
#define FLOATPLATFORM_FIXED_X 100
#define NUM_OF_ENEMY 1
#define NUM_OF_BROKEN 3

SDL_Window* win = NULL;
SDL_Renderer* render = NULL;

Player player;
Enemy enemies[NUM_OF_ENEMY];

Platform platforms[NUM_OF_PLATFORMS];
Platform floatplatforms[NUM_OF_FLOATING_PLATFORMS];
Platform brokenplatforms[NUM_OF_BROKEN];


int win_width = 630, win_height = 950;

int mouse_x = 0;
int mouse_y = 0;

int mouseclick_x = 0;
int mouseclick_y = 0;

int BestRecord = 0;

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
	player.y = FIXED_Y-20;
	player.w = 90;
	player.h = 90;
	player.a = PLAYER_JUMP_SPEED;
	player.isJump = false;
	player.isFlip = false;
	player.score = 0;

	ReadRecord(BestRecord);

	SDL_Surface* playersurf;
	SDL_Texture* playertexture;

	CreatePlayer(render, playersurf, playertexture);
	
	Mix_Chunk* jumpsfx = Mix_LoadWAV("sfx/jump.wav");
	Mix_Chunk* brokeplatform = Mix_LoadWAV("sfx/lomise.mp3");
	Mix_Chunk* falling = Mix_LoadWAV("sfx/pada.mp3");
	Mix_Chunk* jumponmonster = Mix_LoadWAV("sfx/jumponmonster.mp3");
	Mix_Chunk* deathfrommonster = Mix_LoadWAV("sfx/monster-crash.mp3");
	Mix_Chunk* monsterapproaching = Mix_LoadWAV("sfx/monsterblizu.mp3");

	SDL_Rect playercondition = {0,30, player.w, player.h};
	SDL_Rect playerposition = {player.x, player.y, 100, 120};


	SDL_Surface* platformsurf;
	SDL_Texture* platformtexture;

	CreatePlatforms(render, platformsurf, platformtexture);

	GeneratePlatforms(platforms, NUM_OF_PLATFORMS);
	
	SDL_Rect platformcondition = { 315, 895, 115, 30 };
	


	SDL_Surface* floatplatformsurf;
	SDL_Texture* floatplatformtexture;

	CreateFloatPlatforms(render, floatplatformsurf, floatplatformtexture);
	GenerateFloatPlatforms(floatplatforms, NUM_OF_FLOATING_PLATFORMS, FLOATPLATFORM_FIXED_X);
	
	SDL_Rect floatplatformcondition = { 315, 925, 115, 30 };



	SDL_Surface* brokenplatformsurf;
	SDL_Texture* brokenplatformtexture;

	CreateBrokenPlatforms(render, brokenplatformsurf, brokenplatformtexture);
	GenerateBrokenPlatforms(brokenplatforms, NUM_OF_BROKEN);

	SDL_Rect brokenplatformcondition = { 325,826,120,30 }; 





	SDL_Surface* enemysurf;
	SDL_Texture* enemytexture;
	CreateEnemy(render, enemysurf,enemytexture);
	SDL_Rect enemycondition = {675,380, 150, 85};
	GenerateEnemies(enemies,NUM_OF_ENEMY, &enemycondition);

#pragma endregion



#pragma region MAINCYCLE

	SDL_Event ev;
	bool isRunning = true;
	bool isGame = false;
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
					if (!isGame && !isSettings) isRunning = false;
					isGame = false;
					isSettings = false;
					break;
				case SDL_SCANCODE_E: 
					isEPressed = true;
					break;
				case SDL_SCANCODE_A: 
					isLeftPressed = true;
					break;
				case SDL_SCANCODE_Q: 
					isQPressed = true;
					break;
				case SDL_SCANCODE_D: 
					isRightPressed = true;
					break;
				}
				break;

			case SDL_KEYUP: //Обработка отпускания клавиш клавиатуры
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_E: 
					isEPressed = false;
					break;
				case SDL_SCANCODE_A: 
					isLeftPressed = false;
					break;
				case SDL_SCANCODE_Q: 
					isQPressed = false;
					break;
				case SDL_SCANCODE_D:
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
			
			CheckCollisionPlatforms(player, platforms, floatplatforms, brokenplatforms, enemies);

			CheckCollisionFloatPlatforms(player, platforms, floatplatforms, brokenplatforms, enemies);

			CheckCollisionBrokenPlatforms(player, brokenplatforms, brokeplatform);
			
			CheckEnemyCollision(player, platforms, floatplatforms, brokenplatforms, enemies, monsterapproaching, jumponmonster);

			PlayerJump(player, win_width, isRightPressed, isLeftPressed, playercondition, jumpsfx);

			if (player.y <= 200)
				MoveMap(player, platforms, floatplatforms, brokenplatforms, enemies, 200 - player.y);
			
			FloatPlatformsMove(floatplatforms, NUM_OF_FLOATING_PLATFORMS, FLOATPLATFORM_FIXED_X);
			
			EnemiesMove(enemies, NUM_OF_ENEMY, &enemycondition);

			playerposition = { player.x, player.y, 100, 120 };
			player.movementbox = { player.x+25, player.y + 120, 50, 10 };

			CheckLose(player, platforms, floatplatforms, brokenplatforms, enemies, falling, deathfrommonster, playerposition, enemycondition, win_height, win_width, isGame, BestRecord);
			
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
			
			for (int i = 0; i < NUM_OF_PLATFORMS; i++)
			{
				DrawPlatforms(render, platformtexture, platformcondition, platforms[i].platformposition);
				if(i<NUM_OF_FLOATING_PLATFORMS) DrawPlatforms(render, floatplatformtexture, floatplatformcondition, floatplatforms[i].platformposition);
				if(i<NUM_OF_BROKEN) DrawPlatforms(render, brokenplatformtexture, brokenplatformcondition, brokenplatforms[i].platformposition);
				if (i < NUM_OF_ENEMY) DrawEnemy(render, enemytexture, enemycondition, enemies[i].position);
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
	DestroyPlatforms(platformtexture);
	DestroyPlatforms(floatplatformtexture);
	DestroyPlatforms(brokenplatformtexture);
	DestroyEnemy(enemytexture);
	DestroyPlayer(playertexture);
	Mix_CloseAudio();
	
	DeInit(0);

#pragma endregion
	
	return 0;
}