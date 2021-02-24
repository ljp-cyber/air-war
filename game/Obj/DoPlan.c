#include "DoPlan.h"

static void dplog(char* str)
{
	printf(str);
}

Obj *newWeapon(Obj * par, int type)
{
	printf("newwea=%i\n", type);
	Obj *res = NULL;
	switch (type)
	{
	case 1:
		{
			res = newWeapon1();
			res->x = par->x;
			res->y = par->y;
			res->g = par->g;
			break;
		}
	}
	return res;
}

Obj **doAttPlan(Obj * obj, int curTime)
{
	if (obj->plan == NULL)
		return NULL;
	dplog("doAtt\n");
	Plan *plan = (Plan *) obj->plan;
	printObj2(obj);//
	PlanState *state = (PlanState *) obj->planState;
	Step *step = plan->steps[state->i];
	Att *att = &state->att;

	Obj **res = (Obj **) malloc(sizeof(Obj *) * 100);
	memset(res, 0, sizeof(Obj *) * 100);
	int reslen = 0;
	for (int i = 0; i < MAX_WEA; i++)
	{
		if (step->w[i]!=-1 && curTime > state->attTime[i] && obj->weapons[i] != -1)
		{
			state->attTime[i] = curTime + step->i[i];
			switch (step->w[i])
			{
			case 0:
				{
					//dplog("att case 0\n");
					for (int j = 0; j <= att->c; j++)
					{
						Obj *newObj = getObj(obj->weapons[i], -1, curTime, obj);
						newObj->spe = newObj->maxSpe;
						newObj->vx = newObj->spe * cos(att->r + 0.1 * j);
						newObj->vy = newObj->spe * sin(att->r + 0.1 * j);
						res[reslen++] = newObj;

						if (-j != j)
						{
							newObj = getObj(obj->weapons[i], -1, curTime, obj);
							newObj->spe = newObj->maxSpe;
							newObj->vx = newObj->spe * cos(att->r - 0.1 * j);
							newObj->vy = newObj->spe * sin(att->r - 0.1 * j);
							res[reslen++] = newObj;
						}
					}
					break;
				}
				case 1:{
					dplog("att case 1\n");
					Obj *newObj = getObj(obj->weapons[i], 2, curTime, obj);
					dplog("att res\n");
						newObj->spe = newObj->maxSpe;
						newObj->vx = newObj->spe * cos(att->r);
						newObj->vy = newObj->spe * sin(att->r);
						
						res[reslen++] = newObj;
						dplog("att 1 end\n");
				}
			}
		}
	}
	return res;
}


int isArrive(T_POS x, T_POS y, T_POS tx, T_POS ty, int s)
{
	// printf("isArr:%i,%i,%i,%i\n", p.x, p.y, tx, ty);
	if (abs(x - tx) <= s && abs(y - ty) <= s)
		return 1;
	return 0;
}

int isArrive1(Obj * obj)
{
	Plan *plan = (Plan *) obj->plan;
	PlanState *state = (PlanState *) obj->planState;
	Move *move = &state->move;
	int tx = move->x;
	int ty = move->y;			// *mapx
	if (isArrive(obj->x, obj->y, tx, ty, obj->spe))
	{
		obj->x = tx;
		obj->y = ty;
		return 1;
	}
	return 0;
}

void nextStep(Obj * obj, Plan * p, PlanState * s)
{
	s->i = (s->i + 1) % p->stepsSize;
	if (s->t < p->steps[s->i]->t)
	{
		s->i -= p->steps[s->i]->b;
		s->t++;
	}
	else
	{
		if (p->steps[s->i]->t != 0)
			s->t = 0;
	}
	s->move = p->steps[s->i]->move;
	// obj->spe=obj->maxSpe;
	s->att = p->steps[s->i]->att;
}

void setSpeed(Obj * obj, int curTime)
{
	if (obj->plan == NULL)
		return;
	dplog("doSpe\n");
	Plan *plan = (Plan *) obj->plan;
	// printObj2(obj);//
	PlanState *state = (PlanState *) obj->planState;
	// printf("step.i=%i\n",state->i);
	Step *step = plan->steps[state->i];
	Move *move = &state->move;
	switch (move->type)
	{
	case 0:
		{						// 玩家模式
			// printf("%i\n",0);
			if (!isArrive1(obj))
			{
				float a = atan2((move->y - obj->y), (move->x - obj->x));	// maph
				obj->spe = obj->maxSpe;
				// printf("%f\n",a/3.1416*180);
				obj->vx = obj->spe * cos(a);
				obj->vy = obj->spe * sin(a);
			}
			else if (obj->spe != 0)
			{
				obj->s = 0;
				obj->spe = 0;
				obj->vx = 0;
				obj->vy = 0;
			}
			break;
		}
	case 1:
		{						// 固定坐标移动
			// printf("%i\n",1);
			if (isArrive(obj->x, obj->y, move->x, move->y, obj->spe))
			{					// maph
				obj->s = 0;
				nextStep(obj, plan, state);
				setSpeed(obj, curTime);
				return;
			}
			if (obj->s == 0)
			{
				obj->spe = obj->maxSpe;
				float a = atan2((move->y - obj->y), (move->x - obj->x));	// maph
				// printf("%f\n",a/3.1416*180);
				obj->vx = obj->spe * cos(a);
				obj->vy = obj->spe * sin(a);
			}
			break;
		}
	case 2:
		{						// 固定方向移动
			// printf("%i\n",2);
			// if (obj->s >= 100) {
			if (obj->s == 0)
			{
				state->moveTime = curTime + move->t;
				obj->spe = obj->maxSpe;
				obj->vx = obj->spe * cos(move->r);
				obj->vy = obj->spe * sin(move->r);
			}
			else if (state->moveTime <= curTime)
			{
				obj->s = 0;
				nextStep(obj, plan, state);
				setSpeed(obj, curTime);
			}
			break;
		}
	case 3:					// 撤退模式
		{
			if (obj->s == 0)
			{
				obj->spe = obj->maxSpe;
				obj->vx = obj->spe * cos(move->r);
				obj->vy = obj->spe * sin(move->r);
				obj->state = 3;
			}
			break;
		}
	case 4:					// 追踪模式
		{
			Obj* partar=*plan->partar;
			if(partar){
				//&partar==plan->tar
			//drawText("zz");
			obj->spe = obj->maxSpe;
			float a = atan2((partar->y - obj->y), (partar->x - obj->x));
			obj->vx = obj->spe * cos(a);
			obj->vy = obj->spe * sin(a);
			}
			break;
		}
	case -1:					// 停止
		{
			// printf("%i\n",-1);
			if (obj->s == 0)
			{
				state->moveTime = curTime + move->t;
				obj->s++;
				obj->spe = 0;
				obj->vx = 0;
				obj->vy = 0;
			}
			else if (state->moveTime <= curTime)
			{
				obj->s = 0;
				nextStep(obj, plan, state);
				setSpeed(obj, curTime);
			}
			break;
		}
	}
	// printf("end do spe\n");
}