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

RenderWindow window("Test Game", SCREEN_WIDTH, SCREEN_HEIGHT);

SDL_Texture* bgTex = window.loadTexture("res/gtx/Doge.jpg");
SDL_Texture* FakeMG = window.loadTexture("res/gtx/foo.png");
SDL_Texture* TileTex = window.loadTexture("res/gtx/tiles.png");

SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

Tile* tileSet[TOTAL_TILES];

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

bool setTiles(Tile* tiles[]) {
    //Success flag
    bool tilesLoaded = true;

    //The tile offsets
    int x = 0, y = 0;

    //Open the map
    ifstream map("res/gtx/lazy.map");

    //If the map couldn't be loaded
    if (map.fail()) {
        printf("Unable to load map file!\n");
        tilesLoaded = false;
    }
    else
    {
        //Initialize the tiles
        for (int i = 0; i < TOTAL_TILES; ++i) {
            //Determines what kind of tile will be made
            int tileType = -1;

            //Read tile from map file
            map >> tileType;

            //If the was a problem in reading the map
            if (map.fail()) {
                //Stop loading map
                printf("Error loading map: Unexpected end of file!\n");
                tilesLoaded = false;
                break;
            }

            //If the number is a valid tile number
            if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES)) {
                tiles[i] = new Tile(x, y, TileTex, tileType);
            }
            //If we don't recognize the tile type
            else
            {
                //Stop loading map
                printf("Error loading map: Invalid tile type at %d!\n", i);
                tilesLoaded = false;
                break;
            }
            //Move to next tile spot
            x += TILE_WIDTH;

            //If we've gone too far
            if (x >= LEVEL_WIDTH) {
                //Move back
                x = 0;

                //Move to the next row
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

int main(int argc, char* argv[]) {
    
    window.loadFont("res/lazy.ttf");

    if ( !init() ) {
        printf("Failed to initialize!\n");
        return 0;
    }

    if (!setTiles(tileSet)) {
        printf("Failed to load tile set!\n");
    }

    Player knight(0, 0, FakeMG);
    Entity bg(0, 0, bgTex);
 
    fpsTimer.start();

    SDL_Rect camera = { (float)0, (float)0, SCREEN_WIDTH, SCREEN_HEIGHT };
    
    //Game loop
    bool gameRunning = true;
    SDL_Event event;
    while (gameRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) gameRunning = false;
            knight.handleInput(event);
        }
        knight.update();
        knight.handleCamera(camera);

        window.clearRenderer();
        for (int i = 0; i < TOTAL_TILES; i++) {
            window.renderTile(*tileSet[i], gTileClips[tileSet[i]->getType()], camera);
        }
            
       
        window.renderTexture(bg, &camera);
        window.renderPlayer(knight, camera);
        FPSCounter();
        window.renderPresent();
    }

    //Giải phóng bộ nhớ
    window.cleanUp();
    return 0;
}

