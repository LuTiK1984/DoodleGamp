#pragma once

struct Player
{
	int x, y, w, h;
	SDL_Rect movementbox = {x,y,55,10};
	int a;
	bool isJump;
	bool isFlip;
	int score;
};

struct Platform
{
	SDL_Rect platformposition = { 0, 0, 110, 30 };
	int type;
};

void CreatePlayer(SDL_Renderer* render, SDL_Surface*& playersurf, SDL_Texture*& playertexture);

void DrawPlayer(SDL_Renderer* render, SDL_Texture*& playertexture, SDL_Rect& playercondition, SDL_Rect& playerposition, Player player);

void DestroyPlayer(SDL_Texture*& playertexture);

void CreateBackground(SDL_Renderer* render, SDL_Surface*& background, SDL_Texture*& bck);

void DrawBackground(SDL_Renderer* render, SDL_Texture* bck, SDL_Rect& rectbckcondition);

void DestroyBackground(SDL_Texture* bck);

int random(int min, int max);

void CreatePlatforms(SDL_Renderer* render, SDL_Surface*& platformsurf, SDL_Texture*& platformtexture);

void GeneratePlatforms(Platform platforms[], int num);

void DrawPlatforms(SDL_Renderer* render, SDL_Texture* platformtexture, SDL_Rect platformcondition, SDL_Rect platformposition);

void DestroyPlatforms(SDL_Texture* platform);
