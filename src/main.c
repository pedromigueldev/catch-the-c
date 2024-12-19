#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
#include "timer.h"

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700
#define SCREEN_SIZE CLITERAL(Vector2) {SCREEN_WIDTH, SCREEN_HEIGHT}

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

typedef struct Basket {
    Texture texture;
    Vector2 pos;
    float speed;

    Rectangle src_rect;
    Rectangle dst_rect;
    Vector2 center;
} Basket;

Apple _apples[APPLE_MAX_COUNT];
Basket basket;

typedef enum GameState
{
    PLAYING,
    PAUSE,
    END
} GameState;


void InitGame()
{
    Texture APPLE_TEXTURE = LoadTexture("res/C_Logo.png");
    Texture BASKET_TEXTURE = LoadTexture("res/basket.png");

    basket = (Basket) {
        .texture = BASKET_TEXTURE,
        .pos = {SCREEN_WIDTH/2, SCREEN_HEIGHT - 90},
        .speed = 200,
        .src_rect = {0,0, BASKET_TEXTURE.width, BASKET_TEXTURE.height},
        .center = {BASKET_TEXTURE.width/2, BASKET_TEXTURE.height/2},
    };

    for (int i = 0; i <APPLE_MAX_COUNT; i++) {
        _apples[i] = (Apple) {
            .texture = APPLE_TEXTURE,
            .active = false,
            .src_rect = {0,0, APPLE_TEXTURE.width, APPLE_TEXTURE.height},
            .center = {APPLE_TEXTURE.width/2, APPLE_TEXTURE.height/2},
        };

    }
}
void UpdateBasket()
{
    Vector2 motion = {0,0};
    Vector2 basket_scale;

    if (IsKeyDown(KEY_A))
        motion.x += -1;
    if (IsKeyDown(KEY_D))
        motion.x += 1;

    basket_scale = Vector2Scale(motion, GetFrameTime() * basket.speed);
    basket.pos = Vector2Add(basket.pos, basket_scale);
}
bool DidColide() {
    for (int i = 0; i < APPLE_MAX_COUNT; i++) {
        if (_apples[i].active) {
            if (CheckCollisionRecs(
                (Rectangle){_apples[i].pos.x, _apples[i].pos.y, _apples[i].src_rect.width /3, _apples[i].src_rect.height/4},
                (Rectangle) {basket.pos.x, basket.pos.y, basket.src_rect.width/2, basket.src_rect.height/2}
                )
            ) {
                _apples[i].active = false;
                return true;
            }
        }
    }
    return false;
}

void UpdateApples(Timer* timer, float count_down)
{
    if (TimerDone(timer)) {
        for (int i = 0; i < APPLE_MAX_COUNT; i++) { // makes an apple active every (count_down) seconds
            if (!_apples[i].active) {
                float fall_speed = GetRandomValue(200, 500);

                _apples[i].active = true;
                _apples[i].pos = (Vector2) {GetRandomValue(10, SCREEN_WIDTH-100), -100};
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

int main(void)
{
    GameState game_state = PLAYING;
    srand(time(NULL));

    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "Catch the C");
    InitGame();
    int apples_count = 0;
    int apples_missed = 0;

    float count_down = .4f;
    Timer timer = {0};
    TimerStart(&timer, count_down);
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE)) {
            if (game_state == PAUSE) {
                game_state = PLAYING;
            } else
                game_state = PAUSE;
        }

        if (game_state == PLAYING) {
            UpdateBasket();
            UpdateApples(&timer, count_down);
            if (DidColide())
                apples_count++;
            TimerUpdate(&timer);
        }

        BeginDrawing();
        ClearBackground(BLACK);

            char str[40];
            char str2[40];
            snprintf(str, 40, "Cought %d", apples_count);
            snprintf(str2, 40, "Missed %d", apples_missed);

            DrawTexturePro(
                basket.texture,
                basket.src_rect,
                (Rectangle) {basket.pos.x, basket.pos.y, basket.src_rect.width /2, basket.src_rect.height/2 },
                Vector2Divide(basket.center, (Vector2) {2,2}),
                0, WHITE);

            for (int i = 0; i < APPLE_MAX_COUNT; i++) {
                if (_apples[i].active && _apples[i].pos.y <= SCREEN_HEIGHT) {
                    DrawTexturePro(
                        _apples[i].texture,
                        _apples[i].src_rect,
                        (Rectangle){_apples[i].pos.x, _apples[i].pos.y, _apples[i].src_rect.width /3, _apples[i].src_rect.height/3},
                        Vector2Divide(_apples[i].center, (Vector2) {3,3}),
                        0, WHITE);
                }

                if (_apples[i].pos.y >= SCREEN_HEIGHT) {
                    apples_missed++;
                    _apples[i].active = false;
                }
            }

            DrawText(str, 10, 10, 60, WHITE);
            DrawText(str2, 10, 80, 60, RED);

            if (game_state == PAUSE) {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){255,255,255,170});
                Vector2 text_vec = MeasureTextEx(GetFontDefault(), "PAUSED", 80, 10);
                DrawTextPro(GetFontDefault(), "PAUSED", (Vector2) {SCREEN_WIDTH/2 - text_vec.x/2, SCREEN_HEIGHT/2 - text_vec.y/2}, (Vector2) {0,0}, 0, 80, 10, BLACK);
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
