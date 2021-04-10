#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>

#include "RenderWindow.h"
#include "Entity.h"
#include "Bullet.h"
#include "Skeleton.h"
class Skeleton;
using namespace std;

class Player : public Entity {
private:
	const int PLAYER_WIDTH = 64;
	const int PLAYER_HEIGHT = 64;
	const int PLAYER_VEL = 6;

	//animation
	static const int WALKING_ANIMATION_FRAMES = 8;
	static const int IDLING_ANIMATION_FRAMES = 4;
	static const int JUMPING_ANIMATION_FRAMES = 4;
	static const int FALLING_ANIMATION_FRAMES = 4;

	SDL_Rect walkingClips[WALKING_ANIMATION_FRAMES];
	SDL_Rect idlingClips[IDLING_ANIMATION_FRAMES];
	SDL_Rect jumpingClips[JUMPING_ANIMATION_FRAMES];
	SDL_Rect fallingClips[FALLING_ANIMATION_FRAMES];
	int idleCounter = 0, walkCounter = 0, jumpCounter = 0, fallingCounter = 0;

	bool grounded = false, running = false, idling = true, jumping = false, falling = false, dead = false, beingHit = false;
	float xVel = 0, yVel = 0;
	int groundSTT = 1; //số thứ tự của block đang đứng trên
	SDL_Rect collision;
	vector<Bullet*> bulletList;
public:
	Player(float p_x, float p_y, SDL_Texture* p_tex);

	void handleInput(SDL_Event &events);
	void update(Tile *tile[], Skeleton& p_skeleton);
	void jump();
	void gravity();
	void getHit(Skeleton& p_skeleton);
	bool isDead() { return dead; }
	void knockBack();
	void handleCamera(SDL_Rect& camera);
	void render(SDL_Rect& p_camera);
	void setBulletList(vector<Bullet*> bulletList) { this->bulletList = bulletList; }

	//getter
	vector<Bullet*> getBulletList() const { return bulletList; }
	SDL_Rect getCollision() const { return collision; }
};
