#pragma once

void CreateMenu(SDL_Renderer* render, SDL_Surface*& mainmenu, SDL_Texture*& menu, SDL_Surface*& playbutton, SDL_Texture*& plbutton, SDL_Surface*& settingsbutton, SDL_Texture*& sttngsbutton);

void MainMenuDraw(SDL_Renderer* render, SDL_Texture* menu, SDL_Texture* plbutton, SDL_Texture* sttngsbutton, SDL_Rect& rectplbuttoncondition, SDL_Rect& srsrectplbutton, SDL_Rect& rectsttngsbuttoncondition, SDL_Rect& srsrectsttngsbutton, SDL_Rect& rectmenucondition, SDL_Rect& srsrectmenu);

void MainMenuDestroy(SDL_Texture* menu, SDL_Texture* plbutton, SDL_Texture* sttngsbutton);

void VolumeSettings(bool IsUpPressed, bool IsDownPressed, int& VolumeMusic);

void CreateSettings(SDL_Renderer* render, SDL_Surface*& volumesurf, SDL_Texture*& volumetexture);