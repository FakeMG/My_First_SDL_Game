#pragma once
#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <math.h>

#include "RenderWindow.h"
#include "Player.h"
#include "Entity.h"

using namespace std;

class Skeleton : public Entity {
private:
	const int SKELETON_WIDTH = 64;
	const int SKELETON_HEIGHT = 64;
	const int SKELETON_VEL = 4;

	static const int WALKING_ANIMATION_FRAMES = 8;
	static const int IDLING_ANIMATION_FRAMES = 4;
	static const int FALLING_ANIMATION_FRAMES = 4;
	static const int ATTACKING_ANIMATION_FRAMES = 4;
	SDL_Rect walkingClips[WALKING_ANIMATION_FRAMES];
	SDL_Rect idlingClips[IDLING_ANIMATION_FRAMES];
	SDL_Rect fallingClips[FALLING_ANIMATION_FRAMES];
	SDL_Rect attackingClips[FALLING_ANIMATION_FRAMES];
	int idleCounter = 0, walkCounter = 0, fallingCounter = 0, attackingCounter = 0;
	bool grounded = true, walking = false, idling = true, falling = false, attacking = false;

	float xVel = 0, yVel = 0;
	SDL_Rect collision;
public:
	Skeleton(float p_x, float p_y, SDL_Texture* p_tex);
	void attack(Player& p_player);
	void move(Player& p_player);
	void update(Tile* tile[]);
	void render(SDL_Rect& p_camera);
};
