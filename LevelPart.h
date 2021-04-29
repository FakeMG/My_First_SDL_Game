#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <fstream>
#include <vector>

#include "RenderWindow.h"
#include "Tile.h"
using namespace std;

class LevelPart {
private:
	float x, y;
	Tile* tiles[TOTAL_TILES];
public:
	LevelPart(float p_x, float p_y);
	bool setTiles(SDL_Texture* tileTex);
	int getX() const { return x; }
	int getY() const { return y; }
};
 