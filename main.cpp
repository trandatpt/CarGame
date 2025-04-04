#include <SDL.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "defs.h"
#include "graphics.h"
#include "car.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

int main(int argc, char* argv[]) {
    Graphics graphics;
    graphics.init();

    while (graphics.running) {
        graphics.handleEvents();
        graphics.update();
        graphics.render();
        SDL_Delay(16);
    }

    graphics.clean();
    std::cout << "Game Over! Your score: " << graphics.score << std::endl;
    return 0;
}
