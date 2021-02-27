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
public:
	static const int PLAYER_VEL = 8;
	Player(float p_x, float p_y, SDL_Texture* p_tex);
	void handleInput(SDL_Event &events);
	void update();
	bool jump();
	void handleCamera(SDL_Rect& camera);
	void render(SDL_Rect &camera);
};
