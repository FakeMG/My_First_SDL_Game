#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sstream>

#include "Entity.h"
#include "Player.h"

Player::Player(float p_x, float p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {
	collision.w = getCurrentFrame().w;
	collision.h = getCurrentFrame().h;

	for (int i = 0; i < 4; i++) {
		walkingClips[i].x = i * 64;
		walkingClips[i].y = 0;
		walkingClips[i].w = 64;
		walkingClips[i].h = 205;
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
			flipType = SDL_FLIP_NONE;
			running = true;
			idling = false;
			break;
		case SDLK_d:
			xVel += PLAYER_VEL;
			flipType = SDL_FLIP_HORIZONTAL;
			running = true;
			idling = false;
			break;
		case SDLK_SPACE:
			if (grounded) {
				jump();
			}
			break;
		default:
			idling = true;
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
			running = false;
			idling = true;
			break;
		case SDLK_d:
			xVel -= PLAYER_VEL;
			running = false;
			idling = true;
			break;
		default:
			break;
		}
	}
	else if (events.type == SDL_MOUSEBUTTONDOWN && events.key.repeat == 0) {
		Bullet* bullet = new Bullet(getX() + walkingClips->w, getY() + walkingClips->h/2, NULL);
		if (events.button.button == SDL_BUTTON_LEFT) {
			bullet->setWidthHeight(DEFAULTBULLET_W, DEFAULTBULLET_H);
			bullet->setType(Bullet::NORMAL);
			bullet->setFlipType(getFlipType());
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

	//move x
	x += xVel;
	collision.x = x;

	if (x < 0) {
		x = 0;
		collision.x = x;
	}
	if (x > LEVEL_WIDTH - currentFrame.w) {
		x = LEVEL_WIDTH - currentFrame.w;
		collision.x = x;
	}
	/*if (commonFunc::touchesWall(collision, tile, stt)) {
		x -= xVel;
		collision.x = x;
	}*/

	//move y
	y += yVel;
	collision.y = y;

	if (y < 0) {
		y = 0;
		collision.y = y;
	}
	if (y > LEVEL_HEIGHT - currentFrame.h) {
		y = LEVEL_HEIGHT - currentFrame.h;
		collision.y = y;
	}
	/*if (commonFunc::touchesWall(collision, tile, stt)) {
		y -= yVel;
		collision.y = y;
	}*/

	//Gravity
	if (!grounded) {
		yVel += GRAVITY;
		if (yVel > 40) yVel = 40;
	}
	/*else if( stand on ground() ) {
		grounded = true;
		yVel=0;
	}*/
}

bool Player::jump() {
	if (grounded) {
		yVel -= 7;
		grounded = false;
		return true;
	}
	return false;
}


void Player::handleCamera(SDL_Rect& camera) {
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

void Player::render(SDL_Rect &p_camera, int &frame) {
	if (running) {
		commonFunc::renderAnimation(tex, x, y, walkingClips[frame/4], p_camera, 0, NULL, flipType);
		frame++;
		if (frame/4 >= WALKING_ANIMATION_FRAMES) frame = 0;
	}

	if (idling) {
		commonFunc::renderAnimation(tex, x, y, walkingClips[0], p_camera, 0, NULL, flipType);
	}
}
