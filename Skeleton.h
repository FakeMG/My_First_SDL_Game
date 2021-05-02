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
#include "LevelPart.h"
class Player;
using namespace std;

class Skeleton : public Entity {
private:
	const int SKELETON_WIDTH = 64;
	const int SKELETON_HEIGHT = 64;
	const int SKELETON_VEL = 4;

	static const int WALKING_ANIMATION_FRAMES = 4;
	static const int IDLING_ANIMATION_FRAMES = 4;
	static const int FALLING_ANIMATION_FRAMES = 4;
	static const int ATTACKING_ANIMATION_FRAMES = 4;
	static const int BEINGHIT_ANIMATION_FRAMES = 4;
	SDL_Rect walkingClips[WALKING_ANIMATION_FRAMES];
	SDL_Rect idlingClips[IDLING_ANIMATION_FRAMES];
	SDL_Rect fallingClips[FALLING_ANIMATION_FRAMES];
	SDL_Rect attackingClips[FALLING_ANIMATION_FRAMES];
	SDL_Rect beingHitClips[BEINGHIT_ANIMATION_FRAMES];
	int idleCounter = 0, walkCounter = 0, fallingCounter = 0, attackingCounter = 0, beingHitCounter = 0;
	bool grounded = true, walking = false, idling = true, falling = false, attacking = false, beingHit = false, dead = false;

	float xVel = 0, yVel = 0;
	int maxHealth = 3;
	int groundSTT = 1; //số thứ tự của block đang đứng trên
	int levelSTT = 1;
	float distanceToPlayer;
	SDL_Rect collision;
public:
	Skeleton(float p_x, float p_y, SDL_Texture* p_tex);

	void update(Player& p_player, vector<LevelPart>& LevelPartList, Mix_Chunk* p_sfx[], SDL_Rect& camera);
	void gravity();
	void autoMovement(vector<LevelPart>& LevelPartList);
	void moveToPlayer(Player& p_player, vector<LevelPart>& LevelPartList);
	bool isDead() { return dead; }
	bool isAttacking();
	void getHit(Player& p_player, Mix_Chunk* p_sfx[], SDL_Rect& camera);
	void knockBack();
	void render(SDL_Rect& p_camera);

	SDL_Rect getCollision() const { return collision; }
	float getDistance() { return distanceToPlayer; }
};
