#include "Game.h"

bool Game::init() {
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

bool Game::loadMedia() {
    bool success = true;
    if (!commonFunc::loadFont("res/lazy.ttf")) success = false;

    knightTex = commonFunc::loadTexture("res/texture/Metal knight.png");
    if (knightTex == NULL) success = false;

    tileTex = commonFunc::loadTexture("res/texture/Tileset.png");
    if (tileTex == NULL) success = false;

    bulletTex = commonFunc::loadTexture("res/texture/Bullet.png");
    if (bulletTex == NULL) success = false;

    skeletonTex = commonFunc::loadTexture("res/texture/Skeleton.png");
    if (skeletonTex == NULL) success = false;

    explosionTex = commonFunc::loadTexture("res/texture/Explosion.png");
    if (explosionTex == NULL) success = false;

    bgTex = commonFunc::loadTexture("res/texture/Doge.jpg");
    if (bgTex == NULL) success = false;

    buttonTex = commonFunc::loadTexture("res/texture/Button.png");
    if (buttonTex == NULL) success = false;

    circleOfLightTex = commonFunc::loadTexture("res/texture/Circle of light.png");
    if (circleOfLightTex == NULL) success = false;

    bgMusic = Mix_LoadMUS("res/sfx/Vault in Tower Fortress Soundtrack.mp3");
    if (bgMusic == NULL) success = false;

    explosionSFX = Mix_LoadWAV("res/sfx/Explosion.wav");
    if (explosionSFX == NULL) success = false;

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

    if (!success) cout << "FAILED TO LOAD MEDIA: " << SDL_GetError() << endl;
    return success;
}

void Game::FPSCounter() {
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

bool Game::createMap() {
    path_pos Map1({ }, "res/texture/map1.map");
    mapList.push_back(Map1);
    path_pos Map2({ 15,6 }, "res/texture/map2.map");
    mapList.push_back(Map2);
    path_pos Map3({ 1,10,16,8,18,8 }, "res/texture/map3.map");
    mapList.push_back(Map3);
    path_pos Map4({ 9,11 }, "res/texture/map4.map");
    mapList.push_back(Map4);
    path_pos Map5({ 7,12,12,12,17,12 }, "res/texture/map5.map");
    mapList.push_back(Map5);
    path_pos Map6({ }, "res/texture/map_spawn.map");
    mapList.push_back(Map6);
    if (mapList.size() < TOTAL_MAP) {
        cout << "FAILED TO LOAD MAP!" << endl;
        return false;
    }
    return true;
}

bool Game::createLevel() {
    for (int i = 0; i < TOTAL_LEVEL_PART; i++) {
        int random = rand() % (TOTAL_MAP - 1);
        if (i == 0) random = TOTAL_MAP - 1;
        LevelPart level(i * LEVEL_WIDTH, 0, mapList.at(random).path, tileTex);
        level.setSkeletonPos(mapList.at(random).skeletonPos);
        LevelPartList.push_back(level);

    }
    if (LevelPartList.size() < TOTAL_LEVEL_PART) {
        cout << "FAILED TO CREATE LEVEL!" << endl;
        return false;
    }
    return true;
}

bool Game::createSkeleton() {
    for (int i = 0; i < LevelPartList.size(); i++) {
        if (LevelPartList.at(i).getSkeletonPos().size() > 0) {
            for (int j = 0; j < LevelPartList.at(i).getSkeletonPos().size() - 1; j += 2) {
                Skeleton* skeleton = new Skeleton(LevelPartList.at(i).getSkeletonPos().at(j) * TILE_WIDTH + LevelPartList.at(i).getX(), LevelPartList.at(i).getSkeletonPos().at(j + 1) * TILE_WIDTH + LevelPartList.at(i).getY(), skeletonTex);
                skeletonList.push_back(skeleton);
            }
        }
    }
    if (skeletonList.empty()) return false;
    return true;
}

bool Game::createPlayer() {
    Player knight(64 * 3, LEVEL_HEIGHT - TILE_HEIGHT * 5, knightTex);
    playerList.push_back(knight);
    if (playerList.size() < 0) return false;
    return true;
}

bool Game::createMenu() {
    Menu gameMenu(buttonTex, bgTex, bgTex);
    menuList.push_back(gameMenu);
    if (menuList.size() < 0) return false;
    return true;
}

void Game::start_ExplosionSFX() {
    Mix_PlayChannel(-1, explosionSFX, -1);
    Mix_VolumeChunk(explosionSFX, 0);
}

void Game::playMusic() {
    Mix_VolumeChunk(explosionSFX, 80);
    if (Mix_PlayingMusic() == 0) {
        Mix_FadeInMusic(bgMusic, -1, 1000);
        Mix_VolumeMusic(50);
    }
    else if (playerList.at(0).isDead()) Mix_HaltMusic();
}

void Game::render_update_LevelPart() {
    //update LevelPart
    if (LevelPartList.at(0).getX() - camera.x <= -LEVEL_WIDTH) {
        int random = rand() % (TOTAL_MAP - 1);
        LevelPartList.at(0).setTilesType(mapList.at(random).path);
        LevelPartList.at(0).setLevelX(LevelPartList.at(LevelPartList.size() - 1));
        LevelPartList.at(0).setSkeletonPos(mapList.at(random).skeletonPos);
        if (LevelPartList.at(0).getSkeletonPos().size() > 0) {
            for (int j = 0; j < LevelPartList.at(0).getSkeletonPos().size() - 1; j += 2) {
                Skeleton* skeleton = new Skeleton(LevelPartList.at(0).getSkeletonPos().at(j) * TILE_WIDTH + LevelPartList.at(0).getX(), LevelPartList.at(0).getSkeletonPos().at(j + 1) * TILE_WIDTH + LevelPartList.at(0).getY(), skeletonTex);
                skeletonList.push_back(skeleton);
            }
        }
        LevelPart level = LevelPartList.at(0);
        LevelPartList.erase(LevelPartList.begin());
        LevelPartList.push_back(level);
    }

    //render levelPart
    for (int i = 0; i < LevelPartList.size(); i++) {
        LevelPartList.at(i).render(gTileClips, camera);
    }
}

void Game::render_update_bullet() {
    //render & update bullet
    vector<Bullet*> bulletList = playerList.at(0).getBulletList();
    for (int i = 0; i < playerList.at(0).getBulletList().size(); i++) {
        if (bulletList.at(i) != NULL) {
            if (bulletList.at(i)->isMoving()) {
                bulletList.at(i)->render(camera, bulletTex);
                bulletList.at(i)->update(LevelPartList);
            }
            else {
                delete bulletList.at(i);
                bulletList.at(i) = NULL;
                bulletList.erase(bulletList.begin() + i);
                playerList.at(0).setBulletList(bulletList);
            }
        }
    }
}

void Game::render_update_player() {
    //render & update player
    playerList.at(0).update(LevelPartList, skeletonList, playerSFX, camera);
    playerList.at(0).handleCamera(camera, camVel);
    playerList.at(0).render(camera);
}

void Game::render_update_skeleton() {
    //render & update skeleton
    for (int i = 0; i < skeletonList.size(); i++) {
        if (skeletonList.at(i) != NULL) {
            if (!skeletonList.at(i)->isDead()) {
                skeletonList.at(i)->render(camera);
                skeletonList.at(i)->update(playerList.at(0), LevelPartList, skeletonSFX, camera);
            }
            else {
                delete skeletonList.at(i);
                skeletonList.at(i) = NULL;
                skeletonList.erase(skeletonList.begin() + i);
            }
        }
    }
}

void Game::render_update_explosion() {
    //render explosion
    commonFunc::renderAnimation(explosionTex, camera.x - 64, 0, explosion[exCounter / 4], camera);
    exCounter++;
    if (exCounter / 4 >= 15) exCounter = 24;
}

void Game::render_update_Game() {
    commonFunc::clearRenderer();
    render_update_LevelPart();
    render_update_bullet();
    render_update_player();
    render_update_skeleton();
    render_update_explosion();
    if (playerList.at(0).isDead()) menuList.at(0).renderRetryMenu();
    commonFunc::renderPresent();
}

void Game::render_mainMenu() {
    commonFunc::clearRenderer();
    getMenuList().at(0).renderMainMenu();
    commonFunc::renderPresent();
}

void Game::handleGameInput(SDL_Event& event) {
    if (event.type == SDL_QUIT) gameRunning = false;
    menuList.at(0).handleInput(event, gameRunning, playerList.at(0), skeletonList, LevelPartList, camVel, camera);
    if (!menuList.at(0).isMenu()) playerList.at(0).handleInput(event, playerSFX);
}

void Game::setSDL_Rect() {
    int m = 0, n = 0;
    for (int i = 0; i < TOTAL_TILE_SPRITES; i++) {
        gTileClips[i].x = n;
        gTileClips[i].y = m;
        gTileClips[i].w = TILE_WIDTH;
        gTileClips[i].h = TILE_HEIGHT;
        n += TILE_WIDTH;
        if (n >= 17 * TILE_WIDTH) {
            n = 0;
            m += TILE_HEIGHT;
        }
    }

    for (int i = 0; i < 15; i++) {
        explosion[i].x = i * 256;
        explosion[i].y = 0;
        explosion[i].w = 256;
        explosion[i].h = 1280;
    }
}