#include "apple.h"
#include "basket.h"


bool DidCatchApple(Basket* basket, Apple _apples[]) {
    for (int i = 0; i < APPLE_MAX_COUNT; i++) {
        if (_apples[i].active) {
            if (CheckCollisionRecs(
                (Rectangle){_apples[i].pos.x, _apples[i].pos.y, _apples[i].src_rect.width /3, _apples[i].src_rect.height/4},
                (Rectangle) {basket->pos.x, basket->pos.y, basket->src_rect.width/2, basket->src_rect.height/2}
                )
            ) {
                _apples[i].active = false;
                return true;
            }
        }
    }
    return false;
}

bool LostApple(Apple _apples[])
{
    for (int i = 0; i < APPLE_MAX_COUNT; i++) {
        if (_apples[i].pos.y >= SCREEN_HEIGHT) {
            _apples[i].active = false;
            _apples[i].pos.y = 0;
            return true;
        }
    }
    return false;
}

void UpdateApples(Timer* timer, float count_down, Apple _apples[])
{
    if (TimerDone(timer)) {
        for (int i = 0; i < APPLE_MAX_COUNT; i++) { // makes an apple active every (count_down) seconds
            if (!_apples[i].active) {

                _apples[i].active = true;
                _apples[i].pos = (Vector2) {GetRandomValue(10, 600), -100};
                _apples[i].fall_speed = 500;
                break;
            }
        }
        TimerStart(timer, count_down);
    }

    for (int i = 0; i < APPLE_MAX_COUNT; i++) { // move all active apples
        if (_apples[i].active) {
            Vector2 motion = {0,0};
            motion.y++;
            Vector2 apple_scale = Vector2Scale(motion, GetFrameTime() * _apples[i].fall_speed);
            _apples[i].pos = Vector2Add(_apples[i].pos, apple_scale);
        }
    }
}
