#include "SDL2/SDL_rect.h"

SDL_Rect* mySDL_NewRect(int x, int y, int w, int h) {
  SDL_Rect *result = (SDL_Rect *) malloc(sizeof(SDL_Rect));
  result->x = x;
  result->y = y;
  result->w = w;
  result->h = h;
  return result;
}

void printRect(SDL_Rect* rect){
	printf("rect={%i,%i,%i,%i}\n",rect->x,
	rect->y,rect->w,rect->h);
}

void mySDL_FreeRect(SDL_Rect* rect){
	free(rect);
}