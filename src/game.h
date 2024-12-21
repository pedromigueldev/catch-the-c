#ifndef GAME_H
#define GAME_H

typedef enum state
{
    PLAYING = 0,
    PAUSE = 1,
    END = 2
} state;

typedef struct GameState
{
    state state;
    int apples_count;
    int apples_missed;
} GameState;


#endif
