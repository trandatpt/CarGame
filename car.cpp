#include "car.h"
#include "defs.h"

void Car::moveLeft() {
    if (x > 110) {
        x -= 240;
    }
}

void Car::moveRight() {
    if (x < SCREEN_WIDTH - CAR_WIDTH - 170) {
        x += 240;
    }
}
