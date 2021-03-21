#ifndef LIST_H
#define LIST_H

#include "Node.h"

typedef struct {
	Node* head;
	int length;
} List;

List* initList();
int addToList(void* data, List* list); /* return 0 if allocation failed. 1 if successful */

void freeList(List* list, void deleter(void*));

#endif 