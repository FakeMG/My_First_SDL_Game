#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "RenderWindow.h"
#include "Entity.h"
using namespace std;

class Tile : public Entity {
private:
	int tileType;
	SDL_Rect collision;
public:
	Tile(float p_x, float p_y, SDL_Texture* p_tex, int p_type);
	int getType();
	void setX(int p_x);
	void setType(int p_type) { tileType = p_type; }
	SDL_Rect getCollision();
};