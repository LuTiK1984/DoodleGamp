#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#define NUM_OF_PLATFORMS 10
#define NUM_OF_FLOATING_PLATFORMS 5
#define NUM_OF_ENEMY 1
#define NUM_OF_BROKEN 3
#define FIXED_Y 750
#define PLAYER_JUMP_SPEED 46
#define FLOATPLATFORM_FIXED_X 100

struct Player
{
	int x, y, w, h;
	SDL_Rect movementbox = { x,y,50,10 };
	int a;
	bool isJump;
	bool isFlip;
	int score;
};

struct Platform
{
	SDL_Rect platformposition = { 0, 0, 110, 30 };
	int type;
	int movetick;
	int v;
};

struct Enemy
{
	SDL_Rect position;
	int animationtick;
	int movetick;
	int v;
};



void CreatePlayer(SDL_Renderer* render, SDL_Surface* &playersurf, SDL_Texture* &playertexture)
{
	playersurf = IMG_Load("sprites/playerSheet.png");
	playertexture = SDL_CreateTextureFromSurface(render, playersurf);

	if (playersurf == NULL)
	{
		printf("Couldn`t load character! Error: %s", SDL_GetError());
		system("pause");
		exit(1);
	}

	SDL_FreeSurface(playersurf);
}

void DrawPlayer(SDL_Renderer* render, SDL_Texture*& playertexture, SDL_Rect& playercondition, SDL_Rect& playerposition, Player player)
{
	if(!player.isFlip) SDL_RenderCopyEx(render, playertexture, &playercondition, &playerposition, NULL, NULL, SDL_FLIP_NONE);
	else SDL_RenderCopyEx(render, playertexture, &playercondition, &playerposition, NULL, NULL, SDL_FLIP_HORIZONTAL);
}

void DestroyPlayer(SDL_Texture*& playertexture)
{
	SDL_DestroyTexture(playertexture);
}

void CreateBackground(SDL_Renderer* render, SDL_Surface* &background, SDL_Texture* &bck)
{
	background = IMG_Load("sprites/atlas2.png");
	bck = SDL_CreateTextureFromSurface(render, background);

	if (background == NULL)
	{
		printf("Couldn`t load background! Error: %s", SDL_GetError());
		system("pause");
		exit(1);
	}

	SDL_FreeSurface(background);
}

void DrawBackground(SDL_Renderer* render, SDL_Texture* bck, SDL_Rect &rectbckcondition)
{
	SDL_RenderCopy(render, bck, &rectbckcondition, NULL);
}

void DestroyBackground(SDL_Texture* bck)
{
	SDL_DestroyTexture(bck);
}


int random(int min, int max) {
	return min + rand() % (max - min + 1);
}

void CreatePlatforms(SDL_Renderer* render, SDL_Surface* &platformsurf, SDL_Texture* &platformtexture)
{
	platformsurf = IMG_Load("sprites/atlas.png");
	platformtexture = SDL_CreateTextureFromSurface(render, platformsurf);

	if (platformsurf == NULL)
	{
		printf("Couldn`t load platforms! Error: %s", SDL_GetError());
		system("pause");
		exit(1);
	}

	SDL_FreeSurface(platformsurf);
}

void GeneratePlatforms(Platform platforms[], int num)
{
	SDL_Rect term[NUM_OF_PLATFORMS];
	for (int i = 0; i < num; i++)
	{
		if (i == 0)
		{
			platforms[i].platformposition.x = (630 / 2) - 50;
			platforms[i].platformposition.y = 850;
			platforms[i].platformposition.w = 110;
			platforms[i].platformposition.h = 30;
			
			term[i].x = platforms[i].platformposition.x;
			term[i].y = platforms[i].platformposition.y;
			term[i].w = platforms[i].platformposition.w;
			term[i].h = platforms[i].platformposition.h;

			platforms[i].type = 0;
		}
		
		else
		{

			platforms[i].platformposition.x = random(5, 510);
			platforms[i].platformposition.y = random(-500, 750);
			platforms[i].platformposition.w = 110;
			platforms[i].platformposition.h = 30;

			term[i].x = platforms[i].platformposition.x;
			term[i].y = platforms[i].platformposition.y;
			term[i].w = platforms[i].platformposition.w;
			term[i].h = platforms[i].platformposition.h;

			platforms[i].type = 0;
		}
	}

	for (int i = 0; i < num; i++)
	{
		for (int j = i + 1; j < num; j++)
		{
			if (SDL_HasIntersection(&platforms[i].platformposition, &term[j]))
			{
				platforms[i].platformposition.x += random(-10, 50) * random(-platforms[i].platformposition.w* platforms[i].platformposition.w, platforms[i].platformposition.w* platforms[i].platformposition.w);
				platforms[i].platformposition.y -= random(-10, 100) * random(-platforms[i].platformposition.h* platforms[i].platformposition.h, platforms[i].platformposition.h* platforms[i].platformposition.h);
				break;
			}
		}
	}
}

void DrawPlatforms(SDL_Renderer* render, SDL_Texture* platformtexture, SDL_Rect platformcondition, SDL_Rect platformposition)
{
	SDL_RenderCopy(render, platformtexture, &platformcondition, &platformposition);
}

void DestroyPlatforms(SDL_Texture* platform)
{
	SDL_DestroyTexture(platform);
}

void RegeneratePlatform(Platform platforms[],int num)
{
	platforms[num].platformposition.x = random(5, 510);
	platforms[num].platformposition.y = random(-1000, 10);
	platforms[num].platformposition.w = 110;
	platforms[num].platformposition.h = 30;

	SDL_Rect term =
	{
		platforms[num].platformposition.x,
		platforms[num].platformposition.y,
		platforms[num].platformposition.w,
		platforms[num].platformposition.h
	};

	for (int i = 0; i < num; i++)
	{
		if (SDL_HasIntersection(&platforms[i].platformposition, &term))
		{
			platforms[i].platformposition.x += random(-10, 50)*random(-platforms[i].platformposition.w * platforms[i].platformposition.w, platforms[i].platformposition.w * platforms[i].platformposition.w);
			platforms[i].platformposition.y -= random(-10, 100) * random(-platforms[i].platformposition.h * platforms[i].platformposition.h, platforms[i].platformposition.h * platforms[i].platformposition.h);
			break;
		}
	}
}

void UpdatePlatforms(Platform platforms[], int num)
{
	for (int i = 0; i < num; i++)
	{
		if (platforms[i].platformposition.y > 950 + 30) RegeneratePlatform(platforms, i);
	}
}

void CreateFloatPlatforms(SDL_Renderer* render, SDL_Surface*& platformsurf, SDL_Texture*& platformtexture)
{
	platformsurf = IMG_Load("sprites/atlas.png");
	platformtexture = SDL_CreateTextureFromSurface(render, platformsurf);

	if (platformsurf == NULL)
	{
		printf("Couldn`t load platforms! Error: %s", SDL_GetError());
		system("pause");
		exit(1);
	}

	SDL_FreeSurface(platformsurf);
}

void GenerateFloatPlatforms(Platform platforms[], int num, int x)
{
	SDL_Rect term[NUM_OF_FLOATING_PLATFORMS];
	for (int i = 0; i < num; i++)
	{
			platforms[i].platformposition.x = random(5, 510);
			platforms[i].platformposition.y = random(-500, 750);
			platforms[i].platformposition.w = 110;
			platforms[i].platformposition.h = 30;

			term[i].x = platforms[i].platformposition.x;
			term[i].y = platforms[i].platformposition.y;
			term[i].w = platforms[i].platformposition.w;
			term[i].h = platforms[i].platformposition.h;

			platforms[i].type = 1;
			platforms[i].movetick = random(0,x);
			platforms[i].v = random(2, 4);
	}

	for (int i = 0; i < num; i++)
	{
		for (int j = i + 1; j < num; j++)
		{
			if (SDL_HasIntersection(&platforms[i].platformposition, &term[j]))
			{
				platforms[i].platformposition.x += random(-10, 50) *random(-platforms[i].platformposition.w * platforms[i].platformposition.w, platforms[i].platformposition.w * platforms[i].platformposition.w);
				platforms[i].platformposition.y -= random(-10, 100) *random(-platforms[i].platformposition.h * platforms[i].platformposition.h, platforms[i].platformposition.h * platforms[i].platformposition.h);
				break;
			}
		}
	}
}

void FloatPlatformsMove(Platform platforms[], int num, int x)
{
	for (int i = 0; i < num; i++)
	{
		if (platforms[i].movetick >= x)
		{
			platforms[i].movetick = 0;
			platforms[i].v *= -1;
		}
		
			platforms[i].platformposition.x += platforms[i].v;
			platforms[i].movetick ++;
		
	}
}

void CreateEnemy(SDL_Renderer* render, SDL_Surface*& playersurf, SDL_Texture*& playertexture)
{
	playersurf = IMG_Load("sprites/atlas.png");
	playertexture = SDL_CreateTextureFromSurface(render, playersurf);

	if (playersurf == NULL)
	{
		printf("Couldn`t load enemy! Error: %s", SDL_GetError());
		system("pause");
		exit(1);
	}

	SDL_FreeSurface(playersurf);
}

void DrawEnemy(SDL_Renderer* render, SDL_Texture* enemytexture, SDL_Rect enemycondition, SDL_Rect enemyposition)
{
	SDL_RenderCopy(render, enemytexture, &enemycondition, &enemyposition);
}

void DestroyEnemy(SDL_Texture* enemy)
{
	SDL_DestroyTexture(enemy);
}

void GenerateEnemies(Enemy enemies[], int num, SDL_Rect *enemycondition)
{
	SDL_Rect term[NUM_OF_ENEMY];
	for (int i = 0; i < num; i++)
	{
		enemies[i].position.x = random(5, 510);
		enemies[i].position.y = random(-2000, -600);
		enemies[i].position.w = 150;
		enemies[i].position.h = 85;

		term[i].x = enemies[i].position.x;
		term[i].y = enemies[i].position.y;
		term[i].w = enemies[i].position.w;
		term[i].h = enemies[i].position.h;

		enemies[i].animationtick = random(1, 2);
		enemies[i].movetick = random(0, 2);
		enemies[i].v = random(1, 4);

		if (enemies[i].animationtick == 2)
		{
			*enemycondition = { 675,465,150, 85 };
			enemies[i].animationtick--;
		}
		if (enemies[i].animationtick == 1)
		{
			*enemycondition = { 675,380,150, 85 };
			enemies[i].animationtick++;
		}
	}

	for (int i = 0; i < num; i++)
	{
		for (int j = i + 1; j < num; j++)
		{
			if (SDL_HasIntersection(&enemies[i].position, &term[j]))
			{
				enemies[i].position.x += random(-10, 100) * random(-enemies[i].position.w * enemies[i].position.w, enemies[i].position.w * enemies[i].position.w);
				enemies[i].position.y -= random(-10, 100) * random(-enemies[i].position.h * enemies[i].position.h, enemies[i].position.h * enemies[i].position.h);
				break;
			}
		}
	}
}

void RegenerateEnemy(Enemy enemies[], int num)
{
	enemies[num].position.x = random(5, 510);
	enemies[num].position.y = random(-10000, 10);
	enemies[num].position.w = 150;
	enemies[num].position.h = 85;

	SDL_Rect term =
	{
		enemies[num].position.x,
		enemies[num].position.y,
		enemies[num].position.w,
		enemies[num].position.h
	};

	for (int i = 0; i < num; i++)
	{
		if (SDL_HasIntersection(&enemies[num].position, &term))
		{
			enemies[i].position.x += random(-10, 100) * random(-enemies[i].position.w * enemies[i].position.w, enemies[i].position.w * enemies[i].position.w);
			enemies[i].position.y -= random(-10, 100) * random(-enemies[i].position.h * enemies[i].position.h, enemies[i].position.h * enemies[i].position.h);
			break;
		}
	}
}

void UpdateEnemy(Enemy enemies[], int num)
{
	for (int i = 0; i < num; i++)
	{
		if (enemies[i].position.y > 950 + 30) RegenerateEnemy(enemies, i);
	}
}

void EnemiesMove(Enemy enemies[], int num, SDL_Rect *enemycondition)
{
	int x = 6;
	int y = 3;
	for (int i = 0; i < num; i++)
	{
		if (enemies[i].movetick >= x)
		{
			enemies[i].movetick = 0;
			enemies[i].v *= -1;
		}
		if (enemies[i].animationtick > y)
		{
			*enemycondition = { 675,467,150, 85 };
			enemies[i].animationtick = -y;
		}
		if (enemies[i].animationtick == 0)
		{
			*enemycondition = { 675,380,150, 85 };
		}

		enemies[i].position.x += enemies[i].v;
		enemies[i].position.y -= enemies[i].v;
		enemies[i].movetick++;
		enemies[i].animationtick++;

	}
}


void CreateBrokenPlatforms(SDL_Renderer* render, SDL_Surface*& platformsurf, SDL_Texture*& platformtexture)
{
	platformsurf = IMG_Load("sprites/atlas.png");
	platformtexture = SDL_CreateTextureFromSurface(render, platformsurf);

	if (platformsurf == NULL)
	{
		printf("Couldn`t load platforms! Error: %s", SDL_GetError());
		system("pause");
		exit(1);
	}

	SDL_FreeSurface(platformsurf);
}

void GenerateBrokenPlatforms(Platform platforms[], int num)
{
	SDL_Rect term[NUM_OF_BROKEN];
	for (int i = 0; i < num; i++)
	{
			platforms[i].platformposition.x = random(5, 510);
			platforms[i].platformposition.y = random(-500, 750);
			platforms[i].platformposition.w = 110;
			platforms[i].platformposition.h = 30;

			term[i].x = platforms[i].platformposition.x;
			term[i].y = platforms[i].platformposition.y;
			term[i].w = platforms[i].platformposition.w;
			term[i].h = platforms[i].platformposition.h;

			platforms[i].type = 2;
	}

	for (int i = 0; i < num; i++)
	{
		for (int j = i + 1; j < num; j++)
		{
			if (SDL_HasIntersection(&platforms[i].platformposition, &term[j]))
			{
				platforms[i].platformposition.x += random(-10, 50) * random(-platforms[i].platformposition.w * platforms[i].platformposition.w, platforms[i].platformposition.w * platforms[i].platformposition.w);
				platforms[i].platformposition.y -= random(-10, 100) * random(-platforms[i].platformposition.h * platforms[i].platformposition.h, platforms[i].platformposition.h * platforms[i].platformposition.h);
				break;
			}
		}
	}
}

void PlayerJump(Player &player, int win_width, bool isRightPressed, bool isLeftPressed, SDL_Rect& playercondition, Mix_Chunk* jumpsfx)
{
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

	if (player.isJump)
	{
		playercondition = { 125, 0, 100, 120 };
		Mix_PlayChannel(1, jumpsfx, 0);
	}
	else playercondition = { 0, 0, 100, 120 };
}

void MoveMap(Player &player, Platform platforms[], Platform floatplatforms[], Platform brokenplatforms[], Enemy enemies[], int term)
{
	player.y += term;
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

void CheckCollisionPlatforms(Player &player, Platform platforms[], Platform floatplatforms[], Platform brokenplatforms[], Enemy enemies[])
{
	int term = 0;

	for (int i = 0; i < NUM_OF_PLATFORMS; i++)
	{
		if (SDL_HasIntersection(&platforms[i].platformposition, &player.movementbox) && player.a < 0)
		{
			player.isJump = true;
			player.a = PLAYER_JUMP_SPEED;
			if (platforms[i].platformposition.y < FIXED_Y)
			{
				term = FIXED_Y - platforms[i].platformposition.y; //изменеие по y относительно фиксированного значения
				player.score += term;

				//MoveMap(platforms, floatplatforms, brokenplatforms, enemies, term);
			}
			player.y = platforms[i].platformposition.y - 110;
			printf("\nРекорд: %i\n", player.score);
			break;
		}
		else player.isJump = false;
	}
}

void CheckCollisionFloatPlatforms(Player &player, Platform platforms[], Platform floatplatforms[], Platform brokenplatforms[], Enemy enemies[])
{
	int term = 0;
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

				//MoveMap(platforms, floatplatforms, brokenplatforms, enemies, term);
				
			}
			player.y = floatplatforms[i].platformposition.y - 110;
			printf("\nРекорд: %i\n", player.score);
			break;
		}
	}
}

void CheckCollisionBrokenPlatforms(Player &player, Platform brokenplatforms[], Mix_Chunk* brokeplatform)
{
	int term = 0;
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
}

void CheckEnemyCollision(Player &player, Platform platforms[], Platform floatplatforms[], Platform brokenplatforms[], Enemy enemies[], Mix_Chunk* monsterapproaching, Mix_Chunk* jumponmonster)
{
	int term = 0;

	for (int i = 0; i < NUM_OF_ENEMY; i++)
	{
		SDL_Rect termblock = player.movementbox;
		termblock.y -= player.a;

		if (enemies[i].position.y < -300) Mix_PlayChannel(2, monsterapproaching, 1);

		if (player.a <= 0 && SDL_HasIntersection(&enemies[i].position, &termblock))
		{
			Mix_PlayChannel(3, jumponmonster, 0);
			player.a = PLAYER_JUMP_SPEED;
			if (enemies[i].position.y < FIXED_Y)
			{
				term = FIXED_Y - enemies[i].position.y; //изменеие по y относительно фиксированного значения
				player.score += term;

				//MoveMap(platforms, floatplatforms, brokenplatforms, enemies, term);
			}
			player.y = enemies[i].position.y - 110;
			RegenerateEnemy(enemies, i);
			printf("\nРекорд: %i\n", player.score);
			break;
		}
	}
}

void ReadRecord(int& record)
{
	FILE* f;
	if (fopen_s(&f, "record.txt", "rt") != 0)
	{
		perror("Не удалось открыть файл");
		return;
	}

	fscanf_s(f, "%i", &record);

	fclose(f);
}

void SaveRecord(int& record)
{
	FILE* f;
	if (fopen_s(&f, "file.txt", "wt") != 0)
	{
		perror("Не удалось открыть файл");
		return;
	}

	fprintf(f, "%i\n", record);

	fclose(f);
}

void CheckLose(Player& player, Platform platforms[], Platform floatplatforms[], Platform brokenplatforms[], Enemy enemies[], Mix_Chunk* falling, Mix_Chunk* deathfrommonster, SDL_Rect &playerposition, SDL_Rect& enemycondition, int win_height, int win_width, bool &isGame, int& bestrecord)
{
	ReadRecord(bestrecord);
	int actualrecord = 0;
	for (int i = 0; i < NUM_OF_ENEMY; i++)
	{
		if (player.movementbox.y > win_height + player.h + 100 || SDL_HasIntersection(&enemies[i].position, &player.movementbox))
		{
			if (player.movementbox.y > win_height + player.h + 100)
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
			actualrecord = player.score;
			printf("\nВы набрали очков: %i\n", actualrecord);
			if (player.score > bestrecord)
			{
				bestrecord = player.score;
				SaveRecord(bestrecord);
			}
			printf("Лучший результат: %i\n", bestrecord);
			player.score = 0;
			SDL_Delay(1500);
			isGame = false;
		}

	}
}
