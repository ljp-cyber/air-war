#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
//#include "ListNode.h"
//#include "PlaneType.h"

int mapW;
int mapH;

float path1[][3] = {
  {1, 0.5, 0},                  // 标识1:移到坐标w*0.5,h*0
  {1, 0.5, 0.2},
  {1, 0.1, 0.2},
  {1, 0.9, 0.2},
  {3, 2, 2},                    // 标识3：倒退2格循环2次
  {2, 1.57, 1000},              // 标识2：角度1.57移动1000
  {0, 0, 0},                    // 标识0：以x和y轴速度0一直移动
  {4, 0, 0}                     // 标识4：玩家模式
};

float pathPlayer[][3] = {
  {1, 0.5, 0.8},
  {4, 0.5, 0.8}                 // 标识4：玩家模式
};

void setMap(int w, int h) {
  mapW = w;
  mapH = h;
}

void setPath(Obj * obj, float path[][3]) {
  ListNode *head = NULL;
  for (int i = 0;; i++) {
    List_AddNode2(&head, &path[i][0]);
    if (path[i][0] == 0)
      break;
  }
  obj->pos.x = path[0][1] * mapW;
  obj->pos.y = path[0][2] * mapH;
  obj->speed.path = head->next;
  obj->speed.S = 0;
  obj->speed.s = obj->move;
}

bool isArrive(Pos p, int tx, int ty, int s) {
  //printf("isArr:%i,%i,%i,%i\n", p.x, p.y, tx, ty);
  if (abs(p.x - tx) <= s && abs(p.y - ty) <= s)
    return true;
  return false;
}

bool isArrive(Obj * obj) {
  float *a = (float *)obj->speed.path->val;
  int tx = a[1] * mapW;
  int ty = a[2] * mapH;
  if (isArrive(obj->pos, tx, ty, obj->speed.s)) {
    obj->pos.x = tx;
    obj->pos.y = ty;
    return true;
  }
  return false;
}

void setSpeed(Obj * obj,obj * tar) {
  Speed *speed = &obj->speed;
  float *path = (float *)(speed->path->val);
  // printf("setSpeed path=%f,%f,%f\n",path[0],path[1],path[2]);
  switch ((int)path[0]) {
  case 0:{
      speed->x = path[1] * mapW;
      speed->y = path[2] * mapH;
      break;
    }
  case 1:{
      if (isArrive(obj->pos, path[1] * mapW, path[2] * mapH, speed->s)) {
        speed->S = 0;
        speed->path = speed->path->next;
        setSpeed(obj);
        return;
      }
      if (speed->S == 0) {
        float a =
          atan2((path[2] * mapH - obj->pos.y), (path[1] * mapW - obj->pos.x));
        // printf("%f\n",a/3.1416*180);
        speed->x = speed->s * cos(a);
        speed->y = speed->s * sin(a);
      }
      break;
    }
  case 2:{
      if (speed->S >= path[2]) {
        speed->S = 0;
        speed->path = speed->path->next;
        setSpeed(obj);
        return;
      }
      if (speed->S == 0) {
        speed->x = speed->s * cos(path[1]);
        speed->y = speed->s * sin(path[1]);
      }
      break;
    }
  case 3:{
      ListNode *tarPath = speed->path;
      if (speed->r == path[0]) {
        speed->r = 0;
        tarPath = tarPath->next;
      } else {
        for (int i = 0; i < path[1]; i++) {
          tarPath = tarPath->last;
        }
      }
      obj->speed.path = tarPath;
      speed->r++;
      setSpeed(obj);
      break;
    }
  case 4:{
      if (!isArrive(obj)) {
        float a =
          atan2((path[2] * mapH - obj->pos.y), (path[1] * mapW - obj->pos.x));
        speed->s = obj->move;
        // printf("%f\n",a/3.1416*180);
        speed->x = speed->s * cos(a);
        speed->y = speed->s * sin(a);
      } else if (speed->s != 0) {
        speed->S = 0;
        speed->s = 0;
        speed->x = 0;
        speed->y = 0;
      }
      break;
    }
  }
}