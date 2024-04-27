#pragma once

void CreateBackground(SDL_Renderer* render, SDL_Surface*& background, SDL_Texture*& bck);

void DrawBackground(SDL_Renderer* render, SDL_Texture* bck, SDL_Rect& rectbckcondition);

void DestroyGame(SDL_Texture* bck);