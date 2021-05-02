#include "Menu.h"

Menu::Menu(SDL_Texture* buttonTex, SDL_Texture* mainMenuBGTex, SDL_Texture* retryBGTex) {
	for (int i = 0; i < TYPE_OF_BUTTON; i++) {
		playButtonClips[i].x = i * BUTTON_WIDTH;
		playButtonClips[i].y = 0;
		playButtonClips[i].w = BUTTON_WIDTH;
		playButtonClips[i].h = BUTTON_HEIGHT;

		exitButtonClips[i].x = i * BUTTON_WIDTH;
		exitButtonClips[i].y = BUTTON_HEIGHT;
		exitButtonClips[i].w = BUTTON_WIDTH;
		exitButtonClips[i].h = BUTTON_HEIGHT;

		retryButtonClips[i].x = i * BUTTON_WIDTH;
		retryButtonClips[i].y = BUTTON_HEIGHT * 2;
		retryButtonClips[i].w = BUTTON_WIDTH;
		retryButtonClips[i].h = BUTTON_HEIGHT;
	}
	this->buttonTex = buttonTex;
	this->mainMenuBGTex = mainMenuBGTex;
	this->retryBGTex = retryBGTex;
}

void Menu::handleInput(SDL_Event& event, bool& p_gameRunning, Player& p_player, vector<Skeleton*>& p_skeletonList, vector<LevelPart>& LevelPartList, float& p_camVel, SDL_Rect& p_cam) {
	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT) {
			if (isMenu()) {
				if (checkMouseHover(button1.x, button1.y)) {
					pressed[0] = true;
					menu = false;
				}
				if (checkMouseHover(button2.x, button2.y)) {
					pressed[1] = true;
					p_gameRunning = false;
				}
			}
			if (p_player.isDead()) {
				if (checkMouseHover(button1.x, button1.y)) {
					pressed[2] = true;
					resetGame(p_player, p_skeletonList, LevelPartList, p_camVel, p_cam);
				}
				if (checkMouseHover(button2.x, button2.y)) {
					pressed[3] = true;
					p_gameRunning = false;
				}
			}
		}
		break;
	case SDL_MOUSEBUTTONUP:
		for (int i = 0; i < 4; i++) pressed[i] = false;
		break;
	case SDL_MOUSEMOTION:
		if (isMenu()) {
			if (checkMouseHover(button1.x, button1.y) && !pressed[0]) selected[0] = true;
			else selected[0] = false;
			if (checkMouseHover(button2.x, button2.y) && !pressed[1]) selected[1] = true;
			else selected[1] = false;
		}
		if (p_player.isDead()) {
			if (checkMouseHover(button1.x, button1.y) && !pressed[2]) selected[2] = true;
			else selected[2] = false;
			if (checkMouseHover(button2.x, button2.y) && !pressed[3]) selected[3] = true;
			else selected[3] = false;
		}
		break;
	default:
		break;
	}
}

void Menu::renderMainMenu() {
	if (isMenu()) {
		commonFunc::renderTexture(mainMenuBGTex, 0, 0, 1280, 720);

		if (selected[0]) commonFunc::renderTexture(buttonTex, button1.x, button1.y, 0, 0, &playButtonClips[1]);
		else if (!pressed[0]) commonFunc::renderTexture(buttonTex, button1.x, button1.y, 0, 0, &playButtonClips[0]);
		else {
			commonFunc::renderTexture(buttonTex, button1.x, button1.y, 0, 0, &playButtonClips[2]);
			
		}

		if (selected[1]) commonFunc::renderTexture(buttonTex, button2.x, button2.y, 0, 0, &exitButtonClips[1]);
		else if (!pressed[1]) commonFunc::renderTexture(buttonTex, button2.x, button2.y, 0, 0, &exitButtonClips[0]);
		else {
			commonFunc::renderTexture(buttonTex, button2.x, button2.y, 0, 0, &exitButtonClips[2]);
			
		}
	}
}

void Menu::renderRetryMenu() {
	commonFunc::renderTexture(retryBGTex, 0, 0, 1280, 720);

	if (selected[2]) commonFunc::renderTexture(buttonTex, button1.x, button1.y, 0, 0, &retryButtonClips[1]);
	else if (!pressed[2]) commonFunc::renderTexture(buttonTex, button1.x, button1.y, 0, 0, &retryButtonClips[0]);
	else {
		commonFunc::renderTexture(buttonTex, button1.x, button1.y, 0, 0, &retryButtonClips[2]);
		
	}

	if (selected[3]) commonFunc::renderTexture(buttonTex, button2.x, button2.y, 0, 0, &exitButtonClips[1]);
	else if (!pressed[3]) commonFunc::renderTexture(buttonTex, button2.x, button2.y, 0, 0, &exitButtonClips[0]);
	else {
		commonFunc::renderTexture(buttonTex, button2.x, button2.y, 0, 0, &exitButtonClips[2]);
		
	}
}

void Menu::resetGame(Player& p_player, vector<Skeleton*>& p_skeletonList, vector<LevelPart>& LevelPartList, float& p_camVel, SDL_Rect& p_cam) {
	p_player.resetPlayer();
	p_cam.x = 0;
	p_cam.y = 0;
	p_camVel = 1.5;
	for (int i = p_skeletonList.size() - 1; i >= 0; i--) {
		delete p_skeletonList.at(i);
		p_skeletonList.at(i) = NULL;
		p_skeletonList.erase(p_skeletonList.begin() + i);
	}

}

bool Menu::checkMouseHover(const int p_x, const int p_y) {
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (x >= p_x && y >= p_y && x <= p_x + BUTTON_WIDTH && y <= p_y + BUTTON_HEIGHT) return true;
	return false;
}