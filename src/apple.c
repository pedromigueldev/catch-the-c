#include "apple.h"

Apple _apples[APPLE_MAX_COUNT];

void UpdateApples(Timer* timer, float count_down)
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
