#ifndef __TIMER_H__
#define __TIMER_H__

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <dbg.h>

typedef struct Timer{
	Uint32 start_ticks; // clocks time when the timer is started
	Uint32 paused_ticks; // clock time when the timer is paused
	// defines timer status
	bool paused;
	bool started;
}Timer;

Timer* Timer_create(); // Initializes and create a timer
// Various clock function
void Timer_start(Timer* timer);
void Timer_stop(Timer* timer);
void Timer_pause(Timer* timer);
void Timer_resume(Timer* timer);
// get the timer's time
Uint32 Timer_getTicks(Timer* timer);
// check the status of the timer
bool Timer_isStarted(Timer* timer);
bool Timer_isPaused(Timer* timer);

#endif