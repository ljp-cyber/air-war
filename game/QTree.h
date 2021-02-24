//#include "../sdl/MySDL_CNS.h"
#include "../fp.c"

#include <stdio.h>
#include <string.h>
#include "PlaneGroup.h"
#include "../sdl/MySDL_Rect.c"

#define QTreeType PlaneGroup*
#define SpoceType SDL_Rect*

static void qtlog(char* str){
	printf(str);
	//drawText(str);
	//fprint(str);
	//SDL_Log(str);
}

int maxQTreeDeep = 1;
int maxNeedGroup = 32;
int deepFreshInvl = 60;
int deepTime=0;

typedef struct qTree
{
	QTreeType val;
	int allSize;
	SpoceType spoce;
	int deep;
	struct qTree *ul;
	struct qTree *ur;
	struct qTree *dl;
	struct qTree *dr;
	struct qTree *p;
} QTreeNode;

void QTree_Divied(QTreeNode * root);

//历遍每个对象，不适合增加删除对象，需要传入处理函数
void QTree_Iterator(QTreeNode * root,int (*func)(Obj* obj))
{
	if (!root||!func)
		return;
	PlaneGroup *pg = root->val;
	for (int i = 0; i < GROUP_SIZE; i++)
	{
		ListNode *curNode = pg->groups[i];
		while (curNode != NULL)
		{
			if(func(curNode->val)){
				return;
			}
			curNode = curNode->next;
		}
	}
	QTree_Iterator(root->ul,func);
	QTree_Iterator(root->ur,func);
	QTree_Iterator(root->dl,func);
	QTree_Iterator(root->dr,func);
}

//历遍每个节点，增加删除对象，需要传入处理函数
void QTree_Iterator1(QTreeNode * root,int (*func)(QTreeNode*,ListNode*))
{
	if (!root||!func)
		return;
	PlaneGroup *pg = root->val;
	for (int i = 0; i < GROUP_SIZE; i++)
	{
		ListNode *cur = pg->groups[i];
		ListNode *next = NULL;
		while (cur != NULL)
		{
			next = cur->next;
			func(root,cur);
			cur = next;
		}
	}
	QTree_Iterator1(root->ul,func);
	QTree_Iterator1(root->ur,func);
	QTree_Iterator1(root->dl,func);
	QTree_Iterator1(root->dr,func);
}

void printQTree(QTreeNode * root)
{
	if (!root)
		return;
	printf("---QTree:deep=%i,allSize=%i,size=;", root->deep, root->allSize,root->val->size);
	printRect(root->spoce);
	printPG(root->val);
	printQTree(root->ul);
	printQTree(root->ur);
	printQTree(root->dl);
	printQTree(root->dr);
}

void toStrQTree1(char *str, QTreeNode * root)
{
	if (!root)
		return;
	char res[4200] = "";
	char tmp[4100] = "";
	sprintf(tmp, "---Qtree[deep=%i,allSize=%i,size=%i]\n", root->deep, root->allSize,root->val->size);
	strcat(res, tmp);
	toStrPG(tmp, root->val);
	strcat(res, tmp);
	strcat(str, res);
	toStrQTree1(str, root->ul);
	toStrQTree1(str, root->ur);
	toStrQTree1(str, root->dl);
	toStrQTree1(str, root->dr);
}

void toStrQTree(char *str, QTreeNode * root)
{
	if (!root)
		return;
	char tmp[50] = "";
	sprintf(tmp, "Qtree[deep=%i,allSize=%i,size=%i]", root->deep, root->allSize,root->val->size);
	strcat(str, tmp);
	/*toStrQTree(tmp,root->ul); 
	toStrQTree(str,root->ur);
	toStrQTree(str,root->dl);
	toStrQTree(str,root->dr);*/
}

QTreeNode *QTree_NewNode(QTreeType val, SpoceType spoce, int deep, QTreeNode * p)
{
	QTreeNode *qTree = (QTreeNode *) malloc(sizeof(QTreeNode));
	if(!qTree){
  	qtlog("qtree malloc err");
  }
	memset(qTree, 0, sizeof(QTreeNode));
	qTree->allSize = val->size;
	qTree->val = val;
	qTree->p = p;
	qTree->deep = deep;
	qTree->spoce = spoce;
	QTree_Divied(qTree);
	return qTree;
}

QTreeNode *QTree_NewNode1(SpoceType spoce, int deep, QTreeNode * p)
{
	return QTree_NewNode(PG_Creat(), spoce, deep, p);
}

QTreeNode *QTree_NewNode2(SpoceType spoce, int deep)
{
	return QTree_NewNode(PG_Creat(), spoce, deep, NULL);
}

SpoceType getChildSpoce(int i, SpoceType p)
{
	switch (i)
	{
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

// 部分在内
int SpoceCheck2(Obj * obj, SpoceType spoce)
{
	if (spoce->x <= obj->x + obj->w / 2 &&
		spoce->x + spoce->w >= obj->x - obj->w / 2 &&
		spoce->y <= obj->y + obj->h / 2 && spoce->y + spoce->h >= obj->y - obj->h / 2)
		return 1;
	return 0;
}

// 完全在内
int SpoceCheck(Obj * obj, SpoceType spoce)
{
	// printObj(obj);
	// printRect(spoce);
	if (spoce->x > obj->x - obj->w / 2)
		return 0;
	if (spoce->y > obj->y - obj->h / 2)
		return 0;
	if (spoce->x + spoce->w < obj->x + obj->w / 2)
		return 0;
	if (spoce->y + spoce->h < obj->y + obj->h / 2)
		return 0;
	return 1;
}

ListNode *QTree_Move(QTreeNode * from, QTreeNode * to, ListNode * node)
{
	PG_TakeNode(from->val, node);
	if (to->p == from){
		(to->allSize)++;
	}
	else if (from->p == to)
	{
		/* char temp[200]=""; toStrQTree(temp,from); toStrQTree(temp,to);
		   FDEBUG("%s",temp); */
		(from->allSize)--;
	}else{
		qtlog("qtree move err:");
		return NULL;
	}
	return PG_PutNode(to->val, node);
}

void QTree_Divied(QTreeNode * root)
{
	if (root->deep == maxQTreeDeep)
		return;
	QTreeType val = root->val;
	if (val->size <= maxNeedGroup)
		return;
	if (root->ul != NULL)
	{
		return;
	}
	root->ul = QTree_NewNode1(getChildSpoce(0, root->spoce), root->deep + 1, root);
	root->ur = QTree_NewNode1(getChildSpoce(1, root->spoce), root->deep + 1, root);
	root->dl = QTree_NewNode1(getChildSpoce(2, root->spoce), root->deep + 1, root);
	root->dr = QTree_NewNode1(getChildSpoce(3, root->spoce), root->deep + 1, root);
	for (int g = 0; g < GROUP_SIZE; g++)
	{
		ListNode *curNode = val->groups[g];
		ListNode *next=NULL;
		while (curNode != NULL)
		{
			next=curNode->next;
			Obj *obj = (Obj *) (curNode->val);
			if (SpoceCheck(obj, root->ul->spoce))
			{
				QTree_Move(root,root->ul,curNode);
			}
			else if (SpoceCheck(obj, root->ur->spoce))
			{
				QTree_Move(root,root->ur,curNode);
			}
			else if (SpoceCheck(obj, root->dl->spoce))
			{
				QTree_Move(root,root->dl,curNode);
			}
			else if (SpoceCheck(obj, root->dr->spoce))
			{
				QTree_Move(root,root->dr,curNode);
			}
			curNode = next;
		}
	}
	QTree_Divied(root->ul);
	QTree_Divied(root->ur);
	QTree_Divied(root->dl);
	QTree_Divied(root->dr);
}
//只分一层
void QTree_Divied1(QTreeNode * root)
{
	if (root->deep == maxQTreeDeep)
		return;
	QTreeType val = root->val;
	if (root->ul != NULL)
	{
		return;
	}
	root->ul = QTree_NewNode1(getChildSpoce(0, root->spoce), root->deep + 1, root);
	root->ur = QTree_NewNode1(getChildSpoce(1, root->spoce), root->deep + 1, root);
	root->dl = QTree_NewNode1(getChildSpoce(2, root->spoce), root->deep + 1, root);
	root->dr = QTree_NewNode1(getChildSpoce(3, root->spoce), root->deep + 1, root);
	for (int g = 0; g < GROUP_SIZE; g++)
	{
		ListNode *curNode = val->groups[g];
		ListNode *next=NULL;
		while (curNode != NULL)
		{
			next=curNode->next;
			Obj *obj = (Obj *) (curNode->val);
			if (SpoceCheck(obj, root->ul->spoce))
			{
				QTree_Move(root,root->ul,curNode);
			}
			else if (SpoceCheck(obj, root->ur->spoce))
			{
				QTree_Move(root,root->ur,curNode);
			}
			else if (SpoceCheck(obj, root->dl->spoce))
			{
				QTree_Move(root,root->dl,curNode);
			}
			else if (SpoceCheck(obj, root->dr->spoce))
			{
				QTree_Move(root,root->dr,curNode);
			}
			curNode = next;
		}
	}
}

// 只向下调整，适合加入新对象
void QTree_Modified2(QTreeNode * root, ListNode * modNode)
{
	if (!modNode->val)
	{
		qtlog("qtree mod2 err:null point\n");
		return;
	}
	Obj *obj = (Obj *) (modNode->val);
	if (root->deep == maxQTreeDeep)
		return;
	QTreeType val = root->val;
	if (val->size <= maxNeedGroup)
		return;
	if (root->ul != NULL)
	{
		if (SpoceCheck(obj, root->ul->spoce))
		{
			modNode = QTree_Move(root, root->ul, modNode);
			QTree_Modified2(root->ul, modNode);
			return;
		}
		if (SpoceCheck(obj, root->ur->spoce))
		{
			modNode = QTree_Move(root, root->ur, modNode);
			QTree_Modified2(root->ur, modNode);
			return;
		}
		if (SpoceCheck(obj, root->dl->spoce))
		{
			modNode = QTree_Move(root, root->dl, modNode);
			QTree_Modified2(root->dl, modNode);
			return;
		}
		if (SpoceCheck(obj, root->dr->spoce))
		{
			modNode = QTree_Move(root, root->dr, modNode);
			QTree_Modified2(root->dr, modNode);
			return;
		}
	}
	QTree_Divied(root);
}

// 先向上移，再向下移
void QTree_Modified(QTreeNode * root, ListNode * modNode)
{
	Obj *obj = (Obj *) (modNode->val);
	if (!SpoceCheck(obj, root->spoce))
	{
		if (root->p != NULL)
		{
			modNode = QTree_Move(root, root->p, modNode);
			QTree_Modified(root->p, modNode);
			return;
		}
		if (!SpoceCheck2(obj, root->spoce))
		{
			return;
		}
	}
	if (root->ul == NULL)
		return;
	if (SpoceCheck(obj, root->ul->spoce))
	{
		modNode = QTree_Move(root, root->ul, modNode);
		QTree_Modified(root->ul, modNode);
		return;
	}
	if (SpoceCheck(obj, root->ur->spoce))
	{
		modNode = QTree_Move(root, root->ur, modNode);
		QTree_Modified(root->ur, modNode);
		return;
	}
	if (SpoceCheck(obj, root->dl->spoce))
	{
		modNode = QTree_Move(root, root->dl, modNode);
		QTree_Modified(root->dl, modNode);
		return;
	}
	if (SpoceCheck(obj, root->dr->spoce))
	{
		modNode = QTree_Move(root, root->dr, modNode);
		QTree_Modified(root->dr, modNode);
		return;
	}
}

// 历遍所有对象，对每个对象调用Modified
void QTree_ModifiedAll(QTreeNode * root)
{
	if (root == NULL)
		return;
	PlaneGroup *pg = root->val;
	for (int i = 0; i < GROUP_SIZE; i++)
	{
		ListNode *curNode = pg->groups[i];
		ListNode *temp=NULL;
		while (curNode != NULL)
		{
			temp=curNode->next;
			//Obj *obj = (Obj *) (curNode->val);
			QTree_Modified(root, curNode);
			curNode = temp;
		}
	}
	QTree_ModifiedAll(root->ul);
	QTree_ModifiedAll(root->ur);
	QTree_ModifiedAll(root->dl);
	QTree_ModifiedAll(root->dr);
}

void QTree_AddObj(QTreeNode * root, Obj * obj)
{
	// drawText("qtree add obj\n");
	ListNode *add = PG_AddPlane(root->val, obj);
	root->allSize++;
	/* char tmp[10]=""; sprintf(tmp,"allsize=%i",root->allSize);
	   FDEBUG("%s",tmp); */
	QTree_Modified2(root, add);
}

void QTree_Free(QTreeNode * root)
{
	PG_Free(root->val);
	free(root->spoce);
	free(root);
}

void QTree_DelNode(QTreeNode * root)
{
	if (root == NULL)
		return;
	QTree_DelNode(root->ul);
	QTree_DelNode(root->ur);
	QTree_DelNode(root->dl);
	QTree_DelNode(root->dr);
	root->ul = NULL;
	root->ur = NULL;
	root->dl = NULL;
	root->dr = NULL;
	if (root->p == NULL)
	{
		// 这里没有释放obj
		QTree_Free(root);
		return;
	}
	PlaneGroup *val = root->val;
	for (int g = 0; g < GROUP_SIZE; g++)
	{
		ListNode *curNode = val->groups[g];
		while (curNode != NULL)
		{
			ListNode *nextNode = curNode->next;
			QTree_Move(root, root->p, curNode);
			curNode = nextNode;
		}
	}
	QTree_Free(root);
}

void QTree_DelChildren(QTreeNode * root){
	QTree_DelNode(root->ul);
	QTree_DelNode(root->ur);
	QTree_DelNode(root->dl);
	QTree_DelNode(root->dr);
	root->ul = NULL;
	root->ur = NULL;
	root->dl = NULL;
	root->dr = NULL;
}

void QTree_DeepControl(QTreeNode* root){
	if(!root||root->deep!=0){
		qtlog("qtree deep control err\n");
	}
	if (!deepTime++%deepFreshInvl)return;
	int shouldDeep=0;
	for(int i=root->allSize/maxNeedGroup;i!=0;i/=4){
		shouldDeep++;
	}
	shouldDeep=shouldDeep>maxQTreeDeep?maxQTreeDeep:shouldDeep;
	QTreeNode* queue[64]={0};
	int head=0;
	int tail=0;
	queue[head++]=root;
	while (head>tail){
		QTreeNode* cur=queue[tail++];
		if (cur->ul!=NULL && cur->deep==shouldDeep){
			//char info[100]="";
			//sprintf(info,"cur->deep=%i,deep=%i\n",cur->deep,shouldDeep); 
			//qtlog("qtree deep contrl delnode???\n");
			//qtlog(info);
			QTree_DelChildren(cur);
			//char str[6000]="";
			//toStrQTree1(str,root);
			//qtlog(str);
			//qtlog("qtree deep contrl delnode end???\n");
		}else if (cur->deep<shouldDeep){	
			if(!cur->ul){
				//qtlog("qtree deep contrl addnode???\n");
				//QTree_Divied1(cur);
			}else{
			queue[head++]=cur->ul;
			queue[head++]=cur->ur;
			queue[head++]=cur->dl;
			queue[head++]=cur->dr;
			}
		}else {
			//qtlog("qtree deep big???");
		}
	}
	//qtlog("qtree deep contrl end???\n");
}

//删除对象会历遍，如果清空qtreenode的话出错
void QTree_DelObj(QTreeNode * root, ListNode * delNode)
{
	/*Obj *delObj = (Obj *) delNode->val;
	if(!List_Check(root->val->groups[delObj->g],delNode)){
		qtlog("qt del err:\n");
		
		char err[100]="";
		toStrObj(err,delObj);
		qtlog(err);
		
		char tmp[10000]="";
		toStrQTree1(tmp,root);
		qtlog(tmp);
		return;
	}*/
	//drawText("qtree del obj\n");
	PG_DelPlane(root->val, delNode);
	// freeObj(delObj);
	while (root != NULL)
	{
		root->allSize--;
		/*if (root->allSize <= maxNeedGroup)
		{
			QTree_DelChildren(root);
		}*/
		root = root->p;
	}
}

//检测一个对象是否和其他对象碰撞，需要传入碰撞函数处理
void QTree_CrashCheck(QTreeNode * root, Obj * crashObj, void (*fuc) (Obj *, Obj *))
{
	if (root == NULL)
		return;
	PlaneGroup *pg = root->val;
	for (int i = 0; i < GROUP_SIZE; i++)
	{
		if (i == crashObj->g)
		{
			continue;
		}
		ListNode *curNode = pg->groups[i];
		while (curNode != NULL)
		{
			Obj *obj = (Obj *) (curNode->val);
			fuc(crashObj, obj);
			curNode = curNode->next;
		}
	}
	QTree_CrashCheck(root->ul, crashObj, fuc);
	QTree_CrashCheck(root->ur, crashObj, fuc);
	QTree_CrashCheck(root->dl, crashObj, fuc);
	QTree_CrashCheck(root->dr, crashObj, fuc);
}

//所有对象碰撞检测入口，历遍并调用QTree_CrashCheck，需要传入碰撞处理函数
void QTree_CrashCheck2(QTreeNode * root, void (*fuc) (Obj *, Obj *))
{
	if (root == NULL)
		return;
	PlaneGroup *pg = root->val;
	for (int i = 0; i < GROUP_SIZE; i++)
	{
		ListNode *curNode = pg->groups[i];
		while (curNode != NULL)
		{
			Obj *obj = (Obj *) (curNode->val);
			QTree_CrashCheck(root, obj, fuc);
			curNode = curNode->next;
		}
	}
	QTree_CrashCheck2(root->ul, fuc);
	QTree_CrashCheck2(root->ur, fuc);
	QTree_CrashCheck2(root->dl, fuc);
	QTree_CrashCheck2(root->dr, fuc);
}


//传入清理规则函数
void QTree_Clean(QTreeNode * root,int (*func)(Obj*))
{
	if (root == NULL)
		return;
	PlaneGroup *pg = root->val;
	for (int i = 0; i < GROUP_SIZE; i++)
	{
		ListNode *cur = pg->groups[i];
		ListNode *next = NULL;
		while (cur != NULL)
		{
			next = cur->next;
			if (!cur->val){
				qtlog("clean err:");
			}
			if (cur->val&&func(cur->val)){
				//qtlog("clean:");
				QTree_DelObj(root,cur);
			}
			cur = next;
		}
	}
	QTree_Clean(root->ul,func);
	QTree_Clean(root->ur,func);
	QTree_Clean(root->dl,func);
	QTree_Clean(root->dr,func);
}

int mainqt()
{
	printf("testQTree_Start\n");
	SpoceType s = mySDL_NewRect(0, 0, 1200, 2000);
	QTreeType v = PG_Creat();
	for (int i = 0; i < 20; i++)
	{
		Obj *obj = newPlayer1();
		obj->x = i * 100;
		obj->y = i * 200;
		obj->vx = 2;
		obj->vy = 4;
		// setPath(obj,path1);
		obj->f = 10000;
		PG_AddPlane(v, obj);
	}
	printf("-----------------------\n");
	printf("testQTree_creat\n");
	QTreeNode *root = QTree_NewNode(v, s, 0, NULL);
	printQTree(root);
	printf("-----------------------\n");
	printf("testQTree_add\n");
	Obj *obj = newPlayer1();
	obj->x = 100;
	obj->y = 200;
	QTree_AddObj(root, obj);
	printQTree(root);
	printf("-----------------------\n");
	printf("testQTree_del\n");
	QTree_DelObj(root->ul->ul, root->ul->ul->val->groups[0]);
	printQTree(root);
	printf("-----------------------\n");
	printf("testQTree_modified\n");
	Obj *o = (Obj *) root->ul->val->groups[0]->val;
	o->x = 800;
	o->y = 1500;
	// QTree_Modified(root->ul, root->ul->val->groups[0]);
	// printQTree(root);
	QTree_ModifiedAll(root);
	printQTree(root);
	return 1;
}