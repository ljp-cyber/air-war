#include "SDL2/SDL.h"
#include "SDL2/SDL_thread.h"
#include <stdarg.h>

float X = 0.5, Y = 0.8;
SDL_Thread *th = NULL;
SDL_Event event;

typedef struct
{
	int id;
	char name[10];
	SDL_Rect rect;
	void (*func) (void);
} Button;

Button *btns[10] = { 0 };

void drawBtns()
{
	for (int i = 0; i < 10; i++)
	{
		if (btns[i])
		{
			mySDL_DrawRect(btns[i]->rect);
			drawTex1(btns[i]->rect, btns[i]->name);
		}
	}
}

void addBtn(int id, char *name, SDL_Rect rect, void (*func) (void))
{
	Button *b = (Button *) malloc(sizeof(Button));
	strcpy(b->name, name);
	b->id = id;
	b->rect = rect;
	b->func = func;
	btns[id] = b;
}

void delBtn(int id)
{
	Button *btn = btns[id];
	if (btn)
	{
		btns[id] = NULL;
		free(btn);
	}
}

void cleanBtns()
{
	for (int i = 0; i < 10; i++)
	{
		delBtn(i);
	}
}

int getTouch(void *data)
{
	while (1)
	{
		while (SDL_PollEvent(&event))
		{
			int click = 0;
			// 当事件为触屏
			if (event.type == SDL_FINGERDOWN)
			{
				// 获取x,y的坐标
				int x = event.tfinger.x * 1080;
				int y = event.tfinger.y * 2160;
				// drawText_("click:%i,%i\n",x,y);
				for (int i = 0; i < 10; i++)
				{
					if (btns[i])
					{
						SDL_Rect rect = btns[i]->rect;
						if (x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h)
						{
							// drawText("btn on\n");
							click = 1;
							btns[i]->func();
							break;
						}
					}
				}
				if (!click)
				{
					X = event.tfinger.x;
					Y = event.tfinger.y;
				}
				break;
			}
			else if (event.type == SDL_FINGERMOTION)
			{
				if (!click)
				{
					// 获取x,y的坐标
					X = event.tfinger.x;
					Y = event.tfinger.y;
				}
			}
		}
	}
}

void getPosBegin()
{
	th = SDL_CreateThread(getTouch, NULL, NULL);
	// if (th == NULL)drawTex("th creat lose");
}