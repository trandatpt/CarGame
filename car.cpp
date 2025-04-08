#include "car.h"
#include "defs.h"

void Car::moveLeft() {
    if (x > 110) {      // Tránh ra ngoài màn hình bên trái
        x -= 240;
    }
}

void Car::moveRight() {
    if (x < SCREEN_WIDTH - 170) {   // Tránh ra ngoài màn hình bên phải
        x += 240;
    }
}
