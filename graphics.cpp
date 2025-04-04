#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "graphics.h"
#include "defs.h"
#include "car.h"

Graphics graphics;
Car car;
SDL_Texture* car_image = nullptr;
SDL_Texture* background_image = nullptr;
SDL_Texture* obstacle_images[5] = {nullptr};

int background_y = 0;

struct Obstacle {
    int x, y;
    bool active;
    int imageIndex; // Lưu chỉ số của ảnh dùng cho chướng ngại vật

    void reset() {
        x = lane_positions[rand() % 4];
        y = -OBSTACLE_HEIGHT;
        active = true;
        imageIndex = rand() % 5; // Chọn ảnh ngẫu nhiên từ 4 ảnh có sẵn
    }
};

std::vector<Obstacle> obstacles;

void Graphics::init() {
    window = SDL_CreateWindow("Car Dodge Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    srand(time(0));
    car.x = lane_positions[rand() % 4];
    car.y = SCREEN_HEIGHT - CAR_HEIGHT - 20;
    for (int i = 0; i < 3; i++) {
        Obstacle obs;
        obs.reset();
        obs.y -= i * (OBSTACLE_HEIGHT + 200);
        obstacles.push_back(obs);
    }
    backgroundY = 0;
    backgroundSpeed = 5;
    car_image = loadTexture("car.png", renderer);
    background_image = loadTexture("roadway.jpg", renderer);
    obstacle_images[0] = loadTexture("obstacle1.png", renderer);
    obstacle_images[1] = loadTexture("obstacle2.png", renderer);
    obstacle_images[2] = loadTexture("obstacle3.png", renderer);
    obstacle_images[3] = loadTexture("obstacle4.png", renderer);
    obstacle_images[4] = loadTexture("obstacle5.png", renderer);
}

SDL_Texture* Graphics::loadTexture(const char *filename, SDL_Renderer* renderer) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
    return texture;
}

void Graphics::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_LEFT: car.moveLeft(); break;
                case SDLK_RIGHT: car.moveRight(); break;
            }
        }
    }
}

void Graphics::update() {
    backgroundY += backgroundSpeed;

    if (backgroundY >= SCREEN_HEIGHT) {
        backgroundY = 0;
    }

    int maxSpeed = 15;
    backgroundSpeed = 5 + std::min((int)sqrt(score), maxSpeed - 5);

    for (auto& obs : obstacles) {
        obs.y += backgroundSpeed;
        if (obs.y > SCREEN_HEIGHT) {
            obs.reset();
            score++;
        }

        if (obs.active &&
            car.x < obs.x + OBSTACLE_WIDTH &&
            car.x + CAR_WIDTH > obs.x &&
            car.y < obs.y + OBSTACLE_HEIGHT &&
            car.y + CAR_HEIGHT > obs.y) {
            running = false;
        }
    }
}

void Graphics::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Vẽ background hai lần để tạo hiệu ứng cuộn liên tục
    SDL_Rect bgRect1 = { 0, backgroundY - SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_Rect bgRect2 = { 0, backgroundY, SCREEN_WIDTH, SCREEN_HEIGHT };

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

    SDL_RenderPresent(renderer);
}

void Graphics::clean() {
    if (car_image) SDL_DestroyTexture(car_image);
    if (background_image) SDL_DestroyTexture(background_image);

    for (int i = 0; i < 4; i++) {
        if (obstacle_images[i]) SDL_DestroyTexture(obstacle_images[i]);
    }

    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}
