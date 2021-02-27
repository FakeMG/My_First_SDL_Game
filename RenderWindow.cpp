#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sstream>

#include "RenderWindow.h"
#include "Entity.h"
using namespace std;

RenderWindow::RenderWindow(const char* p_title, int p_width, int p_height) {
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
SDL_Texture* RenderWindow::loadTexture(const char* p_filePath) {
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, p_filePath);
	if (texture == NULL) cout << "Texture has failed to load: " << IMG_GetError() << endl;
	return texture;
}

//Copy texture vào renderer và render
void RenderWindow::renderTexture(Entity& entity, SDL_Rect* rec, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	float cameraX = 0, cameraY = 0;
	//mặc định là in toàn bộ texture ra
	SDL_Rect dst = { entity.getX() - cameraX, entity.getY() - cameraY, entity.getCurrentFrame().w, entity.getCurrentFrame().h };
	
	//nếu đầu vào rec != NULL thì in ra theo rec
	if (rec != NULL) {
		dst.w = rec->w;
		dst.h = rec->h;
	}
	SDL_RenderCopyEx(renderer, entity.getTex(), rec, &dst, angle, center, flip);
}

void RenderWindow::renderTile(Entity& entity, SDL_Rect& rec, SDL_Rect& camera) {
	SDL_Rect dst = { entity.getX() - camera.x, entity.getY() - camera.y, rec.w, rec.h };
	SDL_RenderCopy(renderer, entity.getTex(), &rec, &dst);
}

void RenderWindow::renderAnimation(SDL_Texture* p_tex, int p_w, int p_h, float p_x, float p_y) {
	SDL_Rect dst = { 0,0,p_w,p_h };
	SDL_Rect src = { p_x,p_y,p_w,p_h };
	SDL_RenderCopy(renderer, p_tex, &src, &dst);
}

void RenderWindow::renderPlayer(Entity& player, SDL_Rect& camera) {
	SDL_Rect src = { player.getCurrentFrame().x, player.getCurrentFrame().y, player.getCurrentFrame().w, player.getCurrentFrame().h };
	SDL_Rect dst = { player.getX() - camera.x, player.getY() - camera.y, player.getCurrentFrame().w, player.getCurrentFrame().h };
	SDL_RenderCopy(renderer, player.getTex(), &src, &dst);
}

//Load font
void RenderWindow::loadFont(const char* filePath) {
	TTF_CloseFont(font);
	font = TTF_OpenFont(filePath, 28);
	if (font == NULL) cout << "Font has failed to load: " << TTF_GetError() << endl;
}

//Create text
SDL_Texture* RenderWindow::createText(string p_text, SDL_Color p_textColor) {
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
void RenderWindow::Color() {
	surface = SDL_GetWindowSurface(window);
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 0, 0));
	SDL_UpdateWindowSurface(window);
}


//clear renderer cũ
void RenderWindow::clearRenderer() {
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
}

void RenderWindow::renderPresent() {
	SDL_RenderPresent(renderer);
}

//Giải phóng bộ nhớ
void RenderWindow::cleanUp() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

