#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "stdincl.h"
#include "game.h"
#include "timer.h"
#include "apple.h"
#include "basket.h"

GameState Game;
Basket basket;
Apple _apples[APPLE_MAX_COUNT];

void InitGame();
void RenderGame();
void RenderUI();

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
            UpdateBasket(&basket);
            UpdateApples(&timer, count_down, _apples);
            if (DidCatchApple(&basket, _apples))
                Game.apples_count++;
            if (LostApple(_apples))
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

void RenderUI()
{
    char str[40];
    char str2[40];
    snprintf(str, 40, "Cought %d", Game.apples_count);
    snprintf(str2, 40, "Missed %d", Game.apples_missed);

    DrawText(str, 10, 10, 60, WHITE);
    DrawText(str2, 10, 80, 60, RED);
}
