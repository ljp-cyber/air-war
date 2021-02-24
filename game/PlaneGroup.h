#include <stdio.h>
#include <string.h>
#include "ListNode.c"
#include "./obj/DoPlan.h"

#define GROUP_SIZE 2

static void mlog(char* str){
	printf(str);
	//SDL_Log(str);
	//drawText(str);
}

typedef struct
{
	ListNode *groups[GROUP_SIZE];
	int sizes[GROUP_SIZE];
	int size;
} PlaneGroup;

void toStrPG(char *str, PlaneGroup * pg)
{
	char res[4100] = "";
	char tmp[4000] = "";
	sprintf(tmp, "PlaneGroup::size=%i,s0=%i,s1=%i\n", pg->size, (pg->sizes)[0], (pg->sizes)[1]);
	strcat(res, tmp);

	sprintf(tmp, "g0::\n");
	strcat(res, tmp);

	toStrList(tmp, pg->groups[0], toStrObj);
	strcat(res, tmp);

	sprintf(tmp, "g1::\n");
	strcat(res, tmp);

	toStrList(tmp, pg->groups[1], toStrObj);
	strcat(res, tmp);

	sprintf(str, res);
}

void printPG(PlaneGroup * pg)
{
	printf("PlaneGroup::size=%i,s0=%i,s1=%i\n", pg->size, (pg->sizes)[0], (pg->sizes)[1]);
	printf("g0::\n");
	printList(pg->groups[0], printObj1);
	// printObj(pg->groups[0]->val);
	printf("g1::\n");
	printList(pg->groups[1], printObj1);
}

PlaneGroup *PG_Creat()
{
	PlaneGroup *result = (PlaneGroup *) malloc(sizeof(PlaneGroup));
	if(!result){
  	mlog("pg malloc err");
  }
	memset(result, 0, sizeof(PlaneGroup));
	return result;
}

//不会创建新的listnode
ListNode *PG_PutNode(PlaneGroup * pg, ListNode * put)
{
	if(!pg||!put||!put->val)
	{
		char err[]="pg put err:null point";
		mlog(err);
	}
	int g = ((Obj*)put->val)->g;
	ListNode *add = List_AddNode(&pg->groups[g], put);
	if(add!=put)
	{
		mlog("pg put err");
		return NULL;
	}
	pg->sizes[g]++;
	pg->size++;
	return add;
}

//创建新的listnode
ListNode *PG_AddPlane(PlaneGroup * pg, Obj * obj)
{
	if(!pg||!obj)
	{
		char err[]="pg add err:";
		mlog(err);
	}
	int g = obj->g;
	ListNode *add = List_AddVal(&pg->groups[g], obj);	
	if(!add)
	{
		mlog("pg add err");
		return NULL;
	}
	pg->sizes[g]++;
	pg->size++;
	return add;
}

//取出listnode,不删除listnode
ListNode* PG_TakeNode(PlaneGroup* pg, ListNode * take)
{
	if (!pg||!take||!take->val)
	{
		char err[]="pg take err:null point\n";
		mlog(err);
		return NULL;
	}
	int g = ((Obj *) (take->val))->g;
	ListNode* cut=List_CutNode(&pg->groups[g], take);
	if(cut!=take)
	{
		char err[]="pg take err:head has not delnode\n";
		mlog(err);
		return NULL;
	}
	pg->sizes[g]--;
	pg->size--;
	return cut;
}

//删除node ，不会删除obj，obj由主游戏控制
int PG_DelPlane(PlaneGroup * pg, ListNode * del)
{
	if (!del||!pg)
	{
		char err[]="pg del err:null point or val exist\n";
		mlog(err);
		return 0;
	}
	int g = ((Obj *) (del->val))->g;
	int isdel=List_DelNode(&pg->groups[g], del);
	if(!isdel)
	{
		char err[]="pg del err:head has not delnode\n";
		mlog(err);
		return 0;
	}
	pg->sizes[g]--;
	pg->size--;
	return 1;
}

void PG_Free(PlaneGroup * pg)
{
	for (int i = 0; i < GROUP_SIZE; i++)
	{
		List_Free(pg->groups[i]);
	}
	free(pg);
}

int mainpg()
{
	printf("testPlaneGroup\n");
	PlaneGroup *v = PG_Creat();
	for (int i = 0; i < 20; i++)
	{
		Obj *obj = newPlayer1();
		obj->g=i%2;
		obj->x = i * 50;
		obj->y = i * 100;
		obj->vx = 2;
		obj->vy = 4;
		obj->f = 10000;
		PG_AddPlane(v, obj);
	}
	printPG(v);
	
	printf("------------------------\n");
	printf("testPlaneGroup_del\n");
	Obj* del=v->groups[1]->val;
	PG_DelPlane(v, v->groups[1]);
	printObj(del);
	printPG(v);
	
	printf("------------------------\n");
	printf("testPlaneGroup_take\n");
	ListNode* take=
	PG_TakeNode(v, v->groups[1]->next);
	printPG(v);
	printf("%i\n",2);
	printList(take,&printObj1);
	//printObj(take->val);
	
	printf("------------------------\n");
	printf("testPlaneGroup_put\n");
	ListNode* put=
	PG_PutNode(v, take);
	printPG(v);
	printf("%i\n",2);
	printList(put,&printObj1);
	printObj(put->last->val);
	return 1;
}