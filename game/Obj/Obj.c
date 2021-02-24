#include <stdio.h>
#include <string.h>
#include "Obj.h"
void setObj(Obj * obj, int t, int hp, int p, int s, int w, int h, int f,
            int *wea) {
  memset(obj, 0, sizeof(Obj));
  obj->maxHp = hp;
  obj->hp = hp;
  obj->p = p;
  obj->maxSpe = s;
  obj->w = w;
  obj->h = h;
  obj->maxFuel = f;
  obj->f = f;
  obj->type = t;
  for (int i = 0; i < MAX_WEA; i++) {
    obj->weapons[i] = wea[i];
  }
}

void setObj1(Obj * obj, int *data, int *weaData) {
  setObj(obj, data[0], data[1], data[2], data[3], data[4], data[5], data[6],
         weaData);
}

void freeObj(Obj * obj) {
  free(obj);
}

Obj *newPlayer1() {
  int arr1[] = { 0, 1, 1, 15, 150, 100, 1000000 };
  int arr2[] = { 1, -1, -1, -1, -1 };
  Obj *obj = (Obj *) malloc(sizeof(Obj));
  setObj1(obj, arr1, arr2);
  return obj;
}

Obj *newWeapon1() {
  int arr1[] = { 100, 1, 1, 20, 50, 50, 1000 };
  int arr2[] = { -1, -1, -1, -1, -1 };
  Obj *obj = (Obj *) malloc(sizeof(Obj));
  setObj1(obj, arr1, arr2);
  return obj;
}

void toStrObj(char *str, void *o) {
  if (o == NULL)
    return;
  Obj *obj = o;
  sprintf(str, "obj[idgxy=%i,%i,%f,%f;"
          "xySs=%f,%f,%f,%f;"
          "hp,pow,spe=%i,%i,%f;]\n",
          obj->id, obj->g, obj->x, obj->y,
          obj->vx, obj->vy, obj->s, 
          obj->spe, obj->hp, obj->p, obj->maxSpe);
}

void printObj(Obj * obj) {
  if (obj == NULL)
    return;
  printf("obj[idgxy=%i,%i,%f,%f;", obj->id, obj->g, obj->x, obj->y);
  printf("xySs=%f,%f,%f,%f;", obj->vx, obj->vy, obj->s, obj->spe);
  printf("hp,pow,spe=%i,%i,%f;]\n", obj->hp, obj->p, obj->maxSpe);
}

void printObj1(void *obj) {
  printObj((Obj *) obj);
}

int mainobj() {
  char str[50];
  Pos pos = { 0 };
  Size *size = (Size *) & pos;
  printf("%i,%i", size->w, size->h);

  toStrObj(str, newPlayer1());
  printf("%s", str);
}