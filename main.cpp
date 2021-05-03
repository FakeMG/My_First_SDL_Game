#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <ctime>
#include <cstdlib>

#include "RenderWindow.h"
#include "Game.h"
using namespace std;

int main(int argc, char* argv[]) {
    Game mainGame;
    srand(time(NULL));
    if (!mainGame.init()) return 0;
    else {
        if (!mainGame.loadMedia()) return 0;
        else {
            if (!mainGame.createMap() || !mainGame.createLevel() || !mainGame.createMenu() || !mainGame.createPlayer() || !mainGame.createSkeleton()) {
                cout << "FAILED TO CREATE GAME ELEMENTS!" << endl;
                return 0;
            }
            mainGame.start_ExplosionSFX();
            
            while (mainGame.isRunning()) {
                SDL_Event event;
                while (SDL_PollEvent(&event)) mainGame.handleGameInput(event);
                if (mainGame.getMenuList().at(0).isMenu()) mainGame.render_mainMenu();
                else if (mainGame.getMenuList().at(0).isPaused()) mainGame.pauseMusicTime();
                else {
                    mainGame.playMusic();
                    mainGame.render_update_Game();
                }
            }
        }
    }
    mainGame.destroy();
    commonFunc::cleanUp();
    return 0;
}