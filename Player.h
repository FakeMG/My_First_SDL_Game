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
#include "LevelPart.h"
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
	static const int DEATH_ANIMATION_FRAMES = 4;

	SDL_Rect walkingClips[WALKING_ANIMATION_FRAMES];
	SDL_Rect idlingClips[IDLING_ANIMATION_FRAMES];
	SDL_Rect jumpingClips[JUMPING_ANIMATION_FRAMES];
	SDL_Rect fallingClips[FALLING_ANIMATION_FRAMES];
	SDL_Rect deathClips[DEATH_ANIMATION_FRAMES];
	int idleCounter = 0, walkCounter = 0, jumpCounter = 0, fallingCounter = 0, deathCounter = 0;

	bool grounded = false, running = false, idling = true, jumping = false, falling = true, dead = false, beingHit = false;
	float xVel = 0, yVel = 0;
	int groundSTT = 1; //số thứ tự của block đang đứng trên
	int levelSTT = 1; //stt của map đang xét
	SDL_Rect collision;
	vector<Bullet*> bulletList;
	Mix_Chunk* playerSFX[4];
public:
	Player(float p_x, float p_y, SDL_Texture* p_tex);
	enum SFX {
		hitSFX = 0,
		jumpSFX = 1,
		landSFX = 2,
		shootSFX = 3,
	};

	void handleInput(SDL_Event &events, Mix_Chunk* p_sfx[]);
	void update(vector<LevelPart>& LevelPartList, vector<Skeleton*> &skeletonList, Mix_Chunk* p_sfx[], SDL_Rect& camera);
	void jump();
	void gravity();
	void getHit(vector<Skeleton*> &skeletonList, Mix_Chunk* p_sfx[], SDL_Rect& camera);
	bool isDead() { return dead; }
	void knockBack();
	void handleCamera(SDL_Rect& camera, float& camVel);
	void render(SDL_Rect& p_camera);
	void resetPlayer() {
		x = 64 * 3; 
		y = LEVEL_HEIGHT - TILE_HEIGHT * 5;
		xVel = 0;
		yVel = 0;
		dead = false;
		flipType = SDL_FLIP_NONE;
	}
	void setBulletList(vector<Bullet*> bulletList) { this->bulletList = bulletList; }

	//getter
	vector<Bullet*> getBulletList() const { return bulletList; }
	SDL_Rect getCollision() const { return collision; }
};
