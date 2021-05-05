#include "RenderWindow.h"
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

void commonFunc::renderTexture(SDL_Texture* p_tex, float p_x, float p_y, float p_w, float p_h, SDL_Rect* rec, SDL_Rect* camera, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	//mặc định là in toàn bộ texture ra
	SDL_Rect dst = { p_x, p_y, p_w, p_h };

	//nếu đầu vào rec != NULL thì in ra theo rec
	if (rec != NULL) {
		dst.w = rec->w;
		dst.h = rec->h;
	}
	if (camera != NULL) {
		dst.x = dst.x - camera->x;
		dst.y = dst.y - camera->y;
	}
	SDL_RenderCopyEx(renderer, p_tex, rec, &dst, angle, center, flip);
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

	if (bottomA <= topB) {
		return false;
	}

	if (topA >= bottomB) {
		return false;
	}

	if (rightA <= leftB) {
		return false;
	}

	if (leftA >= rightB) {
		return false;
	}

	return true;
}

//bool commonFunc::touchesWall(SDL_Rect& box, vector<LevelPart>& LevelPartList) {
//	for (int i = 0; i < LevelPartList.size(); i++) {
//		for (int j = 0; j < LevelPartList.at(i).getTilesList().size(); ++j) {
//			if ((LevelPartList.at(i).getTilesList().at(j)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(j)->getType() <= 84)) {
//				if (checkCollision(box, LevelPartList.at(i).getTilesList().at(j)->getCollision())) {
//					return true;
//				}
//			}
//		}
//	}
//	return false;
//}

bool commonFunc::touchesWall(SDL_Rect& box, vector<LevelPart>& LevelPartList) {
	for (int i = 0; i < LevelPartList.size(); i++) {
		if (box.x > LevelPartList.at(i).getX() && box.x + box.w + 13 < LevelPartList.at(i).getX() + LEVEL_WIDTH && box.y >= 0 && box.y < LEVEL_HEIGHT - TILE_HEIGHT) {
			int cot_left = (box.x - LevelPartList.at(i).getX()) / TILE_WIDTH;
			int cot_rigth = cot_left + 1;
			int dong_up = box.y / TILE_HEIGHT;
			int dong_down = dong_up + 1;

			int stt1 = dong_up * 21 + cot_rigth;
			int stt2 = dong_down * 21 + cot_rigth;
			int stt3 = dong_up * 21 + cot_left;
			int stt4 = dong_down * 21 + cot_left;

			if ((LevelPartList.at(i).getTilesList().at(stt1)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(stt1)->getType() <= 84))
				if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt1)->getCollision())) return true;

			if ((LevelPartList.at(i).getTilesList().at(stt2)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(stt2)->getType() <= 84))
				if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt2)->getCollision())) return true;


			if ((LevelPartList.at(i).getTilesList().at(stt3)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(stt3)->getType() <= 84))
				if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt3)->getCollision())) return true;

			if ((LevelPartList.at(i).getTilesList().at(stt4)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(stt4)->getType() <= 84))
				if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt4)->getCollision())) return true;
		}
	}
	return false;
}

bool commonFunc::touchesWall(SDL_Rect& box, vector<LevelPart>& LevelPartList, bool& grounded, int& groundSTT, int& levelSTT) {
	bool check = false;
	for (int i = 0; i < LevelPartList.size(); i++) {
		if (box.x + box.w + 12 >= LevelPartList.at(i).getX() && box.x <= LevelPartList.at(i).getX() + LEVEL_WIDTH && box.y >= 0 && box.y < LEVEL_HEIGHT - TILE_HEIGHT) {
			int cot_left = (box.x - LevelPartList.at(i).getX()) / TILE_WIDTH;
			int cot_rigth = cot_left + 1;
			int dong_up = box.y / TILE_HEIGHT;
			int dong_down = dong_up + 1;

			int stt1 = dong_up * 21 + cot_rigth;
			int stt2 = dong_down * 21 + cot_rigth;
			int stt3 = dong_up * 21 + cot_left;
			int stt4 = dong_down * 21 + cot_left;
			
			if (box.x <= LevelPartList.at(i).getX() && box.x + box.w + 12 >= LevelPartList.at(i).getX() || box.x <= LevelPartList.at(i).getX() + LEVEL_WIDTH && box.x + box.w + 12 >= LevelPartList.at(i).getX() + LEVEL_WIDTH) {
				grounded = false;
			}
			else {
				if ((LevelPartList.at(i).getTilesList().at(stt1)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(stt1)->getType() <= 84))
					if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt1)->getCollision())) check = true;

				if ((LevelPartList.at(i).getTilesList().at(stt2)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(stt2)->getType() <= 84))
					if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt2)->getCollision())) check = true;


				if ((LevelPartList.at(i).getTilesList().at(stt3)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(stt3)->getType() <= 84))
					if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt3)->getCollision())) check = true;

				if ((LevelPartList.at(i).getTilesList().at(stt4)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(stt4)->getType() <= 84))
					if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt4)->getCollision())) check = true;

				if ((LevelPartList.at(i).getTilesList().at(stt2)->getType() > 84) && (LevelPartList.at(i).getTilesList().at(stt4)->getType() > 84)) grounded = false;
				if ((LevelPartList.at(i).getTilesList().at(stt4)->getType() > 84) && (LevelPartList.at(i).getTilesList().at(stt2)->getType() <= 84) && box.x + box.w <= LevelPartList.at(i).getTilesList().at(stt2)->getX()) grounded = false;
			}
				
				groundSTT = stt4;
				levelSTT = i;
		}
	}
	return check;
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


void commonFunc::clearRenderer() {
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
}

void commonFunc::renderPresent() {
	SDL_RenderPresent(renderer);
}

void commonFunc::cleanUp() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = NULL;
	renderer = NULL;

	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

