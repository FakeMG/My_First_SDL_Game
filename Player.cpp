#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sstream>

#include "Entity.h"
#include "Player.h"

Player::Player(float p_x, float p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {
	collision.x = getX() + PLAYER_WIDTH;
	collision.y = getY() + PLAYER_HEIGHT;
	collision.w = PLAYER_WIDTH - 12;
	collision.h = PLAYER_HEIGHT;

	for (int i = 0; i < WALKING_ANIMATION_FRAMES; i++) {
		walkingClips[i].x = i * (getCurrentFrame().w/4);
		if (i >= 4) {
			walkingClips[i].x = (i-4) * (getCurrentFrame().w/4);
			walkingClips[i].y = (getCurrentFrame().h/5)*2;
		}
		else walkingClips[i].y = getCurrentFrame().h/5;
		walkingClips[i].w = getCurrentFrame().w/4;
		walkingClips[i].h = getCurrentFrame().h/5;
	}
	for (int i = 0; i < IDLING_ANIMATION_FRAMES; i++) {
		idlingClips[i].x = i * (getCurrentFrame().w/4);
		idlingClips[i].y = 0;
		idlingClips[i].w = getCurrentFrame().w/4;
		idlingClips[i].h = getCurrentFrame().h/5;
	}
	for (int i = 0; i < JUMPING_ANIMATION_FRAMES; i++) {
		jumpingClips[i].x = i * (getCurrentFrame().w / 4);
		jumpingClips[i].y = (getCurrentFrame().h/5) * 3;
		jumpingClips[i].w = getCurrentFrame().w / 4;
		jumpingClips[i].h = getCurrentFrame().h / 5;
	}
	for (int i = 0; i < FALLING_ANIMATION_FRAMES; i++) {
		fallingClips[i].x = i * (getCurrentFrame().w / 4);
		fallingClips[i].y = (getCurrentFrame().h / 5) * 4;
		fallingClips[i].w = getCurrentFrame().w / 4;
		fallingClips[i].h = getCurrentFrame().h / 5;
	}
}

void Player::handleInput(SDL_Event &events) {
	if (events.type == SDL_KEYDOWN && events.key.repeat == 0) {
		switch (events.key.keysym.sym) {
		case SDLK_w:
			yVel -= PLAYER_VEL;
			break;
		case SDLK_s:
			yVel += PLAYER_VEL;
			break;
		case SDLK_a:
			xVel -= PLAYER_VEL;
			break;
		case SDLK_d:
			xVel += PLAYER_VEL;
			break;
		case SDLK_SPACE:
			if (grounded) {
				jump();
			}
			break;
		default:
			break;
		}
	}
	else if (events.type == SDL_KEYUP && events.key.repeat == 0) {
		switch (events.key.keysym.sym) {
		case SDLK_w:
			yVel += PLAYER_VEL;
			break;
		case SDLK_s:
			yVel -= PLAYER_VEL;
			break;
		case SDLK_a:
			xVel += PLAYER_VEL;
			break;
		case SDLK_d:
			xVel -= PLAYER_VEL;
			break;
		default:
			break;
		}
	}
	else if (events.type == SDL_MOUSEBUTTONDOWN && events.key.repeat == 0) {
		Bullet* bullet = new Bullet(getCollision().x + PLAYER_WIDTH*1.25, getCollision().y, NULL);
		if (events.button.button == SDL_BUTTON_LEFT) {
			bullet->setFlipType(getFlipType());
			bullet->setSize_Position(DEFAULTBULLET_W, DEFAULTBULLET_H, getX());
			bullet->setType(Bullet::NORMAL);
		}
		bullet->setMove(true);
		bulletList.push_back(bullet);
	}
	else if (events.type == SDL_MOUSEBUTTONUP && events.key.repeat == 0) {
		//chua biet lam gi
	}
}

void Player::update(Tile* tile[]) {
	int stt = 0; //stt của tile

	if (xVel == 0 && yVel == 0 && grounded) idling = true;
	else idling = false;

	if (xVel != 0 && grounded) running = true;
	else running = false;

	if (xVel < 0) flipType = SDL_FLIP_HORIZONTAL;
	if (xVel > 0) flipType = SDL_FLIP_NONE;

	if (yVel > 0 && !grounded) falling = true;
	else falling = false;

	if (yVel < 0) jumping = true;
	else jumping = false;

	//move x
	x += xVel;
	collision.x = getX() + PLAYER_WIDTH;

	if (getX() + PLAYER_WIDTH < 0) {
		x = -PLAYER_WIDTH;
		collision.x = getX() + PLAYER_WIDTH;
	}
	if (getX() + 2*PLAYER_HEIGHT > LEVEL_WIDTH) {
		x = LEVEL_WIDTH - 2*PLAYER_HEIGHT;
		collision.x = x;
	}
	if (commonFunc::touchesWall(collision, tile, stt)) {
		x -= xVel;
		collision.x = getX() + PLAYER_WIDTH;
	}

	//move y
	y += yVel;
	collision.y = getY() + PLAYER_HEIGHT;

	if (getY() + PLAYER_HEIGHT < 0) {
		y = -PLAYER_HEIGHT;
		collision.y = getY() + PLAYER_HEIGHT;
	}
	if (getY() + 2*PLAYER_HEIGHT > LEVEL_HEIGHT) {
		y = LEVEL_HEIGHT - 2 * PLAYER_HEIGHT;
		collision.y = getY() + PLAYER_HEIGHT;
	}
	if (commonFunc::touchesWall(collision, tile, stt)) {
		y -= yVel;
		collision.y = getY() + PLAYER_HEIGHT;
	}

	//Gravity
	if (!grounded) {
		yVel += GRAVITY;
		if (yVel > MAX_GRAVITY) yVel = MAX_GRAVITY;
	}
	/*else if( stand on ground() ) {
		grounded = true;
		yVel=0;
	}*/
}

void Player::jump() {
	if (grounded) {
		yVel -= 7;
		grounded = false;
	}
}


void Player::handleCamera(SDL_Rect& camera) {
	//Di chuyển camera theo nhân vật
	camera.x = (getX() + PLAYER_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (getY() + PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

	//Giới hạn 
	if (camera.x < 0) {
		camera.x = 0;
	}
	if (camera.y < 0) {
		camera.y = 0;
	}
	if (camera.x > LEVEL_WIDTH - camera.w) {
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if (camera.y > LEVEL_HEIGHT - camera.h) {
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

void Player::render(SDL_Rect &p_camera) {
	if (running) {
		commonFunc::renderAnimation(tex, x, y, walkingClips[walkCounter / 4], p_camera, 0, NULL, getFlipType());
		walkCounter++;
		if (walkCounter/4 >= WALKING_ANIMATION_FRAMES) walkCounter = 0;
	}
	else walkCounter = 0;

	if (idling) {
		commonFunc::renderAnimation(tex, x, y, idlingClips[idleCounter/6], p_camera, 0, NULL, getFlipType());
		idleCounter++;
		if (idleCounter / 6 >= IDLING_ANIMATION_FRAMES) idleCounter = 0;
	}
	else idleCounter = 0;

	if (jumping) {
		commonFunc::renderAnimation(tex, x, y, jumpingClips[jumpCounter / 6], p_camera, 0, NULL, getFlipType());
		jumpCounter++;
		if (jumpCounter / 6 >= JUMPING_ANIMATION_FRAMES) jumpCounter = 0;
	}
	else jumpCounter = 0;

	if (falling) {
		commonFunc::renderAnimation(tex, x, y, fallingClips[fallingCounter / 4], p_camera, 0, NULL, getFlipType());
		fallingCounter++;
		if (fallingCounter / 4 >= FALLING_ANIMATION_FRAMES) fallingCounter = 0;
	}
	else fallingCounter = 0;
}
