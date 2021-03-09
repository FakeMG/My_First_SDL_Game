#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sstream>
#include <fstream>

#include "RenderWindow.h"
#include "Entity.h"
#include "Player.h"
#include "Timer.h"
#include "Tile.h"
using namespace std;

SDL_Event event;

SDL_Texture* bgTex = NULL;
SDL_Texture* FakeMG = NULL;
SDL_Texture* tileTex = NULL;
SDL_Texture* bulletTex = NULL;

SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

//FPScounter
LTimer fpsTimer;
stringstream timeText;
int countedFrames = 0;

SDL_Rect gTileClips[TOTAL_TILE_SPRITES];
Tile* tileSet[TOTAL_TILES];

bool gameRunning = true;

bool init();
bool loadMedia();
void FPSCounter();
bool setTiles(Tile* tiles[]);

int main(int argc, char* argv[]) {
    if (!init()) return 0;
    else {
        if (!loadMedia()) return 0;
        else {
            Player knight(0, 0, FakeMG);
            Entity bg(0, 0, bgTex);
            if (!setTiles(tileSet)) {
                printf("Failed to load tile set!\n");
            }
            
            //Game loop
            fpsTimer.start(); //bắt đầu đếm time
            
            while (gameRunning) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) gameRunning = false;
                    knight.handleInput(event);
                }
                knight.update(tileSet);
                knight.handleCamera(camera);

                commonFunc::clearRenderer();
                for (int i = 0; i < TOTAL_TILES; i++) {
                    commonFunc::renderTile(*tileSet[i], gTileClips[tileSet[i]->getType()], camera);
                }

                for (int i = 0; i < knight.getBulletList().size(); i++) {
                    if (knight.getBulletList().at(i) != NULL) {
                        if (knight.getBulletList().at(i)->isMoving()) {
                            knight.getBulletList().at(i)->render(camera, bulletTex);
                            knight.getBulletList().at(i)->move();
                        }
                        else knight.getBulletList().erase(knight.getBulletList().begin() + i);
                    }
                }

                knight.render(camera);
                FPSCounter();
                commonFunc::renderPresent();
            }
        }
    }

    //Giải phóng bộ nhớ
    commonFunc::cleanUp();
    return 0;
}

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

    if (success == true) commonFunc::renderWindow("Test Game", SCREEN_WIDTH, SCREEN_HEIGHT);

    return success;
}
bool loadMedia() {
    bool success = true;
    if (!commonFunc::loadFont("res/lazy.ttf")) success = false;
    bgTex = commonFunc::loadTexture("res/gtx/Doge.jpg");
    if (bgTex == NULL) success = false;
    FakeMG = commonFunc::loadTexture("res/gtx/Metal knight.png");
    if (FakeMG == NULL) success = false;
    tileTex = commonFunc::loadTexture("res/gtx/tiles.png");
    if (tileTex == NULL) success = false;
    bulletTex = commonFunc::loadTexture("res/gtx/Bullet.png");
    return success;
}
void FPSCounter() {
    float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
    if (avgFPS > 2000000) {
        avgFPS = 0;
    }
    timeText.str("");
    timeText << "FPS: " << avgFPS;

    SDL_Color textColor = { 0,0,0,255 };
    SDL_Texture* textTex = commonFunc::createText(timeText.str().c_str(), textColor);
    Entity text(0, 0, textTex);
    commonFunc::renderTexture(text);
    ++countedFrames;
}
bool setTiles(Tile* tiles[]) {
    //Success flag
    bool tilesLoaded = true;

    //The tile offsets
    int x = 0, y = 0;

    //Mở map
    ifstream map("res/gtx/lazy.map");

    //Nếu ko đọc đc dữ liệu trong map
    if (map.fail()) {
        printf("Unable to load map file!\n");
        tilesLoaded = false;
    }
    else
    {
        //Tạo các tile
        for (int i = 0; i < TOTAL_TILES; ++i) {
            //Chọn loại cho tile
            int tileType = -1;

            //Đọc tu map
            map >> tileType;

            //Debug
            if (map.fail()) {
                printf("Error loading map: Unexpected end of file!\n");
                tilesLoaded = false;
                break;
            }

            //Nếu như đọc đc tileType và thỏa mãn
            if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES)) {
                tiles[i] = new Tile(x, y, tileTex, tileType);
            }
            //Nếu như tileType ko thoả mãn
            else
            {
                //Dừng load
                printf("Error loading map: Invalid tile type at %d!\n", i);
                tilesLoaded = false;
                break;
            }
            //Dịch x của tile tiếp theo
            x += TILE_WIDTH;

            //Nếu như đến giới hạn level
            if (x >= LEVEL_WIDTH) {
                //Xuống dòng mới và làm lại
                x = 0;
                y += TILE_HEIGHT;
            }
        }

        if (tilesLoaded)
        {
            gTileClips[TILE_RED].x = 0;
            gTileClips[TILE_RED].y = 0;
            gTileClips[TILE_RED].w = TILE_WIDTH;
            gTileClips[TILE_RED].h = TILE_HEIGHT;

            gTileClips[TILE_GREEN].x = 0;
            gTileClips[TILE_GREEN].y = 80;
            gTileClips[TILE_GREEN].w = TILE_WIDTH;
            gTileClips[TILE_GREEN].h = TILE_HEIGHT;

            gTileClips[TILE_BLUE].x = 0;
            gTileClips[TILE_BLUE].y = 160;
            gTileClips[TILE_BLUE].w = TILE_WIDTH;
            gTileClips[TILE_BLUE].h = TILE_HEIGHT;

            gTileClips[TILE_TOPLEFT].x = 80;
            gTileClips[TILE_TOPLEFT].y = 0;
            gTileClips[TILE_TOPLEFT].w = TILE_WIDTH;
            gTileClips[TILE_TOPLEFT].h = TILE_HEIGHT;

            gTileClips[TILE_LEFT].x = 80;
            gTileClips[TILE_LEFT].y = 80;
            gTileClips[TILE_LEFT].w = TILE_WIDTH;
            gTileClips[TILE_LEFT].h = TILE_HEIGHT;

            gTileClips[TILE_BOTTOMLEFT].x = 80;
            gTileClips[TILE_BOTTOMLEFT].y = 160;
            gTileClips[TILE_BOTTOMLEFT].w = TILE_WIDTH;
            gTileClips[TILE_BOTTOMLEFT].h = TILE_HEIGHT;

            gTileClips[TILE_TOP].x = 160;
            gTileClips[TILE_TOP].y = 0;
            gTileClips[TILE_TOP].w = TILE_WIDTH;
            gTileClips[TILE_TOP].h = TILE_HEIGHT;

            gTileClips[TILE_CENTER].x = 160;
            gTileClips[TILE_CENTER].y = 80;
            gTileClips[TILE_CENTER].w = TILE_WIDTH;
            gTileClips[TILE_CENTER].h = TILE_HEIGHT;

            gTileClips[TILE_BOTTOM].x = 160;
            gTileClips[TILE_BOTTOM].y = 160;
            gTileClips[TILE_BOTTOM].w = TILE_WIDTH;
            gTileClips[TILE_BOTTOM].h = TILE_HEIGHT;

            gTileClips[TILE_TOPRIGHT].x = 240;
            gTileClips[TILE_TOPRIGHT].y = 0;
            gTileClips[TILE_TOPRIGHT].w = TILE_WIDTH;
            gTileClips[TILE_TOPRIGHT].h = TILE_HEIGHT;

            gTileClips[TILE_RIGHT].x = 240;
            gTileClips[TILE_RIGHT].y = 80;
            gTileClips[TILE_RIGHT].w = TILE_WIDTH;
            gTileClips[TILE_RIGHT].h = TILE_HEIGHT;

            gTileClips[TILE_BOTTOMRIGHT].x = 240;
            gTileClips[TILE_BOTTOMRIGHT].y = 160;
            gTileClips[TILE_BOTTOMRIGHT].w = TILE_WIDTH;
            gTileClips[TILE_BOTTOMRIGHT].h = TILE_HEIGHT;
        }
    }

    //Close the file
    map.close();

    //If the map was loaded fine
    return tilesLoaded;
}