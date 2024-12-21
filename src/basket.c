#include "basket.h"

void UpdateBasket(Basket* basket)
{
    Vector2 motion = {0,0};
    Vector2 basket_scale;

    if (IsKeyDown(KEY_A))
        motion.x += -1;
    if (IsKeyDown(KEY_D))
        motion.x += 1;

    basket_scale = Vector2Scale(motion, GetFrameTime() * basket->speed);
    basket->pos = Vector2Add(basket->pos, basket_scale);
}
