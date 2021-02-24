#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sstream>

#include "RenderWindow.h"
#include "Entity.h"
#include "Player.h"
#include "Timer.h"
using namespace std;

RenderWindow window("Test Game", SCREEN_WIDTH, SCREEN_HEIGHT);

//FPScounter
LTimer fpsTimer;
LTimer capTimer;
stringstream timeText;
int countedFrames = 0;
void FPSCounter() {
    float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
    if (avgFPS > 2000000) {
        avgFPS = 0;
    }
    timeText.str("");
    timeText << "FPS: " << avgFPS;

    SDL_Color textColor = { 0,0,0,255 };
    SDL_Texture* textTex = window.createText(timeText.str().c_str(), textColor);
    Entity text(0, 0, textTex);
    window.renderTexture(text);
    ++countedFrames;
}

//Khởi tạo SDL
bool init() {
    bool success = true;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cout << "SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << endl;
        success = false;
    }
    if (!(IMG_Init(IMG_INIT_PNG))) {
        cout << "IMG_Init HAS FAILED. SDL_ERROR: " << IMG_GetError() << endl;
        success = false;
    }
    if (!(IMG_Init(IMG_INIT_JPG))) {
        cout << "IMG_Init HAS FAILED. SDL_ERROR: " << IMG_GetError << endl;
        success = false;
    }
    if (TTF_Init() == -1) {
        cout << "TTF_Init HAS FAILED. SDL_ERROR: " << TTF_GetError() << endl;
        success = false;
    }
    return success;
}

int main(int argc, char* argv[]) {
    if ( !init() ) {
        printf("Failed to initialize!\n");
        return 0;
    }
    window.loadFont("res/lazy.ttf");

    SDL_Texture* bgTex = window.loadTexture("res/gtx/Doge.jpg");
    SDL_Texture *FakeMG = window.loadTexture("res/gtx/foo.png");
    Player knight(0, 0, FakeMG);
    Entity bg(0, 0, bgTex);
 
    fpsTimer.start();
    
    

    //Game loop
    bool gameRunning = true;
    SDL_Event event;
    while (gameRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) gameRunning = false;
            knight.handleInput(event);
        }
        knight.update();
        SDL_Rect camera = knight.getCamera();
        knight.handleCamera();

        window.clearRenderer();
        window.renderTexture(bg, &camera);
        window.renderPlayer(knight, camera.x, camera.y);
        FPSCounter();
        window.renderPresent();
    }

    //Giải phóng bộ nhớ
    window.cleanUp();
    return 0;
}

