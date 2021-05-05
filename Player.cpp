#include "Player.h"

Player::Player(float p_x, float p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {
	collision.x = getX() + PLAYER_WIDTH;
	collision.y = getY() + PLAYER_HEIGHT;
	collision.w = PLAYER_WIDTH-12; //cho vừa với chân nhân vật
	collision.h = PLAYER_HEIGHT;


	for (int i = 0; i < WALKING_ANIMATION_FRAMES; i++) {
		walkingClips[i].x = i * (getCurrentFrame().w/4);
		if (i >= 4) {
			walkingClips[i].x = (i-4) * (getCurrentFrame().w/4);
			walkingClips[i].y = (getCurrentFrame().h/6)*2;
		}
		else walkingClips[i].y = getCurrentFrame().h/6;
		walkingClips[i].w = getCurrentFrame().w/4;
		walkingClips[i].h = getCurrentFrame().h/6;
	}
	for (int i = 0; i < IDLING_ANIMATION_FRAMES; i++) {
		idlingClips[i].x = i * (getCurrentFrame().w/4);
		idlingClips[i].y = 0;
		idlingClips[i].w = getCurrentFrame().w/4;
		idlingClips[i].h = getCurrentFrame().h/6;
	}
	for (int i = 0; i < JUMPING_ANIMATION_FRAMES; i++) {
		jumpingClips[i].x = i * (getCurrentFrame().w / 4);
		jumpingClips[i].y = (getCurrentFrame().h/6) * 3;
		jumpingClips[i].w = getCurrentFrame().w / 4;
		jumpingClips[i].h = getCurrentFrame().h / 6;
	}
	for (int i = 0; i < FALLING_ANIMATION_FRAMES; i++) {
		fallingClips[i].x = i * (getCurrentFrame().w / 4);
		fallingClips[i].y = (getCurrentFrame().h / 6) * 4;
		fallingClips[i].w = getCurrentFrame().w / 4;
		fallingClips[i].h = getCurrentFrame().h / 6;
	}
	for (int i = 0; i < DEATH_ANIMATION_FRAMES; i++) {
		deathClips[i].x = i * (getCurrentFrame().w / 4);
		deathClips[i].y = (getCurrentFrame().h / 6) * 5;
		deathClips[i].w = getCurrentFrame().w / 4;
		deathClips[i].h = getCurrentFrame().h / 6;
	}
}

void Player::handleInput(SDL_Event &events, Mix_Chunk* p_sfx[]) {
	if (!isDead()) {
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
					Mix_PlayChannel(-1, p_sfx[jumpSFX], 0);
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
			if (events.button.button == SDL_BUTTON_LEFT) {
				Bullet* bullet = new Bullet(getCollision().x + PLAYER_WIDTH * 1.25, getCollision().y, NULL);
				Mix_PlayChannel(-1, p_sfx[shootSFX], 0);
				bullet->setFlipType(getFlipType());
				bullet->setSize_Position(DEFAULTBULLET_W, DEFAULTBULLET_H, getX());
				bullet->setType(Bullet::NORMAL);
				bullet->setMove(true);
				bulletList.push_back(bullet);
			}
			
		}
		else if (events.type == SDL_MOUSEBUTTONUP && events.key.repeat == 0) {
			//chua biet lam gi
		}
	}
}

void Player::update(vector<LevelPart>& LevelPartList, vector<Skeleton*> &skeletonList, Mix_Chunk* p_sfx[], SDL_Rect& camera) {
	gravity();
	if(!dead) getHit(skeletonList, p_sfx, camera);

	// set trạng thái Player
	if (xVel == 0 && grounded && !dead) idling = true;
	else idling = false;

	if (xVel != 0 && grounded && !dead) running = true;
	else running = false;

	if (yVel > 0 && !grounded && !dead) falling = true;
	else falling = false;

	if (yVel <= 0 && !grounded && !dead) jumping = true;
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
		if (commonFunc::touchesWall(collision, LevelPartList)) {
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
	if (commonFunc::touchesWall(collision, LevelPartList, grounded, groundSTT, levelSTT)) {
		if (yVel > 0) {
			y = LevelPartList.at(levelSTT).getTilesList().at(groundSTT)->getY() - 64 * 2;
			if (falling) {
				grounded = true;
				Mix_PlayChannel(-1, p_sfx[landSFX], 0);
			}
		}
		else if (yVel < 0) {
			y -= yVel;
			yVel = 0;
		}
		collision.y = getY() + PLAYER_HEIGHT;
	}

	
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

void Player::getHit(vector<Skeleton*> &skeletonList, Mix_Chunk* p_sfx[], SDL_Rect& camera) {
	for (int i = 0; i < skeletonList.size(); i++) {
		if(skeletonList.at(i) != NULL)
			if ((skeletonList.at(i)->getDistance() <= TILE_WIDTH * 1.5 && skeletonList.at(i)->isAttacking() && getY() >= skeletonList.at(i)->getY() - TILE_WIDTH && getY() <= skeletonList.at(i)->getY() + TILE_WIDTH * 0.5)) {
				dead = true;
				Mix_PlayChannel(-1, p_sfx[hitSFX], 0);
			}
	}
	if (getY() + PLAYER_HEIGHT >= LEVEL_HEIGHT || getX() - camera.x <= 192 - 2*64) {
		dead = true;
		Mix_PlayChannel(-1, p_sfx[hitSFX], 0);
	}
}

void Player::knockBack() {
	if (beingHit) {
		yVel += -4;
		if (getFlipType() == SDL_FLIP_NONE) x += -100;
		else if (getFlipType() == SDL_FLIP_HORIZONTAL) x += 10;
	}
}

void Player::handleCamera(SDL_Rect& camera, float& camVel) {
	//Camera tự di chuyển theo x
	if(!isDead()) camera.x += camVel;
	float acc = 0.001;
	if (camVel > 4) acc = 0.0003;
	if (camVel > 5) acc = 0.00001;
	camVel += acc;

	if (getX() + PLAYER_WIDTH / 2 - camera.x >= SCREEN_WIDTH * 2 / 3) {
		camera.x = (getX() + PLAYER_WIDTH / 2) - SCREEN_WIDTH * 2 / 3;
	}
	camera.y = (getY() + PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

	//Giới hạn 
	if (camera.x < 0) {
		camera.x = 0;
	}
	if (camera.y < 0) {
		camera.y = 0;
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

	if (dead) {
		commonFunc::renderAnimation(tex, x, y, deathClips[deathCounter / 5], p_camera, 0, NULL, getFlipType());
		if(deathCounter / 5 < DEATH_ANIMATION_FRAMES-1) deathCounter++;
	}
	else deathCounter = 0;
}
