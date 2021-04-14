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

Mix_Music* bgMusic = NULL;

Mix_Chunk* playerSFX[4];
Mix_Chunk* skeletonSFX[2];

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
        if (!loadMedia()) cout << "FAILED TO LOAD MEDIA!" << endl;
        else {
            if (!setTiles(tileSet)) {
                cout << "FAILED TO LOAD TILE SET!" << endl;
            }
            Player knight(64 * 3, LEVEL_HEIGHT - TILE_HEIGHT * 3, FakeMG);
            vector<Skeleton*> skeletonList;
            Skeleton* ske1 = new Skeleton(TILE_WIDTH * 18, LEVEL_HEIGHT - TILE_HEIGHT * 9, skeletonTex);
            skeletonList.push_back(ske1);
            Skeleton* ske2 = new Skeleton(TILE_WIDTH * 63, LEVEL_HEIGHT - TILE_HEIGHT * 12, skeletonTex);
            skeletonList.push_back(ske2);
            Skeleton* ske3 = new Skeleton(TILE_WIDTH * 72, LEVEL_HEIGHT - TILE_HEIGHT * 7, skeletonTex);
            skeletonList.push_back(ske3);
            Skeleton* ske4 = new Skeleton(TILE_WIDTH * 82, LEVEL_HEIGHT - TILE_HEIGHT * 7, skeletonTex);
            skeletonList.push_back(ske4);
            Skeleton* ske5 = new Skeleton(TILE_WIDTH * 89, LEVEL_HEIGHT - TILE_HEIGHT * 6, skeletonTex);
            skeletonList.push_back(ske5);
            Skeleton* ske6 = new Skeleton(TILE_WIDTH * 96, LEVEL_HEIGHT - TILE_HEIGHT * 5, skeletonTex);
            skeletonList.push_back(ske6);
            Skeleton* ske7 = new Skeleton(TILE_WIDTH * 133, LEVEL_HEIGHT - TILE_HEIGHT * 12, skeletonTex);
            skeletonList.push_back(ske7);

            //Game loop
            fpsTimer.start(); //bắt đầu đếm time
            Mix_PlayMusic(bgMusic, -1);
            Mix_VolumeMusic(50);

            while (gameRunning) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) gameRunning = false;
                    knight.handleInput(event, playerSFX);
                }
                
                commonFunc::clearRenderer();

                //render tile
                for (int i = 0; i < TOTAL_TILES; i++) {
                    commonFunc::renderTile(*tileSet[i], gTileClips[tileSet[i]->getType()], camera);
                }

                //render bullet
                vector<Bullet*> bulletList = knight.getBulletList();
                for (int i = 0; i < knight.getBulletList().size(); i++) {
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
                knight.update(tileSet, skeletonList, playerSFX);
                knight.handleCamera(camera);
                knight.render(camera);

                //render skeleton
                for (int i = 0; i < skeletonList.size(); i++) {
                    if (skeletonList.at(i) != NULL) {
                        if (!skeletonList.at(i)->isDead()) {
                            skeletonList.at(i)->render(camera);
                            skeletonList.at(i)->update(knight, tileSet, skeletonSFX);
                        }
                        else {
                            delete skeletonList.at(i);
                            skeletonList.at(i) = NULL;
                            skeletonList.erase(skeletonList.begin() + i);
                        }
                    }
                }

                FPSCounter();
                commonFunc::renderPresent();
            }
        }
    }

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
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
        success = false;
    }

    if (success == true) commonFunc::renderWindow("Escape the Dungeon", SCREEN_WIDTH, SCREEN_HEIGHT);

    return success;
}
bool loadMedia() {
    bool success = true;
    if (!commonFunc::loadFont("res/lazy.ttf")) success = false;

    FakeMG = commonFunc::loadTexture("res/texture/Metal knight.png");
    if (FakeMG == NULL) success = false;

    tileTex = commonFunc::loadTexture("res/texture/Tileset.png");
    if (tileTex == NULL) success = false;

    bulletTex = commonFunc::loadTexture("res/texture/Bullet.png");
    if (bulletTex == NULL) success = false;

    skeletonTex = commonFunc::loadTexture("res/texture/Skeleton.png");
    if (skeletonTex == NULL) success = false;

    bgMusic = Mix_LoadMUS("res/sfx/Vault in Tower Fortress Soundtrack.mp3");
    if (bgMusic == NULL) success = false;

    playerSFX[0] = Mix_LoadWAV("res/sfx/sfx_sounds_impact12 (hit).wav"); // hit
    if (playerSFX[0] == NULL) success = false;

    playerSFX[1] = Mix_LoadWAV("res/sfx/sfx_movement_jump1.wav"); //jumping
    if (playerSFX[1] == NULL) success = false;

    playerSFX[2] = Mix_LoadWAV("res/sfx/sfx_sounds_impact1 (landing).wav"); //landing
    if (playerSFX[2] == NULL) success = false;

    playerSFX[3] = Mix_LoadWAV("res/sfx/sfx_wpn_laser7.wav"); //shooting
    if (playerSFX[3] == NULL) success = false;

    skeletonSFX[0] = Mix_LoadWAV("res/sfx/sfx_exp_short_hard16.wav"); //died
    if (skeletonSFX[0] == NULL) success = false;

    skeletonSFX[1] = Mix_LoadWAV("res/sfx/sfx_damage_hit2.wav"); //hit
    if (skeletonSFX[1] == NULL) success = false;

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
    ifstream map("res/texture/map.map");

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