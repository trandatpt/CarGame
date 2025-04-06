#ifndef _OBSTACLE_H__
#define _OBSTACLE_H__

struct Obstacle {
    int x, y;
    bool active;
    int imageIndex;
    void reset();
};

#endif
