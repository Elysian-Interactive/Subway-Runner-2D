#include <Timer.h>

// Function definitions

Timer* Timer_create()
{
	Timer* new_timer = (Timer*)malloc(sizeof(Timer));
	check(new_timer != NULL, "Failed to create a timer! SDL_Error: %s",SDL_GetError());
	
	new_timer->start_ticks = 0;
	new_timer->paused_ticks = 0;
	new_timer->paused = false;
	new_timer->started = false;
	
	return new_timer;
error:
	return NULL;
}

void Timer_start(Timer* timer) // we can simply call it again to restart the timer
{							   // and must clear other flags too
	check(timer != NULL, "Invalid Timer!");
	
	timer->started = true;
	timer->paused = false;
	
	timer->start_ticks = SDL_GetTicks(); // get the elapsed time
	timer->paused_ticks = 0;
	
error: // fallthrough
	return;
}

void Timer_stop(Timer* timer)
{
	check(timer != NULL, "Invalid Timer!");
	
	// basically reinitialize all  the values
	timer->started = false;
	timer->paused = false;
	timer->start_ticks = 0;
	timer->paused_ticks = 0;

error: // fallthrough
	return;
}

void Timer_pause(Timer* timer)
{
	check(timer != NULL, "Invalid Timer!");
	
	// if the timer is running and isn't already paused
	if(timer->started && !timer->paused){ // as it won't make sense to pause a timer which hasn't already started
		// pause the timer
		timer->paused = true;
		// calculate the paused ticks
		timer->paused_ticks = SDL_GetTicks() - timer->start_ticks;
		// reset the start ticks
		timer->start_ticks = 0;
	}
	
error: // fallthrough
	return;
}

void Timer_resume(Timer* timer)
{
	check(timer != NULL, "Invalid Timer!");
	
	// if the timer is running and is paused
	if(timer->started && timer->paused){
		// resume the timer
		timer->paused = false;
		// reset the start ticks
		timer->start_ticks = SDL_GetTicks() - timer->paused_ticks;
		// reset the paused ticks
		timer->paused_ticks = 0;
		
		// this technique maintains the same amount of relative time
		// between our respective timer and the global timer (SDL_GetTicks())
	}

error: // fallthrough
	return;
}

Uint32 Timer_getTicks(Timer* timer)
{
	// storing the timer timer here
	Uint32 time = 0;
	
	check(timer != NULL, "Invalid Timer!");
	
	// if the timer is running
	if(timer->started){
		//if the timer is paused
		if(timer->paused){
			// set the time at which it was paused
			time = timer->paused_ticks;
		}
		else{
			// set the relative time
			time = SDL_GetTicks() - timer->start_ticks;
		}

	}

error: // fallthrough
	return time;
}

bool Timer_isStarted(Timer* timer)
{
	check(timer != NULL, "Invalid Timer!");
	
	return timer->started;
error:
	return false;
}

bool Timer_isPaused(Timer* timer)
{
	check(timer != NULL, "Invalid Timer!");
	
	return timer->started && timer->paused; // as you won't be able to pause if it wasn't started at all
error:
	return false;
}

