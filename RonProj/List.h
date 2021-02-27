#ifndef LIST_H
#define LIST_H

#include "Node.h"

typedef struct {
	Node* head;
	int length;
} List;

List* initList();
void addToList(void* data, List* list);

void freeList(List* list, void deleter(void*));

#endif 