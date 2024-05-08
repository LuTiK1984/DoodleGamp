#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

struct Player
{
	int x, y;
	int a;
	bool isJump;
	bool isFlip;
	int score;
};

struct Platform
{
	int x, y, w, h;
	int type;
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

void GeneratePlatforms(Platform platforms[])
{

	for (int i = 0; i < 10; i++)
	{
		platforms[i].x = random(5, 510);
		platforms[i].y = random(-100, 750);
		platforms[i].w = 115;
		platforms[i].h = 30;
		platforms[i].type = 0;
	}

}

void DrawPlatforms(SDL_Renderer* render, SDL_Texture* platformtexture, SDL_Rect platformcondition, SDL_Rect platformposition)
{
	SDL_RenderCopy(render, platformtexture, &platformcondition, &platformposition);
}