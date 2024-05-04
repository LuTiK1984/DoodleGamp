#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

void CreateMenu(SDL_Renderer* render, SDL_Surface* &mainmenu, SDL_Texture* &menu, SDL_Surface* &playbutton, SDL_Texture* &plbutton, SDL_Surface* &settingsbutton, SDL_Texture* &sttngsbutton)
{

	mainmenu = IMG_Load("sprites/atlas3.png");
	menu = SDL_CreateTextureFromSurface(render, mainmenu);

	playbutton = IMG_Load("sprites/playbutton.png");
	plbutton = SDL_CreateTextureFromSurface(render, playbutton);

	settingsbutton = IMG_Load("sprites/atlas2.png");
	sttngsbutton = SDL_CreateTextureFromSurface(render, settingsbutton);
	
	if (mainmenu == NULL)
	{
		printf("Couldn`t load menu! Error: %s", SDL_GetError());
		system("pause");
		exit(1);
	}

	SDL_FreeSurface(mainmenu);

	if (playbutton == NULL)
	{
		printf("Couldn`t load play button! Error: %s", SDL_GetError());
		system("pause");
		exit(1);
	}

	SDL_FreeSurface(playbutton);

	if (settingsbutton == NULL)
	{
		printf("Couldn`t load settings button! Error: %s", SDL_GetError());
		system("pause");
		exit(1);
	}

	SDL_FreeSurface(settingsbutton);
}	

void MainMenuDraw(SDL_Renderer* render, SDL_Texture* menu, SDL_Texture* plbutton, SDL_Texture* sttngsbutton, SDL_Rect &rectplbuttoncondition, SDL_Rect &srsrectplbutton, SDL_Rect& rectsttngsbuttoncondition, SDL_Rect& srsrectsttngsbutton, SDL_Rect& rectmenucondition, SDL_Rect &srsrectmenu)
{
	SDL_RenderCopy(render, menu, &rectmenucondition, &srsrectmenu);
	SDL_RenderCopy(render, plbutton, &rectplbuttoncondition, &srsrectplbutton);
	SDL_RenderCopy(render, sttngsbutton, &rectsttngsbuttoncondition, &srsrectsttngsbutton);
}

void MainMenuDestroy(SDL_Texture* menu, SDL_Texture* plbutton, SDL_Texture* sttngsbutton)
{
	SDL_DestroyTexture(plbutton);
	SDL_DestroyTexture(menu);
	SDL_DestroyTexture(sttngsbutton);
}

void VolumeSettings(bool IsUpPressed, bool IsDownPressed, int &VolumeMusic)
{
	if (IsUpPressed) {
		if (VolumeMusic < MIX_MAX_VOLUME)
			VolumeMusic += 2;
		else
			VolumeMusic = MIX_MAX_VOLUME;
		Mix_VolumeMusic(VolumeMusic);
		Mix_MasterVolume(VolumeMusic);
	}
	if (IsDownPressed) {
		if (VolumeMusic == 0)
			VolumeMusic = 0;
		else
			VolumeMusic -= 2;
		Mix_VolumeMusic(VolumeMusic);
		Mix_MasterVolume(VolumeMusic);
	}
}

void CreateSettings(SDL_Renderer* render, SDL_Surface* &volumesurf, SDL_Texture* &volumetexture)
{
	volumesurf = IMG_Load("sprites/volume.png");
	volumetexture = SDL_CreateTextureFromSurface(render, volumesurf);

	if (volumesurf == NULL)
	{
		printf("Couldn`t load volume text! Error: %s", SDL_GetError());
		system("pause");
		exit(1);
	}

	SDL_FreeSurface(volumesurf);
}