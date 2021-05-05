#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>

#include "Entity.h"
#include "LevelPart.h"
#include "Tile.h"
class LevelPart;
class Tile;

using namespace std;

const float GRAVITY = 0.3;
const float MAX_GRAVITY = 15;

//screen
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//level
const int LEVEL_WIDTH = 1344;
const int LEVEL_HEIGHT = 1024;

//tile
const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 64;
const int TOTAL_LEVEL_PART = 3;
const int TOTAL_MAP = 15;
const int TOTAL_TILES = 336;
const int TOTAL_TILE_SPRITES = 187;

static TTF_Font* font = NULL;
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Surface* surface = NULL;


struct path_pos {
	const char* path;
	vector<float> skeletonPos;
	path_pos(vector<float> p_skeletonPos, const char* p_path) {
		path = p_path;
		skeletonPos = p_skeletonPos;
	}
};

namespace commonFunc {
	//Window
	void renderWindow(const char* p_title, int p_width, int p_height);

	//Texture
	SDL_Texture* loadTexture(const char* p_filePath);
	void renderTexture(Entity& entity, SDL_Rect *rec = NULL, SDL_Rect *camera = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void renderTexture(SDL_Texture* p_tex, float p_x, float p_y, float p_w = 0, float p_h = 0, SDL_Rect* rec = NULL, SDL_Rect* camera = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void renderTile(Entity& entity, SDL_Rect& rec, SDL_Rect& camera);
	void renderPlayer(Entity& entity, SDL_Rect& camera);
	void renderAnimation(SDL_Texture* p_tex, float p_x, float p_y, SDL_Rect& p_clip, SDL_Rect& p_camera, double p_angle = 0.0, SDL_Point* p_center = NULL, SDL_RendererFlip p_flip = SDL_FLIP_NONE);

	bool checkCollision(SDL_Rect a, SDL_Rect b);
	bool touchesWall(SDL_Rect& box, vector<LevelPart>& LevelPartList);
	bool touchesWall(SDL_Rect& box, vector<LevelPart>& LevelPartList,bool& grounded, int& groundSTT, int& levelSTT);

	//Font
	bool loadFont(const char* filePath);
	SDL_Texture* createText(string p_text, SDL_Color p_textColor);

	void Color();
	void clearRenderer();
	void renderPresent();
	void cleanUp();
}


