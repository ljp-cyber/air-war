#include "fp.c"
#include "game/obj/data.h"


char bgPath[] = "img/bg1_1.jpg";
char weaPath[] = "img/wea1_1.png";
char objPath[] = "img/obj5_1.png";

SDL_Texture* bgTex;
SDL_Texture* weaTex;
SDL_Texture* objTex;
SDL_Rect* bgRect;
SDL_Rect* weaRect;
SDL_Rect* objRect;
SDL_Rect** objRects;
int bgMove=0;

void initImg(){
	bgTex=loadImg(bgPath);
	weaTex=loadImg(weaPath);
	objTex=loadImg(objPath);
}

void gameSDL_init(){
	bgRect=mySDL_NewRect(0,1800,500,1000);
	weaRect=mySDL_NewRect(385,85,45,45);
	objRect=mySDL_NewRect(245,230,135,90);
	objRects=(SDL_Rect**)malloc(sizeof(void*)*objCount);
	for(int i=0;i<objCount;i++){
		objRects[i]=mySDL_NewRect(objRectData[i][0],
		objRectData[i][1],objRectData[i][2],objRectData[i][3]);
	}
	mySDL_Init();
	initImg();
}

void gameSDL_clear(){
	if(bgMove++%4==0){
		bgRect->y--;
		if(bgRect->y<1000){
			bgRect->y=1800;
	  }
	}
	//SDL_Clear();
	SDL_RenderCopy(ren, bgTex, bgRect, NULL);
	//SDL_RenderCopy(ren, bgTex, NULL, NULL);
	
}

void drawGame(QTreeNode * root) {
  if (root == NULL)
    return;
  PlaneGroup *pg = root->val;
  for (int i = 0; i < GROUP_SIZE; i++) {
    ListNode *curNode = pg->groups[i];
    while (curNode != NULL) {
      Obj *obj = (Obj *) (curNode->val);
      SDL_Rect rect={(int)obj->x-obj->w/2, (int)obj->y-obj->h/2, obj->w, obj->h};
      //mySDL_DrawRect( {
                     //(int)obj->x, (int)obj->y, 50, 50});
      switch(obj->type){
      	case 0:{       
      		SDL_RenderCopy(ren, objTex, objRect, &rect);
      		break;
      	}case 1:{       
        	SDL_RenderCopy(ren, weaTex, weaRect, &rect);
        	break;
      	}case 2:{       
        	SDL_RenderCopy(ren, weaTex, objRects[2], &rect);
      	}
      }  
      curNode = curNode->next;
    }
  }
  drawGame(root->ul);
  drawGame(root->ur);
  drawGame(root->dl);
  drawGame(root->dr);
}

void gameSDL_destroy(){
	SDL_DestroyTexture(bgTex);
	SDL_DestroyTexture(weaTex);
	SDL_DestroyTexture(objTex);
	mySDL_Destroy();
}