#include <iostream>
#include "defs.h"
#include "graphics.h"
#include "car.h"

using namespace std;

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
    cout << "Game Over! Your score: " << graphics.score << endl;
    return 0;
}
