#ifndef TIMER_H
#define TIMER_H
#include "stdincl.h"

typedef struct {
    float lifetime;
} Timer;

void TimerStart(Timer* timer, float lifeteme);
void TimerUpdate(Timer* timer);
bool TimerDone(Timer* timer);

#endif
