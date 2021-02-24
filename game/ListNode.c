#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _LISTNODE_H
#define _LISTNODE_H

#define NodeType void*

static void lslog(char* str){
	printf(str);
	//drawText(str);
	//fprint(str);
	//SDL_Log(str);
}

typedef struct listNode {
  NodeType val;
  struct listNode *last;
  struct listNode *next;
} ListNode;

void toStrList(char* str,ListNode* head,void (*fuc)(char*,void*)){
	if(!fuc){
		char err[]="list to string err:\n";
		strcpy(str,err);
		return;
	}
	char res[4000]="";
	while (head!=NULL){
		if (head->val!=NULL){
			char tmp[200]="";
			(*fuc)(tmp,head->val);
			strcat(res,tmp);
		}
		head=head->next;
	}
	sprintf(str,res);
}

ListNode *List_NewNode(NodeType val) {
  ListNode *result = (ListNode *) malloc(sizeof(ListNode));
  if(!result){
  	lslog("ls malloc err");
  }
  memset(result, 0, sizeof(ListNode));
  result->val = val;
  return result;
}

int List_Size(ListNode *head){
	int res=0;
	while(head!=NULL){
		head=head->next;
		res++;
	}
	return res;
}

//头插对象，并返回头
ListNode* List_AddVal(ListNode ** head, NodeType val) {
  ListNode *newNode = List_NewNode(val);
  newNode->next = *head;
  if (*head != NULL)
    (*head)->last = newNode;
  *head = newNode;
  return newNode;
}

//尾插对象
void List_AddNode2(ListNode ** head, NodeType val) {
  ListNode *newNode = List_NewNode(val);
  if (*head == NULL){
    *head = newNode;
    return;
  }
  ListNode* endNode=*head;
  while(endNode->next!=NULL)
    endNode = endNode->next;
  endNode->next = newNode;
  newNode->last = endNode;
}

//头插node，并返回头
ListNode* List_AddNode(ListNode ** head, ListNode* add) {
	if (!head||!add){
		char err[]="list add err:\n";
		lslog(err);
		return NULL;
	}
	add->next=*head;
  add->last=NULL;
	if (*head != NULL){
		(*head)->last=add;
  }
  *head=add;
  return add;
}


//检测头有没有包含节点
int List_Check(ListNode* head, ListNode* check){
	if(!head||!check){
		return 0;
	}
	while(head){
		if(head==check){
			return 1;
		}
		head=head->next;
	}
	return 0;
}

//切断节点但是不free，返回切断的节点
ListNode* List_CutNode(ListNode ** head, ListNode* cut){
	if(!head||!List_Check(*head,cut)){
		char err[]="list cut err:null or head has not cut\n";
		lslog(err);
		return NULL;
	}
	if (*head==cut){
    *head = (*head)->next;
    if (*head!=NULL)(*head)->last = NULL;
	}else{
    cut->last->next = cut->next;
    if (cut->next != NULL) {
      cut->next->last = cut->last;
    }
	}
	cut->next=NULL;
	cut->last=NULL;
	return cut;
}

//删除节点，切断节点并free
int List_DelNode(ListNode ** head, ListNode * del) {
	if(!head||!List_Check(*head,del)){
		char err[]="list del err:null or head has not del\n";
		lslog(err);
		return 0;
	}
	List_CutNode(head,del);
  free(del);
  return 1;
}

void printList(ListNode* head,void (*fuc)(void*)){
	//printf("listnow\n");
	while (head!=NULL){
		if (head->val!=NULL){
			//printf("listnow2\n");
			fuc(head->val);
		}
		head=head->next;
	}
}

void printList1(ListNode* head){
	while (head!=NULL){
		printf("%p->",head->val);
		head=head->next;
	}
}

//不free val，
void List_Free(ListNode * head) {
	ListNode* temp;
	while ( head!=NULL ){
		temp=head->next;
		/*if(head->val){
			free(head->val);
		}*/
		free(head);
		head=temp;
	}
}

int mainls(){
	ListNode* head=NULL;
	for(int i=0;i<10;i++){
		int p=i;
		List_AddNode2(&head,&p);
	}
	for(int i=0;i<5;i++){
		List_DelNode(&head,head);
		printf("\n");
		printList1(head);
	}
		List_DelNode(&head,head->next);
		printf("\n");
		printList1(head);
		List_DelNode(&head,head->next);
		printf("\n");
		printList1(head);
		List_DelNode(&head,head->next->next);
		printf("\n");
		printList1(head);
		List_DelNode(&head,head->next);
		printf("\n");
		printList1(head);
		List_DelNode(&head,head);
		printf("\n");
		printList1(head);
		List_DelNode(&head,head);
		printf("\n");
		printList1(head);
}

#endif