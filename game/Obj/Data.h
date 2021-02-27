#ifndef _DATA_H
#define _DATA_H
#define MAX_WEA 5
#define MAX_STEP 20


extern const int objCount;
extern const int stepCount;

// type,hp,pow,spe,w,h,fuel
extern int objData[][7];

extern int objWeaData[][MAX_WEA];

//最后一个是图片资源id
extern int objRectData[][5];

extern int stepData[][5][MAX_WEA];

extern int planData[][MAX_STEP];

extern int planDataSizes[];
//静态
extern int moveSta[];

extern int attSta[];

int isStatic(int m,int a);

int isStatic1();

void dataInit(int w, int h, int fps);

#endif