#include "SDL2/SDL.h"
#define MAX_FPS 60

const static Uint32 timespan = 1000 / MAX_FPS;
static Uint32 lastTime = 0;
static Uint32 start = 0;

int FPS = MAX_FPS;


void timerStart(){
	start = SDL_GetTicks();
}

int getTimespan(){
	return SDL_GetTicks() - start;
}

void FPS_Modified(){
	int temp = SDL_GetTicks() - lastTime;
  if (temp < timespan){
    SDL_Delay(timespan - SDL_GetTicks() + lastTime);
    FPS=MAX_FPS;
  }
  else
    FPS = 1000 / temp;
  lastTime = SDL_GetTicks();
}