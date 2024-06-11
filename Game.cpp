#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#define NUM_OF_PLATFORMS 10
#define NUM_OF_FLOATING_PLATFORMS 5
#define NUM_OF_ENEMY 1



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
			platforms[i].platformposition.y = random(-50, 750);
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
				platforms[i].platformposition.x += random(-10, 100) * random(-platforms[i].platformposition.w* platforms[i].platformposition.w, platforms[i].platformposition.w* platforms[i].platformposition.w);
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
	platforms[num].platformposition.y = random(-500, 30);
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
			platforms[i].platformposition.x += random(-10, 100)*random(-platforms[i].platformposition.w * platforms[i].platformposition.w, platforms[i].platformposition.w * platforms[i].platformposition.w);
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
			platforms[i].platformposition.y = random(-200, 750);
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
				platforms[i].platformposition.x += random(-10, 100) *random(-platforms[i].platformposition.w * platforms[i].platformposition.w, platforms[i].platformposition.w * platforms[i].platformposition.w);
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
	SDL_Rect term[NUM_OF_FLOATING_PLATFORMS];
	for (int i = 0; i < num; i++)
	{
		enemies[i].position.x = random(5, 510);
		enemies[i].position.y = random(-1000, -100);
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
	enemies[num].position.y = random(-1000, 30);
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