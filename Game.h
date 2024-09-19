#pragma once

struct Player
{
	int x =0, y=0, w=0, h=0;
	SDL_Rect movementbox = {x,y,50,10};
	int a=0;
	bool isJump = false;
	bool isFlip = false;
	int score = 0;
};

struct Platform
{
	SDL_Rect platformposition = { 0, 0, 110, 30 };
	int type;
	int movetick;
	int v;
};

struct Enemy
{
	SDL_Rect position;
	int animationtick;
	int movetick;
	int v;
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

void RegeneratePlatform(Platform platforms[], int num);

void UpdatePlatforms(Platform platforms[], int num);

void CreateFloatPlatforms(SDL_Renderer* render, SDL_Surface*& platformsurf, SDL_Texture*& platformtexture);

void GenerateFloatPlatforms(Platform platforms[], int num, int x);

void FloatPlatformsMove(Platform platforms[], int num, int x);

void CreateEnemy(SDL_Renderer* render, SDL_Surface*& playersurf, SDL_Texture*& playertexture);

void DrawEnemy(SDL_Renderer* render, SDL_Texture* enemytexture, SDL_Rect enemycondition, SDL_Rect enemyposition);

void DestroyEnemy(SDL_Texture* enemy);

void GenerateEnemies(Enemy enemies[], int num, SDL_Rect *enemycondition);

void RegenerateEnemy(Enemy enemies[], int num);

void UpdateEnemy(Enemy enemies[], int num);

void EnemiesMove(Enemy enemies[], int num, SDL_Rect *enemycondition);

void CreateBrokenPlatforms(SDL_Renderer* render, SDL_Surface*& platformsurf, SDL_Texture*& platformtexture);

void GenerateBrokenPlatforms(Platform platforms[], int num);

void PlayerJump(Player& player, int win_width, bool isRightPressed, bool isLeftPressed, SDL_Rect& playercondition, Mix_Chunk* jumpsfx);

void MoveMap(Player &player, Platform platforms[], Platform floatplatforms[], Platform brokenplatforms[], Enemy enemies[], int term);

void CheckCollisionPlatforms(Player &player, Platform platforms[], Platform floatplatforms[], Platform brokenplatforms[], Enemy enemies[]);

void CheckCollisionFloatPlatforms(Player &player, Platform platforms[], Platform floatplatforms[], Platform brokenplatforms[], Enemy enemies[]);

void CheckCollisionBrokenPlatforms(Player &player, Platform brokenplatforms[], Mix_Chunk* brokeplatform);

void CheckEnemyCollision(Player &player, Platform platforms[], Platform floatplatforms[], Platform brokenplatforms[], Enemy enemies[], Mix_Chunk* monsterapproaching, Mix_Chunk* jumponmonster);

void ReadRecord(int& record);

void SaveRecord(int& record);

void CheckLose(Player& player, Platform platforms[], Platform floatplatforms[], Platform brokenplatforms[], Enemy enemies[], Mix_Chunk* falling, Mix_Chunk* deathfrommonster, SDL_Rect& playerposition, SDL_Rect& enemycondition, int win_height, int win_width, bool& isGame, int& bestrecord);