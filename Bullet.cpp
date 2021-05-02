#include "Bullet.h"

Bullet::Bullet(float p_x, float p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {
	collision.x = x;
	collision.y = y;
	for (int i = 0; i < BULLET_ANIMATION_FRAMES; i++) {
		bulletClips[i].x = i * DEFAULTBULLET_W;
		bulletClips[i].y = 0;
		bulletClips[i].w = DEFAULTBULLET_W;
		bulletClips[i].h = DEFAULTBULLET_H;
	}
}

void Bullet::update(vector<LevelPart>& LevelPartList) {
	if (bulletCounter/delay < 3) {
		xVel += BULLETSPEED;

		if (getFlipType() == SDL_FLIP_HORIZONTAL) x -= xVel;
		else if (getFlipType() == SDL_FLIP_NONE) x += xVel;

		collision.x = getX();

		if (getX()< 0) {
			x = 0;
			collision.x = getX();
		}
		/*if (getX() + DEFAULTBULLET_W > LEVEL_WIDTH) {
			x = LEVEL_WIDTH - DEFAULTBULLET_W;
			collision.x = getX();
		}*/
		if (commonFunc::touchesWall(collision, LevelPartList)) {
			if (getFlipType() == SDL_FLIP_HORIZONTAL) x += xVel;
			else if (getFlipType() == SDL_FLIP_NONE) x -= xVel;
			collision.x = getX();
		}
	}
	if (bulletCounter/delay == BULLET_ANIMATION_FRAMES) setMove(false);
}

void Bullet::setFlipType(SDL_RendererFlip p_PlayerflipType) {
	flipType = p_PlayerflipType;
	 
}

void Bullet::setSize_Position(const int& p_width, const int& p_height, const int& p_playerX) {
	if (getFlipType() == SDL_FLIP_HORIZONTAL) {
		x = p_playerX - (DEFAULTBULLET_W / 4);
	}
	collision.w = DEFAULTBULLET_W;
	collision.h = DEFAULTBULLET_H-2;
}

void Bullet::render(SDL_Rect &camera, SDL_Texture* p_tex) {
	commonFunc::renderAnimation(p_tex, getX(), getY(), bulletClips[bulletCounter/ delay], camera, 0, NULL, getFlipType());
	bulletCounter++;
}