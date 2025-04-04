#ifndef _GRAPHIC_H__
#define _GRAPHIC_H__

struct Graphics {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* background_image = nullptr;
    SDL_Texture *car_image;

    int x, y;
    bool active;

    int score = 0;
    bool running = true;

    // Biến để điều khiển background
    int backgroundY = 0;
    int backgroundSpeed = 5;  // Tốc độ ban đầu

    void reset();
    void init();
    SDL_Texture *loadTexture(const char *filename, SDL_Renderer* renderer);
    void handleEvents();
    void update();
    void render();
    void clean();
};

#endif // _GRAPHIC_H__
