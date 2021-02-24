#include "Obj.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_STEP 20

typedef struct{
	int type;
	T_POS x;
	T_POS y;
	float r;//角度
	int t;//时间
}Move;

typedef struct{
	int type;
	T_POS x;
	T_POS y;
	float r;//角度
	int c;//连发
}Att;

typedef struct{
	int t;//重复次数
	int b;//回退步数
	Move move;
	Att att;
	int w[MAX_WEA];//打开武器
	int i[MAX_WEA];//攻击间隔
}Step;

typedef struct{
	int id;
	int x;
	int y;
	Obj* tar;
	Obj** partar;
	Step* steps[MAX_STEP];
	int stepsSize;
}Plan;

typedef struct{
	int t;//次数
	int i;//第几步
	Move move;
	Att att;
	int moveTime;
	int attTime[MAX_WEA];
}PlanState;

extern Plan* defPlans[10];
void toStrPlan(char* str,Plan* plan);
void toStrObj2(char* str,Obj* obj);
void printPlan(Plan* plan);
void printObj2(Obj* obj);
void setPlan(Plan* plan,Obj* tar,Step** steps,int stepSize);
void addPlan(Obj * obj,int type, int curTime);
void setStep(Step* step,int data[5][MAX_WEA]);
void setState1(PlanState* state,int t);
void setState(PlanState* state,int t,int i,int m,int a[MAX_WEA]);
void setMove(Move* move,int type,T_POS x,T_POS y,int r,int t);
void setAtt(Step* step,int type,T_POS x,T_POS y,int r,int c,int w[MAX_WEA],int ii[MAX_WEA]);

void initPlayerPlan();
void initWeaponPlan(Obj* tar);
void initDefPlan(Obj* player);