#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "MySDL_CNS.h"
#include <stdarg.h>
#include <SDL_image.h>

SDL_Window *win = NULL;
SDL_Renderer *ren = NULL;

int winW, winH;

TTF_Font *font = NULL;
char fontPath[] = "./img/1.ttf";
SDL_Color fontColor = { 0, 0, 0, 255 };
SDL_Rect fontRect = { 20, 20, 300, 60 };
SDL_Rect bigRect = { 20, 20, 1000, 60 };
int fontSize = 50;

SDL_Color backColor = { 0, 255, 255, 255 };
SDL_Color rectColor = { 255, 0, 0, 255 };

#define LINES 30
#define LINE 30
#define CACHE_SIZE 500
static int FRESH=100;
static char consoleCache[CACHE_SIZE][LINE+1]={0};
static int conCacheSizes[CACHE_SIZE]={0};
static int conBegin=0;
static int conEnd=0;
static int concurlen=0;
static int conFresh=0;
static int conFontSize = 50;

void mySDL_InitFont() {
  TTF_Init();
  font = TTF_OpenFont(fontPath, fontSize);
}

SDL_Texture* loadImg(char* imgPath) {
  SDL_Surface *image = IMG_Load(imgPath);
  //Uint32 colorkey=SDL_MapRGB(image->format,0,0,0);
  //SDL_SetColorKey(image,SDL_TRUE,colorkey);
  // 载入的图片生成SDL贴图材质
  //SDL_BlitSurface(image,NULL,surface,&rect);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, image);
	SDL_FreeSurface(image);
	return texture;
}

void mySDL_Init() {
  SDL_Init(SDL_INIT_VIDEO);
  win = SDL_CreateWindow("win", 20, 20, 500, 1000,SDL_WINDOW_RESIZABLE);
  ren = SDL_CreateRenderer(win, -1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
  SDL_GetWindowSize(win, &winW, &winH);
  mySDL_InitFont();
  conFontSize=(winW-50)/(LINE-1);
}

void SDL_Clear() {
  SDL_SetRenderDrawColor(ren, backColor.r, backColor.g, backColor.b,
                         backColor.a);
  SDL_RenderClear(ren);
}

void mySDL_DrawRect(SDL_Rect rec) {
  SDL_SetRenderDrawColor(ren, rectColor.r, rectColor.g, rectColor.b,
                         rectColor.a);
  SDL_RenderFillRect(ren, &rec);
}

void drawTex(char *ch, SDL_Rect rec, SDL_Color color, TTF_Font * font) {
  SDL_Surface *sur = TTF_RenderUTF8_Blended(font, ch, color);
  SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, sur);
  SDL_FreeSurface(sur);
  SDL_RenderCopy(ren, tex, NULL, &rec);
  SDL_DestroyTexture(tex);
}

void drawTex1(SDL_Rect rec,char *ch) {
  drawTex(ch, rec, fontColor, font);
}

void drawTex1_(SDL_Rect rec,char *ch,...) {
	char buf[100];
	va_list al;
	va_start(al,ch);
	vsprintf(buf,ch,al);
  drawTex(buf, rec, fontColor, font);
	va_end(al);
}

void drawTex2(char *ch) {
  drawTex1(fontRect,ch);
}

void drawBigTex2(char *ch) {
  drawTex1(bigRect,ch);
}

void drawTex2_(char* str,...){
	char buf[100];
	va_list al;
	va_start(al,str);
	vsprintf(buf,str,al);
	drawTex2(buf);
	va_end(al);
}

void drawTex3(int l,char* ch) {
	SDL_Rect rect=fontRect;
	rect.y+=fontSize*l;
  drawTex1(rect,ch);
}

void drawBigTex3(int l,char *ch) {
	SDL_Rect rect=bigRect;
	rect.y+=fontSize*l;
  drawTex1(rect,ch);
}

void drawTex3_(int l,char* str,...){
	char buf[100];
	va_list al;
	va_start(al,str);
	vsprintf(buf,str,al);
	drawTex3_(l,buf);
	va_end(al);
}

int conCacheSize(){
	if(conBegin>conEnd){
		return conEnd+CACHE_SIZE-conBegin;
	}else{
		return conEnd-conBegin;
	}
}

int conWillFull(){
	return conCacheSize()*100/CACHE_SIZE>70;
}

void drawConsole(int time){
	if(time>=conFresh){
		conFresh=time+FRESH;
		if(conCacheSize()>LINES){
			conBegin=(conBegin+1)%CACHE_SIZE;
			if(FRESH>10)FRESH/=2;
		}else{
			if(FRESH<320)FRESH+=10;
		}
	}
	int w=conFontSize*20;
	int h=conFontSize*1.5;
	int x=5;
	int y=winH-(LINES*h);
	char info[40]={0};
	SDL_Rect rect1={x,y-h,w,h};
	drawTex1_(rect1,"consloe:invl=%i,size=%i,bgn=%i,end=%i",
					FRESH,conCacheSize(),conBegin,conEnd);
	int line=conBegin;
	for(int j=0;j<LINES;j++){
		if(line==conEnd)break;
		w=conFontSize*conCacheSizes[line];
		SDL_Rect rect={x,y,w,h};
		drawTex1(rect,consoleCache[line]);
		y+=h;
		line=(line+1)%CACHE_SIZE;
	}
}

void drawText(char *str){
	int i=0;
	while (str[i]!=0){
		if(str[i]=='\n'||concurlen==LINE){
			conCacheSizes[conEnd]=concurlen;
			consoleCache[conEnd][concurlen++]=0;
			if(concurlen!=0){
				concurlen=0;
				conEnd=(conEnd+1)%CACHE_SIZE;
			}
		}else{
			consoleCache[conEnd][concurlen++]=str[i];
		}
		i++;
	}
}

void drawText_(char* str,...){
	char buf[100];
	va_list al;
	va_start(al,str);
	vsprintf(buf,str,al);
	drawText(buf);
	va_end(al);
}

void mySDL_ColseFont() {
  TTF_CloseFont(font);
  TTF_Quit();
}

void mySDL_Destroy() {
  mySDL_ColseFont();
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
}