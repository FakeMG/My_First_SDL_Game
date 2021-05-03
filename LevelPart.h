#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <fstream>

#include "RenderWindow.h"
#include "Tile.h"
class Tile;
using namespace std;

class LevelPart {
private:
	float x, y;
	vector<Tile*> tilesList;
	vector<float> skeletonPos;
public:
	LevelPart(float p_x, float p_y, const char* p_path, SDL_Texture* p_tileTex);
	void render(SDL_Rect p_TileClips[], SDL_Rect& p_camera);

	void setLevelX(LevelPart& p_level);
	void setLevelX(float p_x);
	void setTilesType(const char* p_path);
	void setSkeletonPos(vector<float>& p_skeletonPos) { skeletonPos = p_skeletonPos; }

	vector<Tile*> getTilesList() const { return tilesList; }
	vector<float> getSkeletonPos() const { return skeletonPos; }
	int getX() const { return x; }
	int getY() const { return y; }
};
 