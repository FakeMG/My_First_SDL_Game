#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sstream>

#include "Entity.h"
using namespace std;

const float GRAVITY = 0.15;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int LEVEL_WIDTH = 1920;
const int LEVEL_HEIGHT = 1200;

class RenderWindow {
private:
	TTF_Font* font = NULL;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Surface* surface = NULL;

public:
	//Window
	RenderWindow(const char* p_title, int p_width, int p_height);

	//Texture
	SDL_Texture* loadTexture(const char* p_filePath);
	void renderTexture(Entity& entity, SDL_Rect* rec = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void renderAnimation(SDL_Texture* p_tex, int p_w, int p_h, float p_x, float p_y);
	void renderPlayer(Entity& player, float p_CamX, float p_CamY);

	//Font
	void loadFont(const char* filePath);
	SDL_Texture* createText(string p_text, SDL_Color p_textColor);

	void Color();
	void clearRenderer();
	void renderPresent();
	void cleanUp();
	
};