#include "defs.h"
#include "obstacle.h"
#include <cstdlib>

void Obstacle::reset() {
    x = lane_positions[rand() % 4];
    y = -OBSTACLE_HEIGHT;
    active = true;
    imageIndex = rand() % 5;
}
