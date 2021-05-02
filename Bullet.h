#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Entity.h"
#include "RenderWindow.h"
#include "LevelPart.h"

using namespace std;

const int DEFAULTBULLET_W = 64;
const int DEFAULTBULLET_H = 64;
const float BULLETSPEED = 2.5;

class Bullet : public Entity {
private:
	float xVel = 7;
	bool moving = false;
	int bulletType;
	SDL_Rect collision;

	static const int BULLET_ANIMATION_FRAMES = 5;
	SDL_Rect bulletClips[BULLET_ANIMATION_FRAMES];
	int bulletCounter = 0;
	const int delay = 4;
public:
	enum bulletType {
		NONE = 0,
		NORMAL = 1,
	};
	Bullet(float p_x, float p_y, SDL_Texture* p_tex);
	void update(vector<LevelPart>& LevelPartList);
	void render(SDL_Rect& camera, SDL_Texture* p_tex);

	//getter
	int getType() const { return bulletType; }
	bool isMoving() const { return moving; }
	SDL_Rect getCollision() { return collision; }

	//setter
	void setFlipType(SDL_RendererFlip p_PlayerflipType);
	void setType(const int& p_type) { bulletType = p_type; }
	void setMove(bool p_move) { moving = p_move; }
	void setSize_Position(const int &p_width, const int &p_height, const int &p_playerX);
};

