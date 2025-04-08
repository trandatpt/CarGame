#ifndef _GRAPHIC_H__
#define _GRAPHIC_H__

#include <SDL_ttf.h>
#include <SDL_mixer.h>

enum GameState { START, PLAYING, GAMEOVER }; // trạng thái game

struct Graphics {
    GameState gameState = START;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* background_image = nullptr;
    SDL_Texture *car_image;
    SDL_Texture* obstacle_images[5] = {nullptr};
    TTF_Font* font;
    Mix_Music* bgm = nullptr;
    Mix_Chunk* crashSound = nullptr;

    int x, y;
    bool active;

    int score = 0;
    bool running = true;

    int background = 0;
    int backgroundSpeed = 5;

    void reset();
    void init();
    SDL_Texture *loadTexture(const char *filename, SDL_Renderer* renderer);
    void state();
    void update();
    void render();
    void quit();
    void resetGame();
};

#endif
