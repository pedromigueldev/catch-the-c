#ifndef APPLE_H
#define APPLE_H
#include "stdincl.h"
#include "timer.h"

#define APPLE_MAX_COUNT 19

typedef struct Apple {
    Texture texture;
    Vector2 pos;
    float fall_speed;
    bool active;

    Rectangle src_rect;
    Rectangle dst_rect;
    Vector2 center;
} Apple;

extern Apple _apples[APPLE_MAX_COUNT];
void UpdateApples(Timer* timer, float count_down);

#endif
