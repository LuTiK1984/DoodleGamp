#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

void CreateMenu(SDL_Renderer* render, SDL_Surface* &mainmenu, SDL_Texture* &menu, SDL_Surface* &playbutton, SDL_Texture* &plbutton, SDL_Surface* &settingsbutton, SDL_Texture* &sttngsbutton)
{

	mainmenu = IMG_Load("sprites/menu.png");
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

void MainMenuDraw(SDL_Renderer* render, SDL_Texture* menu, SDL_Texture* plbutton, SDL_Texture* sttngsbutton, SDL_Rect &rectplbuttoncondition, SDL_Rect &srsrectplbutton, SDL_Rect& rectsttngsbuttoncondition, SDL_Rect& srsrectsttngsbutton)
{
	SDL_RenderCopy(render, menu, NULL, NULL);
	SDL_RenderCopy(render, plbutton, &rectplbuttoncondition, &srsrectplbutton);
	SDL_RenderCopy(render, sttngsbutton, &rectsttngsbuttoncondition, &srsrectsttngsbutton);
}

void MainMenuDestroy(SDL_Texture* menu, SDL_Texture* plbutton, SDL_Texture* sttngsbutton)
{
	SDL_DestroyTexture(plbutton);
	SDL_DestroyTexture(menu);
	SDL_DestroyTexture(sttngsbutton);
}