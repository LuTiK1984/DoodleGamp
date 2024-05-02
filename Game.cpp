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

void DestroyGame(SDL_Texture* bck)
{
	SDL_DestroyTexture(bck);
}
