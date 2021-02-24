#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sstream>

#include "RenderWindow.h"
#include "Entity.h"
using namespace std;

class Player : public Entity {
private:
	bool grounded = true;
	float xVel = 0, yVel = 0;
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
public:
	static const int PLAYER_VEL = 8;
	Player(float p_x, float p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {}
	void handleInput(SDL_Event &events);
	void update();
	bool jump();
	void handleCamera();
	SDL_Rect getCamera();
};
