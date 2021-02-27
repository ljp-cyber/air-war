
#include "ObjFactory.h"

#define CACHE_SIZE 200

static int id=0;
static int pid=0;

static Obj* objs[CACHE_SIZE]={0};
static int objsLen=0;

static Plan* plans[CACHE_SIZE]={0};
static int plansLen=0;

static Step* steps1[CACHE_SIZE]={0};
static int steps1Len;//静态的

static Step* steps[CACHE_SIZE]={0};
static int stepsLen=0;

static PlanState* states[CACHE_SIZE]={0};
static int statesLen=0;

static void setPlan2(Plan* plan,Obj* tar,int* data,int size);

static Step* getStep(int id);
static Plan* getPlan(Obj* tar,int id);
static PlanState* getState(int time);
static void scrapStep(Step* step);
static void scrapPlan(Plan* plan);
static void scrapState(PlanState* state);

int mainof(){
	objFtyInit();
	Obj* obj=getObj(0,1,0,NULL);
	printf("%p,",obj);
	printObj2(obj);
	obj=getObj(0,0,0,NULL);
	printf("%p,",obj);
	printObj2(obj);
	scrapObj(obj);
	obj=getObj(0,1,0,NULL);
	printf("%p",obj);
	printObj2(obj);
	
}

void toStrFac(char* str){
	sprintf(str,"fac[ol:%i,pl:%i,sl=%i]",objsLen,plansLen,statesLen);
}

void objFtyInit(int w,int h,int fps){
	steps1Len=stepCount;//静态的
	dataInit(w,h,fps);
	for(int i=0;i<steps1Len;i++){
		if(isStatic1(i)){
			Step* step=(Step*)malloc(sizeof(Step));
			setStep(step,stepData[i]);
			steps1[i]=step;
		}
	}
}

Obj* getObj(int type,int planType,int time,Obj* par){
	Obj* obj=NULL;
	printf("get obj len=%i,type=%i,ptype=%i\n",objsLen,type,planType);
	if(objsLen){
		obj=objs[--objsLen];
	}else{
		obj = (Obj *) malloc(sizeof(Obj));
	}
	setObj1(obj,objData[type],objWeaData[type]);
	if(par!=NULL){
		obj->x=par->x;
		obj->y=par->y;
		obj->g=par->g;
	}
	if(planType!=-1){
		Obj* tar=NULL;
		Obj** partar=NULL;
		if(par!=NULL){
			obj->x=par->x;
			obj->y=par->y;
			obj->g=par->g;
			if(par->plan!=NULL){
				printf("par not null\n");//
				Plan* plan = (Plan*)par->plan;
				tar=plan->tar;
				partar=&plan->tar;
			}
		}
		Plan* plan=getPlan(tar,planType);
		printPlan(plan);//
		PlanState* state=getState(time);
		if(plan->x&&plan->y){
			obj->x=plan->x;
			obj->y=plan->y;
		}
		plan->partar=partar;
		state->move=plan->steps[state->i]->move;
		state->att=plan->steps[state->i]->att;
		obj->plan=plan;
		
		obj->planState=state;
		obj->spe=obj->maxSpe;
	}
	obj->id=id++;
	printf("end get");//
	printObj2(obj);//
	return obj;
}

void scrapObj(Obj* obj){
	if(obj==NULL)return;
	if(obj->plan)scrapPlan(obj->plan);
	if(obj->planState)scrapState(obj->planState);
	if(objsLen>=CACHE_SIZE){
		free(obj);
	}else{
		objs[objsLen++]=obj;
	}
}

void objFtyDesrory(){
	for(int i=0;i<objsLen;i++){
		free(objs[i]);
	}
	for(int i=0;i<plansLen;i++){
		free(plans[i]);
	}
	for(int i=0;i<statesLen;i++){
		free(states[i]);
	}
	for(int i=0;i<steps1Len;i++){
		if(steps1[i])free(steps1[i]);
	}
}

static void setPlan2(Plan* plan,Obj* tar,int* data,int size){
	plan->tar=tar;
	plan->partar=NULL;
	plan->stepsSize=size;
	plan->x=stepData[data[0]][0][2];
	plan->y=stepData[data[0]][0][3];
	for(int i=0;i<size;i++){
		Step* step=getStep(data[i]);
		plan->steps[i]=step;
	}
}

static Step* getStep(int id){
	if(isStatic1(id)){
		return steps1[id];
	}
	Step* step=NULL;
	if(stepsLen){
		step=steps[--stepsLen];
	}else{
		step=(Step*)malloc(sizeof(Step));
	}
	setStep(step,stepData[id]);
	return step;
}

static Plan* getPlan(Obj* tar,int id){
	Plan* plan=NULL;
	if(plansLen){
		plan=plans[--plansLen];
	}else{
		plan=(Plan*)malloc(sizeof(Plan));
	}
	setPlan2(plan,tar,planData[id],planDataSizes[id]);
	plan->id=pid++;
	return plan;
}

static PlanState* getState(int time){
	PlanState* state=NULL;
	if(statesLen){
		state=states[--statesLen];
	}else{
		state=(PlanState*)malloc(sizeof(PlanState));
	}
	setState1(state,time);
	return state;
}
static void scrapStep(Step* step){
	if(step==NULL)return;
	if(isStatic(step->move.type,step->att.type)){
		return;
	}
	if(stepsLen>=CACHE_SIZE){
		free(step);
	}else{
		steps[stepsLen++]=step;
	}
}

static void scrapPlan(Plan* plan){
	for(int i=0;i<plan->stepsSize;i++){
		scrapStep(plan->steps[i]);
		plan->steps[i]=NULL;
	}
	if(plansLen>=CACHE_SIZE){
		free(plan);
	}else{
		plans[plansLen++]=plan;
	}
}

static void scrapState(PlanState* state){
	if(statesLen>=CACHE_SIZE){
		free(state);
	}else{
		states[statesLen++]=state;
	}
}