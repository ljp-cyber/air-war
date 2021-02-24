// gcc LeiDian.c -o test -l storage/emulated/0/C4/ITc小程序/雷电/Game
// LeiDian.c ./game/obj/PlaneType.c ./game/obj/Attplan.c
// ./game/obj/ObjFactory.c
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "./sdl/TimerAndFPS.c"
#include "./sdl/MySDL_Click.c"
#include "./sdl/MySDL.c"
#include "./game/GameMain.h"
#include "DrawGame.c"
#include "fp.c"
#include <stdarg.h>

int pause=0;
int gstart=0;
void btn1Func(){
	pause=(++pause)%2;
	drawText_("btn1 click:pause=%i\n",pause);
}
void btn2Func(){
	gstart=(++gstart)%2;
	drawText_("btn2 click:start=%i\n",gstart);
}

int main() {
	
  //SDL_SetError("test err");
  //SDL_Log("test log");//会在logcat里显示
  fprint_init(1);
  FDEBUG("%s", "start");
  fprint_close();
  
  gameSDL_init();
  getPosBegin();
  timerStart();
  newGame(winW, winH,FPS);
  SDL_Rect prect={ 800,1000,200,100 };
  SDL_Rect srect={ 800,1250,200,100 };
  addBtn(0,"暂停",prect,btn1Func);
  addBtn(1,"退出",srect,btn2Func);
  while (1) {
    //SDL_Clear();
    gameSDL_clear();
    drawGame(gameQTree);
    
    if (!conWillFull()&&!pause) {
    	playerMove(X,Y);
      doGame();
    }
    
    drawBtns();
	  char tmp[200]="";
    //sprintf(ch_fps,"fps=%i,X=%i,Y=%i",FPS,(int)(X*winW),(int)(Y*winH));
    drawTex2_("fps=%i,time=%i,hp=%i", FPS, getTimespan(), player->hp);
    toStrQTree(tmp, gameQTree);
    drawTex3(2, tmp);
    toStrFac(tmp);
    drawTex3(3, tmp);
    //drawConsole(getTimespan());
    
    SDL_RenderPresent(ren);
    FPS_Modified();
  }
}