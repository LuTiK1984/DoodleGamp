#include <iostream>
#include <SDL.h>
SDL_Window* win = NULL;
SDL_Renderer* render = NULL;

int win_width = 800, win_height = 600;

const int Max_Rects = 1000;
int Rect_Size = 20;
int Interval = 30;
int Trail_Length = 10;

struct Block
{
	int x = win_width / 2;
	int y = win_height / 2;
	int h = 40;
	int w = 40;
	int choosecolor = 0;
	SDL_Color color;
}block;

struct Rectangle {
	int x, y;
	int transparency;
};

void DeInit(int error)
{
	if (render != NULL) SDL_DestroyRenderer(render);
	if (win != NULL) SDL_DestroyWindow(win);
	SDL_Quit();
	exit(error);
}

void Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("Init Error! - %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}

	win = SDL_CreateWindow(
		"Practice 6",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		win_width,
		win_height,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	if (win == NULL)
	{
		printf("Window Error! - %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}

	render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	if (render == NULL)
	{
		printf("Renderer Error! - %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}
}
int Square_Size = 50;

int Square_Vision_Time = 1500;

int Miss_Position = 50;
int Clicked_Square = 100;

int score = 0;


struct Position {
	int x, y;
};

Position Square_Positions[] = {
		{ win_width * 3 / 4,win_height * 3 / 4 },
		{win_width / 6, win_height / 7},
		{win_width * 3 / 4, win_height * 3 / 4},
		{win_width / 3, win_height / 3},
		{win_width * 2 / 3, win_height * 2 / 3},
		{win_width / 5, win_height * 4 / 5},
		{win_width * 4 / 5, win_height / 5}
};
void UpdatePosition(int& win_width, int& win_height, Position Square_Positions[])
{
	Square_Positions[0] = { win_width * 3 / 4, win_height * 3 / 4 };
	Square_Positions[1] = { win_width / 6, win_height / 7 };
	Square_Positions[2] = { win_width * 3 / 4, win_height * 3 / 4 };
	Square_Positions[3] = { win_width / 3, win_height / 3 };
	Square_Positions[4] = { win_width * 2 / 3, win_height * 2 / 3 };
	Square_Positions[5] = { win_width / 5, win_height * 4 / 5 };
	Square_Positions[6] = { win_width * 4 / 5, win_height / 5 };
}

struct Square {
	int x, y, TimeVision;
	bool visible;
};

Square squares[2];

int random(int min, int max) {
	return min + rand() % (max - min + 1);
}

void generateSquare(Square& square, bool pause) {
	int index = random(0, 7);
	square.x = Square_Positions[index].x;
	square.y = Square_Positions[index].y;
	square.visible = true;
	square.TimeVision = SDL_GetTicks();
}

bool checkSquareTime(Square& square) {
	int currentTime = SDL_GetTicks();
	if (square.visible and (currentTime - square.TimeVision >= Square_Vision_Time))
		return true;
	else
		return false;
}

void drawSquare(SDL_Renderer* render, Square& square) {
	if (square.visible) {
		SDL_SetRenderDrawColor(render, 20, 50, 25, 255);
		SDL_Rect squareRect = { square.x, square.y, Square_Size, Square_Size };
		SDL_RenderFillRect(render, &squareRect);
	}
}

void updateScreen(SDL_Renderer* renderer, Square squares[], int numSquares, int score) {

	for (int i = 0; i < numSquares; i++) {
		drawSquare(renderer, squares[i]);
	}
}

void DrawBlock()
{
	switch (block.choosecolor)
	{
	case 0:
		block.color = { 255, 0, 0, 255 }; //красный
		break;
	case 1:
		block.color = { 220, 150, 50, 255 }; //оранжевый
		break;
	case 2:
		block.color = { 255, 240, 0, 255 }; //желтый
		break;
	case 3:
		block.color = { 0, 255, 0, 255 }; //зеленый
		break;
	case 4:
		block.color = { 0, 160, 230, 255 }; //голубой
		break;
	case 5:
		block.color = { 60, 70, 200, 255 }; //синий
		break;
	case 6:
		block.color = { 165, 70, 165, 255 }; //фиолетовый
		break;
	}

	SDL_SetRenderDrawColor(render, block.color.r, block.color.g, block.color.b, block.color.a);
	SDL_Rect rect = { block.x, block.y, block.h, block.w };
	SDL_RenderFillRect(render, &rect);
}

void AddRect(Rectangle rectangles[], int& numRects, int& trailIndex, int mouseX, int mouseY) {
	if (numRects < Max_Rects) {
		rectangles[trailIndex] = { mouseX - Rect_Size / 2, mouseY - Rect_Size / 2, 255 };
		trailIndex = (trailIndex + 1) % Trail_Length;
		if (numRects < Trail_Length)
			numRects += 1;
		else numRects = Trail_Length;
	}
}

void AddRectByButton(Rectangle rectangles[], int& numRects, int& trailIndex, int mouseX, int mouseY)
{
	SDL_GetMouseState(&mouseX, &mouseY);
	AddRect(rectangles, numRects, trailIndex, mouseX, mouseY);
	Trail_Length++;
}

void RemoveRect(int& numRects) {
	if (numRects > 1 && Trail_Length > 1) {
		numRects--;
		Trail_Length--;
	}
}

void UpdateRects(Rectangle rectangles[], int& numRects, int& trailIndex, int mouseX, int mouseY, int lastRectTime)
{
	int currentTime = SDL_GetTicks();
	if (currentTime - lastRectTime >= Interval && numRects < Max_Rects) {

		SDL_GetMouseState(&mouseX, &mouseY);
		AddRect(rectangles, numRects, trailIndex, mouseX, mouseY);
		lastRectTime = currentTime;
	}
}

void DrawRect(Rectangle rectangles[], int& numRects, int& trailIndex)
{
	for (int i = 1; i < numRects; i++) {
		int index = (trailIndex - i + Trail_Length) % Trail_Length;
		SDL_SetRenderDrawColor(render, 100, 255, 255, rectangles[index].transparency);
		SDL_Rect rect = { rectangles[index].x, rectangles[index].y, Rect_Size, Rect_Size };
		SDL_RenderFillRect(render, &rect);
		rectangles[index].transparency = 255 - 255 / (numRects)*i;
		if (numRects < 2) rectangles[index].transparency = 255;
	}
}

int main(int arcg, char* argv[])
{
	srand(time(NULL));
	system("chcp 1251");
	Init();
	SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);

	SDL_Event ev;
	bool isRunning = true;
	bool isUpPressed = false;
	bool isDownPressed = false;
	bool isRightPressed = false;
	bool isLeftPressed = false;

	Rectangle rectangles[Max_Rects];
	int numRects = 0;
	int trailIndex = 0;

	int lastRectTime = SDL_GetTicks();

	int mouseX = 0, mouseY = 0;

	bool pause = false;
	while (isRunning)
	{

#pragma region EVENTS
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{

			case SDL_QUIT: //Выход из приложения
				if (pause)
				{
					isRunning = false;
					printf("Your score: %d\n", score);
					system("pause");
				}
				pause = true;
				break;

			case SDL_WINDOWEVENT:
				if (ev.window.event == SDL_WINDOWEVENT_RESIZED) //Изменения размеров окна
				{
					win_width = ev.window.data1;
					win_height = ev.window.data2;
					UpdatePosition(win_width, win_height, Square_Positions);
				}
				break;

			case SDL_KEYDOWN: //Обработка зажатия клавиш клавиатуры
				pause = false;
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_RIGHT: //Клавиша стрелка вправо
					isRightPressed = true;
					break;
				case SDL_SCANCODE_LEFT: //Клавиша стрелка влево
					isLeftPressed = true;
					break;
				case SDL_SCANCODE_UP: //Клавиша стрелка вверх
					isUpPressed = true;
					break;
				case SDL_SCANCODE_DOWN: //Клавиша стрелка вниз
					isDownPressed = true;
					break;
				case SDL_SCANCODE_PAGEUP:
					if (block.choosecolor == 6) block.choosecolor = 0;
					else block.choosecolor++;
					break;
				case SDL_SCANCODE_PAGEDOWN:
					if (block.choosecolor == 0) block.choosecolor = 6;
					else block.choosecolor--;
					break;
				}
				break;

			case SDL_KEYUP: //Обработка отпускания клавиш клавиатуры
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_RIGHT: //Клавиша стрелка вправо
					isRightPressed = false;
					break;
				case SDL_SCANCODE_LEFT: //Клавиша стрелка влево
					isLeftPressed = false;
					break;
				case SDL_SCANCODE_UP: //Клавиша стрелка вверх
					isUpPressed = false;
					break;
				case SDL_SCANCODE_DOWN: //Клавиша стрелка вниз
					isDownPressed = false;
					break;
				}
				break;
				// Для whack_a_mole
			case SDL_MOUSEBUTTONDOWN:
			{
				SDL_GetMouseState(&mouseX, &mouseY);
				bool IsclickedOnSquare = false;
				for (int i = 0; i < 2; i++) {
					if (mouseX >= squares[i].x && mouseX <= squares[i].x + Square_Size && mouseY >= squares[i].y && mouseY <= squares[i].y + Square_Size && squares[i].visible) {
						squares[i].visible = false;
						score += Clicked_Square;
						IsclickedOnSquare = true;
					}
				}
				if (!IsclickedOnSquare) {
					score -= Miss_Position;
				}
				if (ev.button.button == SDL_BUTTON_LEFT)
					AddRectByButton(rectangles, numRects, trailIndex, mouseX, mouseY);
				else if (ev.button.button == SDL_BUTTON_RIGHT)
					RemoveRect(numRects);
				break;
			}
			}
		}

#pragma endregion


#pragma region LOGIC
		for (int i = 0; i < 2; ++i) {
			if (!squares[i].visible || checkSquareTime(squares[i]))
				generateSquare(squares[i], pause);
			if (squares[i].visible and pause)
				squares[i].visible = false;
		}
		if (!pause)
		{

			if (isUpPressed && !isDownPressed && block.y > 0) block.y -= 2;
			if (!isUpPressed && isDownPressed && ((block.y + block.h) < win_height)) block.y += 2;
			if (isRightPressed && !isLeftPressed && ((block.x + block.w) < win_width)) block.x += 2;
			if (!isRightPressed && isLeftPressed && block.x > 0) block.x -= 2;

			UpdateRects(rectangles, numRects, trailIndex, mouseX, mouseY, lastRectTime);
#pragma endregion


#pragma region DRAWING
			SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
			SDL_RenderClear(render);

			DrawBlock();

			updateScreen(render, squares, 2, score);

			DrawRect(rectangles, numRects, trailIndex);

			SDL_Delay(10);
		}

		else
		{
			SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
			SDL_RenderClear(render);
		}
		SDL_RenderPresent(render);
#pragma endregion

	}

	DeInit(0);
	return 0;
}