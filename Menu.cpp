#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

void CreateMenu(SDL_Renderer* render, SDL_Surface* &mainmenu, SDL_Texture* &menu, SDL_Surface* &playbutton, SDL_Texture* &plbutton)
{

	mainmenu = IMG_Load("sprites/menu.png");
	menu = SDL_CreateTextureFromSurface(render, mainmenu);

	playbutton = IMG_Load("sprites/playbutton.png");
	plbutton = SDL_CreateTextureFromSurface(render, playbutton);
	
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
}	

void MainMenuDraw(SDL_Renderer* render, SDL_Texture* menu, SDL_Texture* plbutton, SDL_Rect &rectplbuttoncondition, SDL_Rect &srsrectplbutton)
{
	SDL_RenderCopy(render, menu, NULL, NULL);
	SDL_RenderCopy(render, plbutton, &rectplbuttoncondition, &srsrectplbutton);
}

void MainMenuDestroy(SDL_Texture* menu, SDL_Texture* plbutton)
{
	SDL_DestroyTexture(plbutton);
	SDL_DestroyTexture(menu);
}