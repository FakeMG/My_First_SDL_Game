#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sstream>

#include "RenderWindow.h"
#include "Entity.h"
#include "Player.h"


void Player::handleInput(SDL_Event &events) {
	if (events.type == SDL_KEYDOWN && events.key.repeat == 0) {
		switch (events.key.keysym.sym) {
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

	}
	else if (events.type == SDL_MOUSEBUTTONUP && events.key.repeat == 0) {

	}
}

void Player::update() {
	//move x
	x += xVel;
	if (x < 0) x = 0;
	if (x > LEVEL_WIDTH - currentFrame.w) x = LEVEL_WIDTH - currentFrame.w;

	//move y
	y += yVel;
	if (y < 0) y = 0;
	if (y > LEVEL_HEIGHT - currentFrame.h) y = LEVEL_HEIGHT - currentFrame.h;

	//Gravity
	if (!grounded) {
		yVel += GRAVITY;
	}
	/*else if( stand on ground() ) {
		grounded = true;
		yVel=0;
	}*/
}

bool Player::jump() {
	if (grounded) {
		yVel -= 5;
		grounded = false;
		return true;
	}
	return false;
}


void Player::handleCamera() {
	//Di chuyển camera theo nhân vật
	camera.x = (getX() + getCurrentFrame().w / 2) - SCREEN_WIDTH / 2;
	camera.y = (getY() + getCurrentFrame().h / 2) - SCREEN_HEIGHT / 2;

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

SDL_Rect Player::getCamera() {
	return camera;
}