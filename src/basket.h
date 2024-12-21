#ifndef BASKET_H
#define BASKET_H
#include "stdincl.h"

typedef struct Basket {
    Texture texture;
    Vector2 pos;
    float speed;

    Rectangle src_rect;
    Rectangle dst_rect;
    Vector2 center;
} Basket;

void UpdateBasket(Basket* basket);

#endif
