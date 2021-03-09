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
using namespace std;

class Player : public Entity {
private:
	static const int PLAYER_VEL = 8;
	static const int WALKING_ANIMATION_FRAMES = 4;
	SDL_Rect walkingClips[WALKING_ANIMATION_FRAMES];
	SDL_Texture* idlingTex = NULL;
	SDL_Texture* jumpingTex = NULL;
	SDL_Texture* fallingTex = NULL;

	bool grounded = true, running = false, idling = true, falling = false;
	float xVel = 0, yVel = 0;
	SDL_Rect collision;
	vector<Bullet*> bulletList;
public:
	Player(float p_x, float p_y, SDL_Texture* p_tex);
	void handleInput(SDL_Event &events);
	void update(Tile *tile[]);
	bool jump();
	void handleCamera(SDL_Rect& camera);
	void render(SDL_Rect& p_camera, int &frame);
	void setBullet(vector<Bullet*> bulletList) { this->bulletList = bulletList; }
	vector<Bullet*> getBulletList() const { return bulletList; }
};
