#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include <cstdlib>

#include "RenderWindow.h"
#include "Entity.h"
#include "Player.h"
#include "Skeleton.h"
#include "Timer.h"
#include "Tile.h"
#include "Menu.h"
#include "LevelPart.h"
using namespace std;

class Game {
private:
	SDL_Texture* knightTex = NULL;
	SDL_Texture* skeletonTex = NULL;
	SDL_Texture* tileTex = NULL;
	SDL_Texture* bulletTex = NULL;
	SDL_Texture* explosionTex = NULL;
	SDL_Texture* bgTex = NULL;
	SDL_Texture* buttonTex = NULL;
	SDL_Texture* circleOfLightTex = NULL;

	Mix_Music* bgMusic = NULL;
	Mix_Chunk* explosionSFX = NULL;
	Mix_Chunk* playerSFX[4] = { NULL };
	Mix_Chunk* skeletonSFX[2] = { NULL };

	//Time
	LTimer fpsTimer;
	stringstream timeText;
	stringstream scoreText;
	stringstream highscoreText;
	int score = 0;
	int highscore = 0;
	int countedFrames = 0;

	SDL_Rect gTileClips[TOTAL_TILE_SPRITES];
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	float camVel = 1.5;
	SDL_Rect explosion[15];
	int exCounter = 0;

	bool gameRunning = true;
	vector<path_pos> mapList;
	vector<LevelPart> LevelPartList;
	vector<Skeleton*> skeletonList;
	vector<Player> playerList;
	vector<Menu> menuList;
public:
	Game() {
		setSDL_Rect();
	}
	bool init();
	bool loadMedia();
	void FPSCounter();
	void renderScore();
	bool createMap();
	bool createLevel();
	bool createSkeleton();
	bool createPlayer();
	bool createMenu();
	void start_ExplosionSFX();
	void playMusic();
	void pauseMusicTime();
	void render_update_LevelPart();
	void render_update_bullet();
	void render_update_player();
	void render_update_skeleton();
	void render_update_explosion();
	void update_hight_score();
	void render_update_Game();
	void render_mainMenu();
	void resetGame();
	void handleGameInput(SDL_Event& event);
	bool isRunning() { return gameRunning; }
	vector<Menu> getMenuList() { return menuList; }
	void setSDL_Rect();
	void destroy() {
		SDL_DestroyTexture(knightTex);
		SDL_DestroyTexture(skeletonTex);
		SDL_DestroyTexture(tileTex);
		SDL_DestroyTexture(bulletTex);
		SDL_DestroyTexture(explosionTex);
		SDL_DestroyTexture(bgTex);
		SDL_DestroyTexture(buttonTex);
		SDL_DestroyTexture(bgTex);
		SDL_DestroyTexture(circleOfLightTex);
	}
};
