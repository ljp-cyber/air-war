#include "Plan.h"

Plan* defPlans[10];

int mainp(){
	initPlayerPlan();
	char str[200];
	toStrPlan(str,defPlans[0]);
	printf(str);
}

void printPlan(Plan* plan){
	if(plan->tar!=NULL){
		printf("plan.tar::");
		printObj(plan->tar);
	}
	printf("plan.id=%i,stepsSize=%i::\n",plan->id,plan->stepsSize);
	for(int i=0;i<plan->stepsSize;i++){
		Step* s=plan->steps[i];
		if(s)printf("th=%i::mt=%i,at=%i->\n",i,s->move.type,s->att.type);
		else printf("err step null");
	}
}

void toStrPlan(char* str,Plan* plan){
	char tmp[20]="";
	if(plan->tar!=NULL){
		Obj* obj=plan->tar;
		sprintf(tmp,"tar.id=%i,",obj->id);
	}
	strcat(str,tmp);
	sprintf(tmp,"pid=%i,stepsSize=%i::",plan->id,plan->stepsSize);
	strcat(str,tmp);
	for(int i=0;i<plan->stepsSize;i++){
		Step* s=plan->steps[i];
		if(s){
			sprintf(tmp,"id=%i:mt=%i,at=%i -> ",i,s->move.type,s->att.type);
			strcat(str,tmp);
		}
		else{
			strcat(str,"err:step null -> ");
		}
	}
}

void toStrObj2(char* str,Obj* obj){
	char tmp[80]="";
	toStrObj(tmp,obj);
	strcat(str,tmp);
	if(obj->plan!=NULL){
	toStrPlan(tmp,obj->plan);
		strcat(str,tmp);
	}else {
		strcat(str,"plan is null");
	}
}

void printObj2(Obj* obj){
	printObj(obj);
	if(obj->plan!=NULL){
		printPlan(obj->plan);
	}
	else {
		printf("plan is null");
	}
}

void setPlan(Plan* plan,Obj* tar,Step** steps,int stepsSize){
	plan->tar=tar;
	plan->stepsSize=stepsSize;
	for(int i=0;i<stepsSize;i++){
		plan->steps[i]=steps[i];
	}
}
void setStep(Step* step,int data[5][MAX_WEA]){
	step->t=data[0][0];
	step->b=data[0][1];
	int* d1=data[1];
	setMove(&step->move,d1[0],d1[1],d1[2],d1[3],d1[4]);
	int* d2=data[2];
	setAtt(step,d2[0],d2[1],d2[2],d2[3],d2[4],data[3],data[4]);
}

void setState1(PlanState* p,int t){
	memset(p,0,sizeof(PlanState));
	p->moveTime=t;
	for(int i=0;i<MAX_WEA;i++){
		p->attTime[i]=t;
	}
}

void setState(PlanState* p,int t,int i,int m,int a[MAX_WEA]){
	p->t=t;
	p->i=i;
	p->moveTime=m;
	for(int i=0;i<MAX_WEA;i++){
		p->attTime[i]=a[i];
	}
}

void setMove(Move* move,int type,T_POS x,T_POS y,int r,int t){
	move->type=type;
	move->x=x;
	move->y=y;
	move->r=r*3.14/180;
	move->t=t;
}

void setAtt(Step* step,int type,T_POS x,T_POS y,int r,int c,int w[MAX_WEA],int ii[MAX_WEA]){
	step->att.type=type;
	step->att.x=x;
	step->att.y=y;
	step->att.r=r*3.14/180;
	step->att.c=c;
	for(int i=0;i<MAX_WEA;i++){
		step->w[i]=w[i];
		step->i[i]=ii[i];
	}
}

void initPlayerPlan(){
	Plan* plan=(Plan*)malloc(sizeof(Plan));
	plan->tar=NULL;
	Step* step=(Step*)malloc(sizeof(Step));
	plan->stepsSize=1;
	memset(step,0,sizeof(Step));
	int w[MAX_WEA]={1,0};
	int i[MAX_WEA]={1000,1000};
	setAtt(step,0,0,0,0,-1.57,w,i);
	setMove(&step->move,0,0,0,0,0);
	plan->steps[0]=step;
	//printPlan(plan);
	defPlans[0]=plan;
}

void initWeaponPlan(Obj* tar){
	Plan* weapon=(Plan*)malloc(sizeof(Plan));
	weapon->tar=tar;
	Step* step=(Step*)malloc(sizeof(Step));
	memset(step,0,sizeof(Step));
	setMove(&step->move,1,0,0,-1.57,-1);
	defPlans[1]=weapon;
}

void initDefPlan(Obj* player){
	initPlayerPlan();
	initWeaponPlan(player);
}

void addPlan(Obj * obj,int type, int curTime) {
	obj->plan=defPlans[type];
	int attTime[MAX_WEA]={curTime,curTime};
	PlanState* state=(PlanState*)malloc(sizeof(PlanState));
	setState(state,0,0,curTime,attTime);
	obj->planState=state;
	//printPlan(obj->plan);
}