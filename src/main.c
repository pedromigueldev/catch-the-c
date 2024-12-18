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

typedef enum GameState
{
    PLAYING,
    END
} GameState;

int main(void)
{
    srand(time(NULL));

    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "Catch the C");

    int apples_count = 0;

    float apple_apper_time = .9f;
    Timer timer = {0};

    Texture APPLE_TEXTURE = LoadTexture("res/C_Logo.png");
    Texture BASKET_TEXTURE = LoadTexture("res/basket.png");

    Basket basket = {
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

    TimerStart(&timer, apple_apper_time);
    bool collision = false;
    while (!WindowShouldClose())
    {
        char str[40];
        int random_x_pos = GetRandomValue(200, SCREEN_WIDTH-200);
        Vector2 motion = {0,0};
        Vector2 basket_scale;


        if (IsKeyDown(KEY_A))
            motion.x += -1;
        if (IsKeyDown(KEY_D))
            motion.x += 1;

        basket_scale = Vector2Scale(motion, GetFrameTime() * basket.speed);
        basket.pos = Vector2Add(basket.pos, basket_scale);


        for (int i = 0; i < APPLE_MAX_COUNT; i++) {
            if (_apples[i].active) {
                if (CheckCollisionRecs(
                    (Rectangle){_apples[i].pos.x, _apples[i].pos.y, _apples[i].src_rect.width /3, _apples[i].src_rect.height/4},
                    (Rectangle) {basket.pos.x, basket.pos.y, basket.src_rect.width, basket.src_rect.height}
                )
                ) {
                    _apples[i].active = false;
                    apples_count++;
                }
            }
        }


        for (int i = 0; i < APPLE_MAX_COUNT; i++) {
            if (TimerDone(&timer) && !_apples[i].active) {
                float fall_speed = GetRandomValue(100, 200);

                _apples[i].active = true;
                _apples[i].pos = (Vector2) {random_x_pos, -100};
                _apples[i].fall_speed = fall_speed;

                TimerStart(&timer, apple_apper_time);
            }

            if (_apples[i].active) {
                Vector2 motion = {0,0};
                motion.y++;
                Vector2 apple_scale = Vector2Scale(motion, GetFrameTime() * _apples[i].fall_speed);
                _apples[i].pos = Vector2Add(_apples[i].pos, apple_scale);
            }
        }
        TimerUpdate(&timer);

        BeginDrawing();
        ClearBackground(BLACK);

        snprintf(str, 40, "%d", apples_count);
        DrawText(str, 10, 10, 80, WHITE);

        DrawTexturePro(
            basket.texture,
            basket.src_rect,
            (Rectangle) {basket.pos.x, basket.pos.y, basket.src_rect.width, basket.src_rect.height},
            basket.center,
            0, WHITE);

        for (int i = 0; i < APPLE_MAX_COUNT; i++) {
            if (_apples[i].active) {
                DrawTexturePro(
                    _apples[i].texture,
                    _apples[i].src_rect,
                    (Rectangle){_apples[i].pos.x, _apples[i].pos.y, _apples[i].src_rect.width /3, _apples[i].src_rect.height/3},
                    Vector2Divide(_apples[i].center, (Vector2) {3,3}),
                    0, WHITE);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
