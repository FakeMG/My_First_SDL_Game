#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Entity.h"
#include "RenderWindow.h"

using namespace std;

const int DEFAULTBULLET_W = 64*5;
const int DEFAULTBULLET_H = 64;

const int BULLETSPEED = 5;

class Bullet : public Entity {
private:
	bool moving;
	int bulletType;
	SDL_Rect collision;
public:
	enum bulletType {
		NONE = 0,
		NORMAL = 1,
	};
	Bullet(float p_x, float p_y, SDL_Texture* p_tex);
	void handelInput(SDL_Event &events);
	void move();
	void render(SDL_Rect& camera, SDL_Texture* p_tex);

	//getter
	int getType() const { return bulletType; }
	bool isMoving() const { return moving; }
	SDL_Rect getCollision() { return collision; }

	//setter
	void setFlipType(SDL_RendererFlip p_PlayerflipType);
	void setType(const int& p_type) { bulletType = p_type; }
	void setMove(bool p_move) { moving = p_move; }
	void setWidthHeight(const int &p_width, const int &p_height);
};

