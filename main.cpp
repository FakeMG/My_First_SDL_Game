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
#include "Skeleton.h"
#include "Timer.h"
#include "Tile.h"
using namespace std;

SDL_Event event;

SDL_Texture* FakeMG = NULL;
SDL_Texture* skeletonTex = NULL;
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
            Player knight(64*3, LEVEL_HEIGHT - TILE_HEIGHT * 3, FakeMG);
            Skeleton test(64 * 18, LEVEL_HEIGHT - TILE_HEIGHT * 9, skeletonTex);
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

                commonFunc::clearRenderer();

                //render tile
                for (int i = 0; i < TOTAL_TILES; i++) {
                    commonFunc::renderTile(*tileSet[i], gTileClips[tileSet[i]->getType()], camera);
                }

                //render bullet
                for (int i = 0; i < knight.getBulletList().size(); i++) {
                    vector<Bullet*> bulletList = knight.getBulletList();
                    if (bulletList.at(i) != NULL) {
                        if (bulletList.at(i)->isMoving()) {
                            bulletList.at(i)->render(camera, bulletTex);
                            bulletList.at(i)->update(tileSet);
                        }
                        else {
                            delete bulletList.at(i);
                            bulletList.at(i) = NULL;
                            bulletList.erase(bulletList.begin() + i);
                            knight.setBulletList(bulletList);
                        }
                    }
                }

                //render player
                knight.update(tileSet, test);
                knight.handleCamera(camera);
                knight.render(camera);

                test.update(knight, tileSet);
                test.render(camera);

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

    FakeMG = commonFunc::loadTexture("res/gtx/Metal knight.png");
    if (FakeMG == NULL) success = false;

    tileTex = commonFunc::loadTexture("res/gtx/Tileset.png");
    if (tileTex == NULL) success = false;

    bulletTex = commonFunc::loadTexture("res/gtx/Bullet.png");
    if (bulletTex == NULL) success = false;

    skeletonTex = commonFunc::loadTexture("res/gtx/Skeleton.png");
    if (skeletonTex == NULL) success = false;

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
    ifstream map("res/gtx/map.map");

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

            //Đọc từ map
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
            int m = 0, n = 0;
            for (int i = 0; i < TOTAL_TILE_SPRITES; i++) {
                gTileClips[i].x = n;
                gTileClips[i].y = m;
                gTileClips[i].w = TILE_WIDTH;
                gTileClips[i].h = TILE_HEIGHT;
                n += TILE_WIDTH;
                if (n >= 1088) {
                    n = 0;
                    m += TILE_HEIGHT;
                }
            }
        }
    }

    //Close the file
    map.close();

    //If the map was loaded fine
    return tilesLoaded;
}