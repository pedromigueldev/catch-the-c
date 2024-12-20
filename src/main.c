#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "stdincl.h"
#include "timer.h"
#include "apple.h"

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700
#define SCREEN_SIZE CLITERAL(Vector2) {SCREEN_WIDTH, SCREEN_HEIGHT}


typedef struct Basket {
    Texture texture;
    Vector2 pos;
    float speed;

    Rectangle src_rect;
    Rectangle dst_rect;
    Vector2 center;
} Basket;

Basket basket;

typedef enum state
{
    PLAYING,
    PAUSE,
    END
} state;

typedef struct GameState
{
    state state;
    int apples_count;
    int apples_missed;
} GameState;

GameState Game;

void InitGame()
{
    Game = (GameState) {
        .apples_count = 0,
        .apples_missed = 0,
        .state = PLAYING
    };

    Texture APPLE_TEXTURE = LoadTexture("res/C_Logo.png");
    Texture BASKET_TEXTURE = LoadTexture("res/basket.png");

    basket = (Basket) {
        .texture = BASKET_TEXTURE,
        .pos = {SCREEN_WIDTH/2, SCREEN_HEIGHT - 90},
        .speed = 200,
        .src_rect = {0,0, BASKET_TEXTURE.width, BASKET_TEXTURE.height},
        .center = {BASKET_TEXTURE.width/2, BASKET_TEXTURE.height/2},
    };

    for (int i = 0; i < APPLE_MAX_COUNT; i++) {
        _apples[i] = (Apple) {
            .texture = APPLE_TEXTURE,
            .active = false,
            .src_rect = {0,0, APPLE_TEXTURE.width, APPLE_TEXTURE.height},
            .center = {APPLE_TEXTURE.width/2, APPLE_TEXTURE.height/2},
        };

    }
}
void RenderGame()
{
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
    }
}
void RenderUI() {
    char str[40];
    char str2[40];
    snprintf(str, 40, "Cought %d", Game.apples_count);
    snprintf(str2, 40, "Missed %d", Game.apples_missed);

    DrawText(str, 10, 10, 60, WHITE);
    DrawText(str2, 10, 80, 60, RED);
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
bool LostApple()
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
int main(void)
{
    srand(time(NULL));

    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "Catch the C");
    InitGame();

    float count_down = .4f;
    Timer timer = {0};
    TimerStart(&timer, count_down);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE)) {
            if (Game.state == PAUSE) {
                Game.state = PLAYING;
            } else
                Game.state = PAUSE;
        }

        if (Game.state == PLAYING) {
            UpdateBasket();
            UpdateApples(&timer, count_down);
            if (DidColide())
                Game.apples_count++;
            if (LostApple())
                Game.apples_missed++;
            TimerUpdate(&timer);
        }

        BeginDrawing();
        ClearBackground(BLACK);

            RenderGame();
            RenderUI();

            if (Game.state == PAUSE) {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){255,255,255,170});
                Vector2 text_vec = MeasureTextEx(GetFontDefault(), "PAUSED", 80, 10);
                DrawTextPro(GetFontDefault(), "PAUSED", (Vector2) {SCREEN_WIDTH/2 - text_vec.x/2, SCREEN_HEIGHT/2 - text_vec.y/2}, (Vector2) {0,0}, 0, 80, 10, BLACK);
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
