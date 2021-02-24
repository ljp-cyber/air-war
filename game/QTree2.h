#include <stdio.h>
#include <string.h>
#include "ListNode.h"
#include "PlaneType.h"
#include "PlaneGroup.h"
#include "MySDL_Rect.h"

#define QTreeType PlaneGroup*
#define SpoceType SDL_Rect*
// #define SpoceType int*

int maxQTreeDeep = 2;
int maxNeedGroup = 4;

typedef struct qTree {
  QTreeType val;
  SpoceType spoce;
  int deep;
  struct qTree *ul;
  struct qTree *ur;
  struct qTree *dl;
  struct qTree *dr;
  struct qTree *p;
} QTreeNode;

void QTree_Divied(QTreeNode * root);

void printQTree(QTreeNode * root) {
  if (!root)
    return;
  printf("---QTree---::deep=%i;", root->deep);
  printRect(root->spoce);
  printPG(root->val);
  printQTree(root->ul);
  printQTree(root->ur);
  printQTree(root->dl);
  printQTree(root->dr);
}

QTreeNode *QTree_NewNode(QTreeType val, SpoceType spoce, int deep) {
  QTreeNode *qTree = (QTreeNode *) malloc(sizeof(QTreeNode));
  memset(qTree, 0, sizeof(QTreeNode));
  qTree->val = val;
  qTree->deep = deep;
  qTree->spoce = spoce;
  QTree_Divied(qTree);
  return qTree;
}

QTreeNode *QTree_NewNode(SpoceType spoce, int deep) {
  return QTree_NewNode(PG_Creat(), spoce, deep);
}

SpoceType getChildSpoce(int i, SpoceType p) {
  switch (i) {
  case 0:
    return mySDL_NewRect(p->x, p->y, p->w / 2, p->h / 2);
  case 1:
    return mySDL_NewRect(p->x + p->w / 2, p->y, p->w / 2, p->h / 2);
  case 2:
    return mySDL_NewRect(p->x, p->y + p->h / 2, p->w / 2, p->h / 2);
  case 3:
    return mySDL_NewRect(p->x + p->w / 2, p->y + p->h / 2, p->w / 2, p->h / 2);
  }
  return NULL;
}

bool SpoceCheck2(Obj * obj, SpoceType spoce) {
  if (spoce->x <= obj->x + obj->w / 2 &&
      spoce->x + spoce->w >= obj->x - obj->w / 2 &&
      spoce->y <= obj->y + obj->h / 2 &&
      spoce->y + spoce->h >= obj->y - obj->h / 2)
    return true;
  return false;
}

bool SpoceCheck(Obj * obj, SpoceType spoce) {
  // printObj(obj);
  // printRect(spoce);
  if (spoce->x > obj->x - obj->w / 2)
    return false;
  if (spoce->y > obj->y - obj->h / 2)
    return false;
  if (spoce->x + spoce->w < obj->x + obj->w / 2)
    return false;
  if (spoce->y + spoce->h < obj->y + obj->h / 2)
    return false;
  return true;
}

void QTree_Divied(QTreeNode * root) {
  if (root->deep == maxQTreeDeep)
    return;
  QTreeType val = root->val;
  if (val->size <= maxNeedGroup)
    return;
  root->ul = QTree_NewNode(getChildSpoce(0, root->spoce), root->deep + 1);
  root->ur = QTree_NewNode(getChildSpoce(1, root->spoce), root->deep + 1);
  root->dl = QTree_NewNode(getChildSpoce(2, root->spoce), root->deep + 1);
  root->dr = QTree_NewNode(getChildSpoce(3, root->spoce), root->deep + 1);
  for (int g = 0; g < GROUP_SIZE; g++) {
    ListNode *curNode = val->groups[g];
    while (curNode != NULL) {
      Obj *obj = (Obj *) (curNode->val);
      if (SpoceCheck(obj, root->ul->spoce))
        PG_AddPlane(root->ul->val, obj);
      else if (SpoceCheck(obj, root->ur->spoce))
        PG_AddPlane(root->ur->val, obj);
      else if (SpoceCheck(obj, root->dl->spoce))
        PG_AddPlane(root->dl->val, obj);
      else if (SpoceCheck(obj, root->dr->spoce))
        PG_AddPlane(root->dr->val, obj);
      else {
        curNode = curNode->next;
        continue;
      }
      ListNode *delNode = curNode;
      curNode = curNode->next;
      PG_DelPlane(root->val, delNode);
    }
  }
  QTree_Divied(root->ul);
  QTree_Divied(root->ur);
  QTree_Divied(root->dl);
  QTree_Divied(root->dr);
}

void QTree_Move(QTreeNode * from, QTreeNode * to, ListNode * node) {
  Obj *obj = (Obj *) (node->val);
  PG_DelPlane(from->val, node);
  PG_AddPlane(to->val, obj);
}

void QTree_Modified(QTreeNode * root, ListNode * modNode) {
  Obj *obj = (Obj *) (modNode->val);
  if (!SpoceCheck(obj, root->spoce)) {
    if (root->p != NULL) {
      QTree_Move(root, root->p, modNode);
      QTree_Modified(root->p, modNode);
      return;
    }
    if (!SpoceCheck2(obj, root->spoce)){
      return;
    }
  }
  if(root->ul==NULL)return;
  if (SpoceCheck(obj, root->ul->spoce)) {
  	QTree_Move(root, root->ul, modNode);
    QTree_Modified(root->ul, modNode);
    return;
  }
  if (SpoceCheck(obj, root->ur->spoce)) {
  	QTree_Move(root, root->ur, modNode);
    QTree_Modified(root->ur, modNode);
    return;
  }
  if (SpoceCheck(obj, root->dl->spoce)) {
  	QTree_Move(root, root->dl, modNode);
    QTree_Modified(root->dl, modNode);
    return;
  }
  if (SpoceCheck(obj, root->dr->spoce)) {
  	QTree_Move(root, root->dr, modNode);
    QTree_Modified(root->dr, modNode);
    return;
  }
}


int main() {
  printf("test1");
  SpoceType s = mySDL_NewRect(0, 0, 1200, 2000);
  QTreeType v = PG_Creat();
  for (int i = 0; i < 20; i++) {
    Obj *obj = newPlayer1();
    obj->x = i * 50;
    obj->y = i * 100;
    obj->vx = 2;
    obj->vy = 4;
    PG_AddPlane(v, obj);
  }
  printf("test2\n");
  QTreeNode *root = QTree_NewNode(v, s, 0);
  printQTree(root);
  return 1;
}