#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "graphics.h"
#include "defs.h"
#include "car.h"
#include "obstacle.h"

using namespace std;

Car car;
vector<Obstacle> obstacles;

void Graphics::init() {
    window = SDL_CreateWindow("Car Dodge Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    srand(time(0));

    // Khởi tạo vị trí ban đầu của xe
    car.x = lane_positions[rand() % 4];
    car.y = SCREEN_HEIGHT - CAR_HEIGHT - 20;

    // Tạo 3 obstacle ban đầu và thêm vào vector
    for (int i = 0; i < 3; i++) {
        Obstacle obs;
        obs.reset();
        obs.y -= i * (OBSTACLE_HEIGHT + 200);
        obstacles.push_back(obs);
    }

    background = 0;
    backgroundSpeed = 5; // tốc độ cuộn ban đầu

    car_image = loadTexture("asset//car.png", renderer);
    background_image = loadTexture("asset//roadway.jpg", renderer);
    obstacle_images[0] = loadTexture("asset//obstacle1.png", renderer);
    obstacle_images[1] = loadTexture("asset//obstacle2.png", renderer);
    obstacle_images[2] = loadTexture("asset//obstacle3.png", renderer);
    obstacle_images[3] = loadTexture("asset//obstacle4.png", renderer);
    obstacle_images[4] = loadTexture("asset//obstacle5.png", renderer);

    TTF_Init();
    font = TTF_OpenFont("asset//arial.ttf", 30);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    bgm = Mix_LoadMUS("asset//music.mp3");
    crashSound = Mix_LoadWAV("asset//dead.mp3");
}

SDL_Texture* Graphics::loadTexture(const char *filename, SDL_Renderer* renderer) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
    return texture;
}

void Graphics::state() {    // xử lí trạng thái game
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;

        if (e.type == SDL_KEYDOWN) {
            if (gameState == START) {
                if (e.key.keysym.sym == SDLK_RETURN) { // Nhấn Enter để chơi
                    gameState = PLAYING;
                    if (bgm) Mix_PlayMusic(bgm, -1); // -1 là lặp vô hạn
                }
            } else if (gameState == PLAYING) {
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT: car.moveLeft(); break;
                    case SDLK_RIGHT: car.moveRight(); break;
                }
            } else if (gameState == GAMEOVER) {
                if (e.key.keysym.sym == SDLK_RETURN) {
                    resetGame(); // Tạo hàm này bên dưới
                }
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }
    }
}

void Graphics::update() {   // cập nhật trạng thái game
    if (gameState != PLAYING) return;

    background += backgroundSpeed;

    if (background >= SCREEN_HEIGHT) {
        background = 0; // lặp lại nền
    }

    int maxSpeed = 15;
    backgroundSpeed = 5 + min((int)sqrt(score), maxSpeed - 5);  // tăng tốc độ cuộn theo điểm

    for (auto& obs : obstacles) {   // duyệt obstacle
        obs.y += backgroundSpeed;
        if (obs.y > SCREEN_HEIGHT) {    // obstacle ra ngoài màn hình reset lại và + điểm
            obs.reset();
            score++;
        }

        //Kiểm tra va chạm
        if (obs.active &&
            car.x < obs.x + OBSTACLE_WIDTH &&
            car.x + CAR_WIDTH > obs.x &&
            car.y < obs.y + OBSTACLE_HEIGHT &&
            car.y + CAR_HEIGHT > obs.y) {
            Mix_PlayChannel(-1, crashSound, 0);
            Mix_HaltMusic(); // dừng nhạc khi đâm
            SDL_Delay(1000); // Delay trước khi thoát khỏi game để nghe được nhạc va chạm
            gameState = GAMEOVER;
        }
    }
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, const string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Graphics::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // nền đen
    SDL_RenderClear(renderer);

    if (gameState == START) {
        SDL_Color white = {255, 255, 255, 255};
        SDL_Color yellow = {255, 255, 0, 255};

        renderText(renderer, font, "CAR DODGE GAME", SCREEN_WIDTH/2 - 170 , 150, yellow);
        renderText(renderer, font, "Press ENTER to Play", SCREEN_WIDTH/2 - 190 , 250, white);
        renderText(renderer, font, "Use left and right to move", SCREEN_WIDTH/2 - 250, 300, white);
    }
    else if (gameState == PLAYING) {
        // Vẽ background cuộn
        SDL_Rect bgRect1 = { 0, background - SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_Rect bgRect2 = { 0, background, SCREEN_WIDTH, SCREEN_HEIGHT };
        if (background_image) {
            SDL_RenderCopy(renderer, background_image, NULL, &bgRect1);
            SDL_RenderCopy(renderer, background_image, NULL, &bgRect2);
        }
        SDL_Rect carRect = { car.x, car.y, CAR_WIDTH, CAR_HEIGHT };
        if (car_image) {
            SDL_RenderCopy(renderer, car_image, NULL, &carRect);
        }
        for (const auto& obs : obstacles) {
            SDL_Rect obsRect = { obs.x, obs.y, OBSTACLE_WIDTH, OBSTACLE_HEIGHT };
            if (obstacle_images[obs.imageIndex]) {
            SDL_RenderCopy(renderer, obstacle_images[obs.imageIndex], NULL, &obsRect);
            }
        }
        SDL_Color yellow = {255, 255, 0, 255};
        renderText(renderer, font, "Score: " + to_string(score), 20, 20, yellow);
    }
    else if (gameState == GAMEOVER) {
        SDL_Color red = {255, 0, 0, 255};
        SDL_Color white = {255, 255, 255, 255};
        renderText(renderer, font, "GAME OVER", SCREEN_WIDTH / 2 - 120, 150, red);
        renderText(renderer, font, "Your Score: " + to_string(score), SCREEN_WIDTH / 2 - 150, 220, white);
        renderText(renderer, font, "Press ENTER to Play Again", SCREEN_WIDTH / 2 - 250, 280, white);
        renderText(renderer, font, "Press ESC to Quit", SCREEN_WIDTH / 2 - 190, 320, white);
    }
    SDL_RenderPresent(renderer);
}

void Graphics::quit() { // giải phóng bộ nhớ
    if (car_image) SDL_DestroyTexture(car_image);
    if (background_image) SDL_DestroyTexture(background_image);

    for (int i = 0; i < 5; i++) {
        if (obstacle_images[i]) SDL_DestroyTexture(obstacle_images[i]);
    }

    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);

    TTF_CloseFont(font);
    TTF_Quit();

    if (bgm) Mix_FreeMusic(bgm);
    if (crashSound) Mix_FreeChunk(crashSound);
    Mix_CloseAudio();

    SDL_Quit();
}

void Graphics::resetGame() {
    score = 0;
    background = 0;
    backgroundSpeed = 5;
    car.x = lane_positions[rand() % 4];
    car.y = SCREEN_HEIGHT - CAR_HEIGHT - 20;

    obstacles.clear();
    for (int i = 0; i < 3; i++) {
        Obstacle obs;
        obs.reset();
        obs.y -= i * (OBSTACLE_HEIGHT + 200);
        obstacles.push_back(obs);
    }

    gameState = PLAYING;
    if (bgm) Mix_PlayMusic(bgm, -1);
}
