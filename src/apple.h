#ifndef APPLE_H
#define APPLE_H
#include "stdincl.h"
#include "timer.h"
#include "basket.h"

typedef struct Apple {
    Texture texture;
    Vector2 pos;
    float fall_speed;
    bool active;

    Rectangle src_rect;
    Rectangle dst_rect;
    Vector2 center;
} Apple;

void UpdateApples(Timer* timer, float count_down, Apple _apples[]);
bool LostApple(Apple _apples[]);
bool DidCatchApple(Basket* basket, Apple _apples[]);

#endif
