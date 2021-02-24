#include "ObjFactory.h"

Obj *newWeapon(Obj* par, int type);
Obj **doAttPlan(Obj* obj, int curTime);
int isArrive(T_POS x,T_POS y, T_POS tx, T_POS ty, int s);
int isArrive1(Obj * obj);
void setSpeed(Obj * obj,int t);