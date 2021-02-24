// #include "../sdl/MySDL_CNS.h"
// #include "../fp.c"

#include <stdlib.h>
#include "QTree.h"
#include <math.h>
#include <time.h>

#define max(a,b) (a>b?a:b)
#define min(a,b) (a<b?a:b)

//qtree只是适合碰撞检测，主战场还是要另外的容器的
//主战场的容器应该是hashlinkedlist，实现追踪弹还是要根据id获取的
//主战场可以直接分组，方便获取不同类型的obj
QTreeNode *gameQTree;
Obj *player;

static Obj *aiPar;
static Move *pMove;

static int W, H, aTime;
static int curTime = 0;

void setFPS(int fps)
{
	aTime=1000/fps;
}

void playerMove(float x, float y)
{
	pMove->x = x * W;
	pMove->y = y * H;
}

void addSomeObj(Obj ** news)
{
	if (!news)
	{
		return;
	}
	int i;
	for (i = 0; news[i] != NULL; i++)
	{
		QTree_AddObj(gameQTree, news[i]);
		
	}
	free(news);
}

int setTar(Obj* obj){
	if(obj->g==0&&obj->type==0){
		((Plan*)player->plan)->tar=obj;
		return 1;
	}
	((Plan*)player->plan)->tar=NULL;
	return 0;
}

void setPlayerTar(){
	Plan* plan=player->plan;
	Obj* old=NULL;
	if(plan->tar){
		old=plan->tar;
	}
	if(old==NULL||old->state==4){
		QTree_Iterator(gameQTree,setTar);
	}
}

void setTestObj()
{
	player->g = 1;
	player->hp = 100000;
	player->f = 100000;
	aiPar->g = 0;
	aiPar->x = 0;
	aiPar->y = 0;
}

void newGame(int w, int h,int fps)
{
	W = w;
	H = h;
	setFPS(fps);
	objFtyInit(w, h, fps);
	SpoceType s = mySDL_NewRect(0, 0, W, H);
	QTreeType pg = PG_Creat();
	//fprint("newgame fty");
	player = getObj(0, 0, curTime, NULL);
	pMove = &((PlanState *) player->planState)->move;
	aiPar = getObj(0, 0, curTime, NULL);
	((Plan *) aiPar->plan)->tar = player;
  setTestObj();				// 
  
	for (int i = 0; i < 1; i++)
	{
		Obj *obj = getObj(0, 1, curTime, aiPar);
		PG_AddPlane(pg, obj);
	}
	PG_AddPlane(pg, player);
	
	gameQTree = QTree_NewNode(pg, s, 0, NULL);
  setPlayerTar();
}

int nextTime = 200;
void go()
{
	curTime+=aTime;
	if (curTime > nextTime)
	{
		nextTime = curTime + 1000;
		QTree_AddObj(gameQTree, getObj(0, 1, curTime, aiPar));
	}
}

int action(Obj * obj)
{
	if (obj->hp > 0 && obj->f > 0)
	{
		addSomeObj(doAttPlan(obj, curTime));
		setSpeed(obj, curTime);
		obj->x += obj->vx;
		obj->y += obj->vy;
		obj->f -= obj->spe;
		obj->s += obj->spe;
		obj->f--;
		if(obj->f<0)obj->state=4;
	}
	return 0;
}

void crashCheck(Obj * obj1, Obj * obj2)
{
	if(obj1->hp<0||obj2->hp<0)return;
	// if(SDL_HasIntersection(obj1,obj2))
	if (abs(obj1->x - obj2->x) <= (obj1->w + obj2->w) * 0.5
		&& abs(obj1->y - obj2->y) <= (obj1->h + obj2->h) * 0.5)
	{
		obj1->hp -= obj2->p;
		obj2->hp -= obj1->p;
		if(obj1->hp<=0)obj1->state=4;	
		if(obj2->hp<=0)obj2->state=4;
	}
}

int cleanHander(Obj * obj)
{
	if (obj->hp <= 0 || obj->f <= 0)
	{
		if (obj != player)
		{
			scrapObj(obj);
			return 1;
		}
	}
	return 0;
}

void doGame()
{
	go();
	QTree_Iterator(gameQTree, action);
	QTree_ModifiedAll(gameQTree);
	QTree_CrashCheck2(gameQTree, crashCheck);
	setPlayerTar();
	QTree_Clean(gameQTree, cleanHander);
	QTree_DeepControl(gameQTree);
}

int maingm()
{
	newGame(1080, 2160,60);
	srand((int)time(NULL));
	playerMove(rand() % 1000, rand() % 500 + 1600);
	printQTree(gameQTree);
	int time = 20;
	doGame(time += 20);
	printQTree(gameQTree);
	doGame(time);
	printQTree(gameQTree);
	while (gameQTree->allSize > 0)
	{
		doGame(time += 20);
		printQTree(gameQTree);
	}
}