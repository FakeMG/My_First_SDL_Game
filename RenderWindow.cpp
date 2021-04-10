#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sstream>

#include "RenderWindow.h"
#include "Entity.h"
using namespace std;

void commonFunc::renderWindow(const char* p_title, int p_width, int p_height) {
	//Khởi tạo cửa sổ
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, p_width, p_height, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		cout << "Window has failed to init: " << SDL_GetError << endl;
	}
	//Tạo renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		cout << "Renderer has failed to init: " << SDL_GetError() << endl;
	}
}

//Load texture
SDL_Texture* commonFunc::loadTexture(const char* p_filePath) {
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, p_filePath);
	if (texture == NULL) cout << "Texture has failed to load: " << IMG_GetError() << endl;
	return texture;
}

//Copy texture vào renderer và render
void commonFunc::renderTexture(Entity& entity, SDL_Rect *rec, SDL_Rect *camera, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	//mặc định là in toàn bộ texture ra
	SDL_Rect dst = { entity.getX(), entity.getY(), entity.getCurrentFrame().w, entity.getCurrentFrame().h };
	
	//nếu đầu vào rec != NULL thì in ra theo rec
	if (rec != NULL) {
		dst.w = rec->w;
		dst.h = rec->h;
	}
	if (camera != NULL) {
		dst.x = dst.x - camera->x;
		dst.y = dst.y - camera->y;
	}
	SDL_RenderCopyEx(renderer, entity.getTex(), rec, &dst, angle, center, flip);
}

void commonFunc::renderTile(Entity& entity, SDL_Rect& rec, SDL_Rect& camera) {
	SDL_Rect dst = { entity.getX() - camera.x, entity.getY() - camera.y, rec.w, rec.h };
	SDL_RenderCopy(renderer, entity.getTex(), &rec, &dst);
}

void commonFunc::renderAnimation(SDL_Texture* p_tex, float p_x, float p_y, SDL_Rect &p_clip, SDL_Rect &p_camera, double p_angle, SDL_Point* p_center, SDL_RendererFlip p_flip) {
	SDL_Rect src = { p_clip.x, p_clip.y, p_clip.w, p_clip.h };
	SDL_Rect dst = { p_x - p_camera.x, p_y - p_camera.y, p_clip.w, p_clip.h};
	SDL_RenderCopyEx(renderer, p_tex, &p_clip, &dst, p_angle, p_center, p_flip);
}

void commonFunc::renderPlayer(Entity& entity, SDL_Rect &camera) {
	SDL_Rect src = { 0, 0, entity.getCurrentFrame().w, entity.getCurrentFrame().h };
	SDL_Rect dst = { entity.getX() - camera.x, entity.getY() - camera.y, entity.getCurrentFrame().w, entity.getCurrentFrame().h };
	SDL_RenderCopy(renderer, entity.getTex(), &src, &dst);
}

bool commonFunc::checkCollision(SDL_Rect a, SDL_Rect b) {
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	if (bottomA < topB) {
		return false;
	}

	if (topA > bottomB) {
		return false;
	}

	if (rightA < leftB) {
		return false;
	}

	if (leftA > rightB) {
		return false;
	}

	return true;
}

bool commonFunc::touchesWall(SDL_Rect box, Tile* tiles[]) {
	for (int i = 0; i < TOTAL_TILES; ++i) {
		if ((tiles[i]->getType() >= 0) && (tiles[i]->getType() <= 84)) {
			if (checkCollision(box, tiles[i]->getCollision())) {
				return true;
			}
		}
	}
	return false;
}

bool commonFunc::touchesWall(SDL_Rect box, Tile* tiles[], int &stt) {
	for (int i = 0; i < TOTAL_TILES; ++i) {
		if ((tiles[i]->getType() >= 0) && (tiles[i]->getType() <= 84)) {
			if (checkCollision(box, tiles[i]->getCollision())) {
				stt = i;
				return true;
			}
		}
	}
	return false;
}

//Load font
bool commonFunc::loadFont(const char* filePath) {
	TTF_CloseFont(font);
	font = TTF_OpenFont(filePath, 28);
	if (font == NULL) {
		printf ("Font has failed to load: %s\n",TTF_GetError());
		return false;
	}
	return true;
}

//Create text
SDL_Texture* commonFunc::createText(string p_text, SDL_Color p_textColor) {
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, p_text.c_str(), p_textColor);
	SDL_Texture* texture = NULL;
	if (textSurface == NULL) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else {
		//Tạo texture text từ surface
		texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (texture == NULL) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		SDL_FreeSurface(textSurface);
		return texture;
	}
	return NULL;
}

//Tô màu
void commonFunc::Color() {
	surface = SDL_GetWindowSurface(window);
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 0, 0));
	SDL_UpdateWindowSurface(window);
}


//clear renderer cũ
void commonFunc::clearRenderer() {
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
}

void commonFunc::renderPresent() {
	SDL_RenderPresent(renderer);
}

//Giải phóng bộ nhớ
void commonFunc::cleanUp() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

