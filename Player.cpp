#include "Player.h"

Player::Player(float p_x, float p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {
	collision.x = getX() + PLAYER_WIDTH;
	collision.y = getY() + PLAYER_HEIGHT;
	collision.w = PLAYER_WIDTH-12; //cho vừa với chân nhân vật
	collision.h = PLAYER_HEIGHT-2; //cho nhân vật vừa trong 1 ô

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
	if (!dead) {
		if (events.type == SDL_KEYDOWN && events.key.repeat == 0) {
			switch (events.key.keysym.sym) {
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
			case SDLK_a:
				xVel += PLAYER_VEL;
				break;
			case SDLK_d:
				xVel -= PLAYER_VEL;
				break;
			case SDLK_SPACE:
				if (!grounded && jumping) {
					yVel *= .5f;
				}
				break;
			default:
				break;
			}
		}
		else if (events.type == SDL_MOUSEBUTTONDOWN && events.key.repeat == 0) {
			Bullet* bullet = new Bullet(getCollision().x + PLAYER_WIDTH * 1.25, getCollision().y, NULL);
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
}

void Player::update(Tile* tile[], Skeleton &p_skeleton) {
	gravity();
	getHit(p_skeleton);
	//knockBack();

	// set trạng thái Player
	if (xVel == 0 && grounded && !dead) idling = true;
	else idling = false;

	if (xVel != 0 && grounded && !dead) running = true;
	else running = false;

	if (yVel > 0 && !grounded && !dead) falling = true;
	else falling = false;

	if (yVel <= 0 && !dead) jumping = true;
	else jumping = false;

	if (!beingHit) {
		if (xVel < 0) flipType = SDL_FLIP_HORIZONTAL;
		if (xVel > 0) flipType = SDL_FLIP_NONE;
	}

	//move x
	if (!dead) {
		x += xVel;
		collision.x = getX() + PLAYER_WIDTH;

		if (getX() + PLAYER_WIDTH < 0) {
			x = -PLAYER_WIDTH;
			collision.x = getX() + PLAYER_WIDTH;
		}
		if (getX() + 2 * PLAYER_HEIGHT > LEVEL_WIDTH) {
			x = LEVEL_WIDTH - 2 * PLAYER_HEIGHT;
			collision.x = getX() + PLAYER_WIDTH;
		}
		if (commonFunc::touchesWall(collision, tile)) {
			x -= xVel;
			collision.x = getX() + PLAYER_WIDTH;
		}
	}

	//move y
	y += yVel;
	collision.y = getY() + PLAYER_HEIGHT;
	if (getY() + PLAYER_HEIGHT < 0) {
		y = -PLAYER_HEIGHT;
		collision.y = getY() + PLAYER_HEIGHT;
	}
	if (commonFunc::touchesWall(collision, tile, groundSTT)) {
		if (yVel > 0) {
			y = tile[groundSTT]->getY() - 64 * 2 - 0.1 + 2;
			if (falling) {
				grounded = true;
			}
		}
		else if (yVel < 0) {
			y -= yVel;
			yVel = 0;
		}
		collision.y = getY() + PLAYER_HEIGHT;
	}
	else grounded = false;
}

void Player::jump() {
	if (grounded) {
		yVel -= 10;
		grounded = false;
	}
}

void Player::gravity() {
	if (!grounded) {
		yVel += GRAVITY;
		if (yVel > MAX_GRAVITY) yVel = MAX_GRAVITY;
	}
	else yVel = GRAVITY;
}

void Player::getHit(Skeleton& p_skeleton) {
	if ((p_skeleton.getDistance() <= TILE_WIDTH * 1.5 && p_skeleton.isAttacking() && getY() >= p_skeleton.getY() - TILE_WIDTH && getY() <= p_skeleton.getY() + TILE_WIDTH * 0.5) || getY() >= LEVEL_HEIGHT) dead = true;
}

void Player::knockBack() {
	if (beingHit) {
		yVel += -4;
		if (getFlipType() == SDL_FLIP_NONE) x += -100;
		else if (getFlipType() == SDL_FLIP_HORIZONTAL) x += 10;
	}
}

void Player::handleCamera(SDL_Rect& camera) {
	//Camera tự di chuyển theo x
	//camera.x += 3;
	//if (getX() + PLAYER_WIDTH / 2 - camera.x >= SCREEN_WIDTH * 2 / 3) {
		camera.x = (getX() + PLAYER_WIDTH / 2) - SCREEN_WIDTH * 2 / 3;
	//}
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

	if (idling) {
		commonFunc::renderAnimation(tex, x, y, idlingClips[idleCounter/6], p_camera, 0, NULL, getFlipType());
		idleCounter++;
		if (idleCounter / 6 >= IDLING_ANIMATION_FRAMES) idleCounter = 0;
	}
	else idleCounter = 0;

	if (jumping) {
		commonFunc::renderAnimation(tex, x, y, jumpingClips[jumpCounter / 5], p_camera, 0, NULL, getFlipType());
		jumpCounter++;
		if (jumpCounter / 5 >= JUMPING_ANIMATION_FRAMES) jumpCounter = 0;
	}
	else jumpCounter = 0;

	if (falling) {
		commonFunc::renderAnimation(tex, x, y, fallingClips[fallingCounter / 4], p_camera, 0, NULL, getFlipType());
		fallingCounter++;
		if (fallingCounter / 4 >= FALLING_ANIMATION_FRAMES) fallingCounter = 0;
	}
	else fallingCounter = 0;
}
