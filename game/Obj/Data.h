#ifndef MAX_WEA
#define MAX_WEA 5
#endif
#ifndef MAX_STEP
#define MAX_STEP 20
#endif

static int objCount = 3;
const static int stepCount = 7;

// type,hp,pow,spe,w,h,fuel
int objData[][7] = {
  {0, 50, 10, 10, 150, 100, 1000000},
  {1, 1, 1, 15, 50, 50, 2000},
  {2, 1, 2, 18, 30, 70, 3000}
};

int objWeaData[][MAX_WEA] = {
  {1, 2, -1, -1, -1},
  {-1, -1, -1, -1, -1},
  {-1, -1, -1, -1, -1}
};
//最后一个是图片资源id
int objRectData[][5] = {
  {245, 230, 135, 90, 1},
  {385, 85, 45, 45, 2},
  {280, 409, 45, 45, 2}
};

int stepData[][5][MAX_WEA] = {
	{// 0:玩家
  	{0, 0, 500, 1600, 0},             // 
  	{0, 500, 1600, 0, 0},        // move
  	{0, 0, 0, 270, 10},         // att
  	{0, 1, -1, -1, -1},             // wea on
  	{200, 250, 100, 100, 100}    // att间隔
	},
  {// 1:
  	{0, 0, 0, 200, 0},             // 
  	{1, 100, 400, 0, 0},        // move
  	{0, 0, 0, 90, 0},          // att
  	{0, 1, -1, -1, -1},             // wea on
  	{1000, 500, 0, 0, 0}           // att间隔
	},
  {// 2:
  	{0, 0, 0, 0, 0},             // 
  	{1, 900, 400, 0, 0},        // move
  	{0, 0, 0, 90, 0},          // att
  	{0, -1, -1, -1, -1},             // wea on
  	{1000, 0, 0, 0, 0}           // att间隔
	},
	{// 3:
  	{3, 2, 0, 0, 0},             // 
  	{2, 0, 0, 90, 1000},        // move
  	{0, 0, 0, 90, 0},          // att
  	{0, -1, -1, -1, -1},             // wea on
  	{1000, 0, 0, 0, 0}           // att间隔
	},
	{// 4:
  	{0, 0, 0, 0, 0},             // 
  	{-1, 0, 0, 0, 1000},        // move
  	{0, 0, 0, 90, 0},          // att
  	{0, -1, -1, -1, -1},             // wea on
  	{1000, 0, 0, 0, 0}           // att间隔
	},
	{// 5:
  	{0, 0, 0, 0, 0},             // 
  	{3, 0, 0, 270, 0},        // move
  	{-1, 0, 0, 0, 0},          // att
  	{0, -1, -1, -1, -1},             // wea on
  	{1000, 0, 0, 0, 0}           // att间隔
	},
  {// 6追踪:
  	{0, 0, 0, 0, 0},             // 
  	{4, 0, 0, 270, 0},        // move
  	{0, 0, 0, 0, 0},          // att
  	{-1, -1, -1, -1, -1},             // wea on
  	{0, 0, 0, 0, 0}           // att间隔
	}
};

int planData[][MAX_STEP]={
	{0},
	{1,2,3,4,5},
	{6}
};

int planDataSizes[]={
	1,4,1
};
//静态
int moveSta[]={
	1,2,3,4,-1
};

int attSta[]={
	-1,0
};

int isStatic(int m,int a){
	int res=0;
	for(int i=0;i<4;i++){
		if(m==moveSta[i]){
			res= 1;
			break;
		}
	}
	int r2=0;
  for(int i=0;i<2;i++){
		if(a==attSta[i]){
			r2=1;
			break;
		}
	}
	return res&&r2;
}

int isStatic1(int id){
	isStatic(stepData[id][1][0],stepData[id][2][0]);
}

void dataInit(int w, int h, int fps) {
  if (w != 1080 && h != 2160) {
    float wScale = w / 1080.0;
    float hScale = h / 2160.0;
    float fpsScale = 60.0 / fps;
    for (int i = 0; i < objCount; i++) {
      objData[i][3] *= wScale;
      objData[i][4] *= wScale;
      objData[i][5] *= hScale;
    }
    for (int i = 0; i < stepCount; i++) {
      stepData[i][2][1] *= wScale;
      stepData[i][2][2] *= hScale;
      stepData[i][3][1] *= wScale;
      stepData[i][3][2] *= hScale;
    }
    if (fps != 60) {
      float fpsScale = 60 / fps;
      for (int i = 0; i < objCount; i++) {
        objData[i][3] *= fpsScale;
      }
    }
  }
}