#include "Skeleton.h"

Skeleton::Skeleton(float p_x, float p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {
	collision.x = getX() + SKELETON_WIDTH;
	collision.y = getY() + SKELETON_HEIGHT;
	collision.w = SKELETON_WIDTH-12;
	collision.h = SKELETON_HEIGHT-2;

	for (int i = 0; i < WALKING_ANIMATION_FRAMES; i++) {
		walkingClips[i].x = i * (getCurrentFrame().w / 4);
		walkingClips[i].y = getCurrentFrame().h / 5;
		walkingClips[i].w = getCurrentFrame().w / 4;
		walkingClips[i].h = getCurrentFrame().h / 5;
	}
	for (int i = 0; i < IDLING_ANIMATION_FRAMES; i++) {
		idlingClips[i].x = i * (getCurrentFrame().w / 4);
		idlingClips[i].y = 0;
		idlingClips[i].w = getCurrentFrame().w / 4;
		idlingClips[i].h = getCurrentFrame().h / 5;
	}
	for (int i = 0; i < FALLING_ANIMATION_FRAMES; i++) {
		fallingClips[i].x = i * (getCurrentFrame().w / 4);
		fallingClips[i].y = (getCurrentFrame().h / 5) * 4;
		fallingClips[i].w = getCurrentFrame().w / 4;
		fallingClips[i].h = getCurrentFrame().h / 5;
	}
	for (int i = 0; i < ATTACKING_ANIMATION_FRAMES; i++) {
		attackingClips[i].x = i * (getCurrentFrame().w / 4);
		attackingClips[i].y = (getCurrentFrame().h / 5) * 3;
		attackingClips[i].w = getCurrentFrame().w / 4;
		attackingClips[i].h = getCurrentFrame().h / 5;
	}
	for (int i = 0; i < BEINGHIT_ANIMATION_FRAMES; i++) {
		beingHitClips[i].x = i * (getCurrentFrame().w / 4);
		beingHitClips[i].y = (getCurrentFrame().h / 5) * 2;
		beingHitClips[i].w = getCurrentFrame().w / 4;
		beingHitClips[i].h = getCurrentFrame().h / 5;
	}
}

void Skeleton::update(Player& p_player, vector<LevelPart>& LevelPartList, Mix_Chunk* p_sfx[], SDL_Rect& camera) {
	if (!beingHit) {
		if (xVel < 0) flipType = SDL_FLIP_HORIZONTAL;
		if (xVel > 0) flipType = SDL_FLIP_NONE;
	}
	gravity();
	getHit(p_player, p_sfx, camera);
	autoMovement(LevelPartList);
	moveToPlayer(p_player, LevelPartList);
	knockBack();
	
	//update trạng thái Skeleton
	if (xVel == 0 && grounded && !attacking && !dead && !beingHit) idling = true;
	else idling = false;

	if (xVel != 0 && grounded && !attacking && !dead && !beingHit) walking = true;
	else walking = false;

	if (yVel > 0 && !grounded && !dead && !beingHit) falling = true;
	else falling = false;

	//move x
	if (!attacking) {
		x += xVel;
		collision.x = getX() + SKELETON_WIDTH;

		if (getX() + SKELETON_WIDTH < 0) {
			x = -SKELETON_WIDTH;
			collision.x = getX() + SKELETON_WIDTH;
			xVel *= -1;
		}
		if (commonFunc::touchesWall(collision, LevelPartList)) {
			x -= xVel;
			collision.x = getX() + SKELETON_WIDTH;
			xVel *= -1;
		}
	}

	//move y
	y += yVel;
	collision.y = getY() + SKELETON_HEIGHT;
	if (getY() + SKELETON_HEIGHT < 0) {
		y = -SKELETON_HEIGHT;
		collision.y = getY() + SKELETON_HEIGHT;
	}
	if (commonFunc::touchesWall(collision, LevelPartList, grounded, groundSTT, levelSTT)) {
		if (yVel > 0) {
			y = LevelPartList.at(levelSTT).getTilesList().at(groundSTT)->getY() - 64 * 2;
			if (falling) {
				grounded = true;
			}
		}
		else if (yVel < 0) {
			y -= yVel;
			yVel = 0;
		}
		collision.y = getY() + SKELETON_HEIGHT;
	}
	
}

void Skeleton::gravity() {
	if (!grounded) {
		yVel += GRAVITY;
		if (yVel > MAX_GRAVITY) yVel = MAX_GRAVITY;
	}
	else yVel = GRAVITY;
}

void Skeleton::moveToPlayer(Player& p_player, vector<LevelPart>& LevelPartList) {
	distanceToPlayer = sqrt(pow(p_player.getX() - getX(), 2) + pow(p_player.getY() - getY(), 2));
	if (!beingHit) {
		//trong tầm nhìn của skeleton
		if ((p_player.getY() >= getY() - TILE_WIDTH && p_player.getY() <= getY() + TILE_WIDTH * 0.5) && distanceToPlayer <= TILE_WIDTH * 7) {
			if (p_player.getX() - getX() < 0) {
				if (LevelPartList.at(levelSTT).getTilesList().at(groundSTT - 1)->getType() > 84) xVel = 0;
				else xVel = -SKELETON_VEL;
			}
			else if (LevelPartList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() > 84) xVel = 0;
			else xVel = SKELETON_VEL;
		}
	}
	if ( (distanceToPlayer <= TILE_WIDTH * 1.5 || (p_player.getY() >= getY() - TILE_WIDTH * 2.5 && p_player.getY() <= getY() - 64 && distanceToPlayer <= TILE_WIDTH * 2.5)) && !dead && !beingHit && grounded) attacking = true;
	else attacking = false;
}

void Skeleton::autoMovement(vector<LevelPart>& LevelPartList) {
	if (grounded && !beingHit) {
		if (LevelPartList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() > 84 && LevelPartList.at(levelSTT).getTilesList().at(groundSTT - 2)->getType() > 84) xVel = 0;
		else if (LevelPartList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() > 84 && xVel > 0) xVel = -SKELETON_VEL * 0.5;
		else if (LevelPartList.at(levelSTT).getTilesList().at(groundSTT - 1)->getType() > 84 && xVel < 0) xVel = SKELETON_VEL * 0.5;
		else if (LevelPartList.at(levelSTT).getTilesList().at(groundSTT + 2)->getType() > 84 && LevelPartList.at(levelSTT).getTilesList().at(groundSTT - 2)->getType() > 84) xVel = 0;
		else if(getFlipType() == SDL_FLIP_NONE) xVel = SKELETON_VEL * 0.5;
		else if(getFlipType() == SDL_FLIP_HORIZONTAL) xVel = -SKELETON_VEL * 0.5;
	}
}

bool Skeleton::isAttacking() {
	if (attackingCounter / 7 >= 2) return true;
	return false;
}

void Skeleton::getHit(Player& p_player, Mix_Chunk* p_sfx[], SDL_Rect& camera) {
	for (int i = 0; i < p_player.getBulletList().size(); i++) {
		if (p_player.getBulletList().at(i) != NULL) {
			if (commonFunc::checkCollision(p_player.getBulletList().at(i)->getCollision(), getCollision())) {
				if (p_player.getBulletList().at(i)->getX() >= getX() + SKELETON_WIDTH && p_player.getBulletList().at(i)->getX() <= getX()+SKELETON_WIDTH*1.5) {
					beingHit = true;
					maxHealth--;
					p_player.getBulletList().at(i)->setMove(false);
				}
			}
		}
	}
	if (beingHit && beingHitCounter == 0) Mix_PlayChannel(-1, p_sfx[1], 0);

	if (beingHitCounter / 7 >= BEINGHIT_ANIMATION_FRAMES) {
		beingHit = false;
		beingHitCounter = 0;
	}

	if (maxHealth <= 0 || getY() + SKELETON_HEIGHT/2 > LEVEL_HEIGHT || getX() - camera.x < 192 - 2*64) {
		dead = true;
		beingHit = false;
		Mix_PlayChannel(-1, p_sfx[0], 0);
	}
}

void Skeleton::knockBack() {
	if (beingHit && beingHitCounter==0) {
		yVel = -3;
		if (getFlipType() == SDL_FLIP_NONE) xVel = -4;
		else if(getFlipType() == SDL_FLIP_HORIZONTAL ) xVel = 4;
	}
}

void Skeleton::render(SDL_Rect& p_camera) {
	if (walking) {
		commonFunc::renderAnimation(tex, x, y, walkingClips[walkCounter / 4], p_camera, 0, NULL, getFlipType());
		walkCounter++;
		if (walkCounter / 4 >= WALKING_ANIMATION_FRAMES) walkCounter = 0;
	}

	if (idling) {
		commonFunc::renderAnimation(tex, x, y, idlingClips[idleCounter / 6], p_camera, 0, NULL, getFlipType());
		idleCounter++;
		if (idleCounter / 6 >= IDLING_ANIMATION_FRAMES) idleCounter = 0;
	}
	else idleCounter = 0;

	if (falling) {
		commonFunc::renderAnimation(tex, x, y, fallingClips[fallingCounter / 4], p_camera, 0, NULL, getFlipType());
		fallingCounter++;
		if (fallingCounter / 4 >= FALLING_ANIMATION_FRAMES) fallingCounter = 0;
	}
	else fallingCounter = 0;

	if (attacking) {
		commonFunc::renderAnimation(tex, x, y, attackingClips[attackingCounter / 7], p_camera, 0, NULL, getFlipType());
		attackingCounter++;
		if (attackingCounter / 7 >= ATTACKING_ANIMATION_FRAMES) attackingCounter = 0;
	}
	else attackingCounter = 0;

	if (beingHit) {
		commonFunc::renderAnimation(tex, x, y, beingHitClips[beingHitCounter / 7], p_camera, 0, NULL, getFlipType());
		beingHitCounter++;
	}
	else beingHitCounter = 0;
}
