#ifndef _FP_H
#define _FP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FDEBUG(format,...) fprint("[%s]%s,func=%s,line=%d: %s\n",__TIME__,shortName(__FILE__),__FUNCTION__,__LINE__,##__VA_ARGS__)

static FILE* file = NULL;
char* fname="/storage/emulated/0/C4/ITc小程序/雷电/log.txt";

void fprint(char* ch,...);
void fprint_init();
void fprint_close();

char* shortName(char* path){
	char* res=strrchr(path,'/');
	if(res){
		res++;
		return res;
	}else{
		return path;
	}
}

void fprint_init(int flag){
	switch(flag){
		case 0:{
			file=fopen(fname,"a+");
			break;
		}
		case 1:{
			file=fopen(fname,"w");
			fclose(file);
			file=fopen(fname,"a+");
			break;
		}
	}
}

void fprint_close(){
	if(file){
		fclose(file);
	}
}

void fprintb(char* ch,...){
  fprint_init(0);
	char buf[10000];
	va_list al;
	va_start(al,ch);
	vsprintf(buf,ch,al);
	fprintf(file,buf);
	va_end(al);
  fprint_close();
}

void fprint(char* ch,...){
  fprint_init(0);
	char buf[100];
	va_list al;
	va_start(al,ch);
	vsprintf(buf,ch,al);
	fprintf(file,buf);
	va_end(al);
  fprint_close();
}

#endif