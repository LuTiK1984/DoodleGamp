#pragma once

void CreateMenu(SDL_Renderer* render, SDL_Surface*& mainmenu, SDL_Texture*& menu, SDL_Surface*& playbutton, SDL_Texture*& plbutton);

void MainMenuDraw(SDL_Renderer* render, SDL_Texture* menu, SDL_Texture* plbutton, SDL_Rect& rectplbuttoncondition, SDL_Rect& srsrectplbutton);

void MainMenuDestroy(SDL_Texture* menu, SDL_Texture* plbutton);