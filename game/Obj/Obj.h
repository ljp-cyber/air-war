#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _PLANETYPE_H
#define _PLANETYPE_H

#define MAX_WEA 5
#define T_POS float

typedef struct {
  int x;
  int y;
} Pos;

typedef struct {
  int x;
  int y;
  int s;
  int r;
} Speed;

typedef struct {
  int w;
  int h;
} Size;

typedef struct {
  T_POS x;
  T_POS y;
  int w;
  int h;
  
	int type;//类别标识
  int state;//战斗状态
  
  int weapons[MAX_WEA];
	int maxFuel;
	int maxHp;
  int p;//力量
  T_POS maxSpe;
  
  int id;
  int g;//组
  int f;//燃料
  int hp;
  T_POS spe;
  T_POS s;//路程
  T_POS vx;
  T_POS vy;
  float r;//方向
  
  void* plan;
  void* planState;
} Obj;

void setObj(Obj* obj,int tpye,int hp,int pow,int speed,int w,int h,int f,int* wea);

void setObj1(Obj* obj,int* data,int* weaData);

void freeObj(Obj* obj);

void printObj(Obj* obj);

void toStrObj(char *str, void * obj);

void printObj1(void* obj);

Obj* newPlayer1();

Obj* newWeapon1();

#endif