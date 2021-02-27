#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sstream>

#include "Entity.h"
using namespace std;

Entity::Entity(float p_x, float p_y, SDL_Texture* p_tex) 
	:x(p_x), y(p_y), tex(p_tex)
{
	currentFrame.x = 0;
	currentFrame.y = 0;
	SDL_QueryTexture(p_tex, NULL, NULL, &currentFrame.w, &currentFrame.h);
}

float Entity::getX() {
	return x;
}

float Entity::getY() {
	return y;
}

SDL_Texture* Entity::getTex() {
	return tex;
}

SDL_Rect Entity::getCurrentFrame() {
	return currentFrame;
}
