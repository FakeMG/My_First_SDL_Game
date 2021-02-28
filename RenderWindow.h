#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sstream>

#include "Entity.h"


const float GRAVITY = 0.25;

//screen
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//level
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

//tile
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 12;

const int TILE_RED = 0;
const int TILE_GREEN = 1;
const int TILE_BLUE = 2;
const int TILE_CENTER = 3;
const int TILE_TOP = 4;
const int TILE_TOPRIGHT = 5;
const int TILE_RIGHT = 6;
const int TILE_BOTTOMRIGHT = 7;
const int TILE_BOTTOM = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_LEFT = 10;
const int TILE_TOPLEFT = 11;



class RenderWindow {
private:
	TTF_Font* font = NULL;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Surface* surface = NULL;
public:
	RenderWindow() {}
	//Window
	void create(const char* p_title, int p_width, int p_height);

	//Texture
	SDL_Texture* loadTexture(const char* p_filePath);
	void renderTexture(Entity& entity, SDL_Rect* rec = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void renderTile(Entity& entity, SDL_Rect& rec, SDL_Rect& camera);
	void renderAnimation(SDL_Texture* p_tex, int p_w, int p_h, float p_x, float p_y);
	void renderPlayer(Entity& player, SDL_Rect& camera);

	//Font
	void loadFont(const char* filePath);
	SDL_Texture* createText(string p_text, SDL_Color p_textColor);

	void Color();
	void clearRenderer();
	void renderPresent();
	void cleanUp();
};

	

