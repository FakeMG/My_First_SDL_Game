#include "Tile.h"

Tile::Tile(float p_x, float p_y, SDL_Texture* p_tex, int p_type) : Entity(p_x, p_y, p_tex) {
	collision.x = p_x;
	collision.y = p_y;
	collision.w = TILE_WIDTH;
	collision.h = TILE_HEIGHT;

	tileType = p_type;
}

int Tile::getType() {
	return tileType;
}

SDL_Rect Tile::getCollision() {
	return collision;
}

void Tile::setX(int p_x) {
	 x = p_x;
	 collision.x = getX();
}