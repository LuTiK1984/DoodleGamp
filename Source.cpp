#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Menu.h"
#include "Game.h"

#pragma region GLOBALINIT
#define PLAYER_JUMP_SPEED 46
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
	player.y = FIXED_Y;
	player.w = 90;
	player.h = 90;
	player.a = PLAYER_JUMP_SPEED;
	player.isJump = false;
	player.isFlip = false;
	player.score = 0;

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
			

			if (player.isJump) 
			{ 
				playercondition = { 125, 0, 100, 120 }; 
				Mix_PlayChannel(1, jumpsfx, 0);
			}
			else playercondition = { 0, 0, 100, 120 };

			
			
			int term = 0;
			
			for (int i = 0; i < NUM_OF_PLATFORMS; i++)
			{
				if (SDL_HasIntersection(&platforms[i].platformposition, &player.movementbox))
				{
					player.isJump = true;
					player.a = PLAYER_JUMP_SPEED;
					if (platforms[i].platformposition.y < FIXED_Y)
					{
						term = FIXED_Y - platforms[i].platformposition.y; //изменеие по y относительно фиксированного значения
						player.score += term;
						
						for (int j = 0; j < NUM_OF_PLATFORMS; j++)
						{
							
							platforms[j].platformposition.y += term;
							UpdatePlatforms(platforms, NUM_OF_PLATFORMS);
							
							if (j < NUM_OF_FLOATING_PLATFORMS)
							{
								floatplatforms[j].platformposition.y += term;
								UpdatePlatforms(floatplatforms, NUM_OF_FLOATING_PLATFORMS);
							}
							if (j < NUM_OF_BROKEN)
							{
								brokenplatforms[j].platformposition.y += term;
								UpdatePlatforms(brokenplatforms, NUM_OF_BROKEN);
							}
							if (j < NUM_OF_ENEMY)
							{
								enemies[j].position.y += term;
								UpdateEnemy(enemies, NUM_OF_ENEMY);
							}
						}
					}
					player.y = platforms[i].platformposition.y-110;
					printf("\nРекорд: %i\n", player.score);
					break;
				}
				else player.isJump = false;	
			}

			for (int i = 0; i < NUM_OF_FLOATING_PLATFORMS; i++)
			{
				if (SDL_HasIntersection(&floatplatforms[i].platformposition, &player.movementbox))
				{
					player.isJump = true;
					player.a = PLAYER_JUMP_SPEED;
					if (floatplatforms[i].platformposition.y < FIXED_Y)
					{
						term = FIXED_Y - floatplatforms[i].platformposition.y; //изменеие по y относительно фиксированного значения
						player.score += term;

						for (int j = 0; j < NUM_OF_PLATFORMS; j++)
						{

							platforms[j].platformposition.y += term;
							UpdatePlatforms(platforms, NUM_OF_PLATFORMS);
							
							if (j < NUM_OF_FLOATING_PLATFORMS)
							{
								floatplatforms[j].platformposition.y += term;
								UpdatePlatforms(floatplatforms, NUM_OF_FLOATING_PLATFORMS);
							}

							if (j < NUM_OF_BROKEN)
							{
								brokenplatforms[j].platformposition.y += term;
								UpdatePlatforms(brokenplatforms, NUM_OF_BROKEN);
							}

							if (j < NUM_OF_ENEMY)
							{
								enemies[j].position.y += term;
								UpdateEnemy(enemies, NUM_OF_ENEMY);
							}
						}
					}
					player.y = floatplatforms[i].platformposition.y - 110;
					printf("\nРекорд: %i\n", player.score);
					break;
				}
			}
			
			for (int i = 0; i < NUM_OF_BROKEN; i++)
			{
				SDL_Rect termblock = player.movementbox;
				termblock.y -= player.a;
				if (SDL_HasIntersection(&brokenplatforms[i].platformposition, &termblock))
				{
					Mix_PlayChannel(4, brokeplatform, 0);
					RegeneratePlatform(brokenplatforms, i);
					break;
				}
			}

			for (int i = 0; i < NUM_OF_ENEMY; i++)
			{
				SDL_Rect termblock = player.movementbox;
				termblock.y -= player.a;

				if(enemies[i].position.y < -300) Mix_PlayChannel(2, monsterapproaching, 0);

				if (player.a <= 0 && SDL_HasIntersection(&enemies[i].position, &termblock))
				{
					Mix_PlayChannel(3, jumponmonster, 0);
					player.a = PLAYER_JUMP_SPEED;
					if (enemies[i].position.y < FIXED_Y)
					{
						term = FIXED_Y - enemies[i].position.y; //изменеие по y относительно фиксированного значения
						player.score += term;

						for (int j = 0; j < NUM_OF_PLATFORMS; j++)
						{

							platforms[j].platformposition.y += term;
							UpdatePlatforms(platforms, NUM_OF_PLATFORMS);

							if (j < NUM_OF_FLOATING_PLATFORMS)
							{
								floatplatforms[j].platformposition.y += term;
								UpdatePlatforms(floatplatforms, NUM_OF_FLOATING_PLATFORMS);
							}

							if (j < NUM_OF_BROKEN)
							{
								brokenplatforms[j].platformposition.y += term;
								UpdatePlatforms(brokenplatforms, NUM_OF_BROKEN);
							}

							if (j < NUM_OF_ENEMY)
							{
								enemies[j].position.y += term;
								UpdateEnemy(enemies, NUM_OF_ENEMY);
							}
						}
					}
					player.y = enemies[i].position.y - 110;
					RegenerateEnemy(enemies, i);
					printf("\nРекорд: %i\n", player.score);
					break;
				}
			}

			player.a -= 2;	

			player.y -= player.a;
				
			if (player.x > (win_width + 50)) player.x = -100;
			if (player.x < -100) player.x = win_width + 50;

			if (isRightPressed && !isLeftPressed)
			{
				player.x += 12;
				player.isFlip = true;
			}
			if (!isRightPressed && isLeftPressed)
			{
				player.x -= 12;
				player.isFlip = false;
			}
			

			FloatPlatformsMove(floatplatforms, NUM_OF_FLOATING_PLATFORMS, FLOATPLATFORM_FIXED_X);
			EnemiesMove(enemies, NUM_OF_ENEMY, &enemycondition);

			playerposition = { player.x, player.y, 100, 120 };
			player.movementbox = { player.x+25, player.y + 120, 50, 10 };

			for (int i = 0; i < NUM_OF_ENEMY; i++)
			{
				if (player.movementbox.y > win_height + player.h+100 || SDL_HasIntersection(&enemies[i].position, &player.movementbox))
				{
					if (player.movementbox.y > win_height + player.h+100)
					{
						Mix_PlayChannel(3, falling, 0);
					}

					if (SDL_HasIntersection(&enemies[i].position, &player.movementbox))
					{
						Mix_PlayChannel(3, deathfrommonster, 0);
					}
					Mix_HaltChannel(2);
					system("cls");
					player.x = (win_width / 2) - 50;
					player.y = FIXED_Y;
					playerposition = { player.x, player.y, 100, 120 };
					GeneratePlatforms(platforms, NUM_OF_PLATFORMS);
					GenerateFloatPlatforms(floatplatforms, NUM_OF_FLOATING_PLATFORMS, FLOATPLATFORM_FIXED_X);
					GenerateBrokenPlatforms(brokenplatforms, NUM_OF_BROKEN);
					GenerateEnemies(enemies, NUM_OF_ENEMY, &enemycondition);
					player.movementbox = { player.x + 25, player.y + 120, 50, 10 };
					printf("\nРекорд: %i\n", player.score);
					player.score = 0;
					SDL_Delay(1400);
					isGame = false;
				}

			}
			
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