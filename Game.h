#pragma once

struct Player
{
	int x, y, w, h;
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

void CreatePlayer(SDL_Renderer* render, SDL_Surface*& playersurf, SDL_Texture*& playertexture);

void DrawPlayer(SDL_Renderer* render, SDL_Texture*& playertexture, SDL_Rect& playercondition, SDL_Rect& playerposition, Player player);

void DestroyPlayer(SDL_Texture*& playertexture);

void CreateBackground(SDL_Renderer* render, SDL_Surface*& background, SDL_Texture*& bck);

void DrawBackground(SDL_Renderer* render, SDL_Texture* bck, SDL_Rect& rectbckcondition);

void DestroyBackground(SDL_Texture* bck);

int random(int min, int max);

void CreatePlatforms(SDL_Renderer* render, SDL_Surface*& platformsurf, SDL_Texture*& platformtexture);

void GeneratePlatforms(Platform platforms[]);

void DrawPlatforms(SDL_Renderer* render, SDL_Texture* platformtexture, SDL_Rect platformcondition, SDL_Rect platformposition);