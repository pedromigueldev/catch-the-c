#include <stdbool.h>
#include <stdio.h>
#include <raylib.h>
#ifndef TIMER_H
#define TIMER_H

typedef struct {
    float lifetime;
} Timer;

void TimerStart(Timer* timer, float lifeteme);
void TimerUpdate(Timer* timer);
bool TimerDone(Timer* timer);

#endif
