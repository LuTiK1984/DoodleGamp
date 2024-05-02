#pragma once

struct Player
{
	int x, y;
	int a;
	bool isJump;
	bool isFlip;
	int score;
};

void CreatePlayer(SDL_Renderer* render, SDL_Surface*& playersurf, SDL_Texture*& playertexture);

void DrawPlayer(SDL_Renderer* render, SDL_Texture*& playertexture, SDL_Rect& playercondition, SDL_Rect& playerposition, Player player);

void DestroyPlayer(SDL_Texture*& playertexture);

void CreateBackground(SDL_Renderer* render, SDL_Surface*& background, SDL_Texture*& bck);

void DrawBackground(SDL_Renderer* render, SDL_Texture* bck, SDL_Rect& rectbckcondition);

void DestroyGame(SDL_Texture* bck);