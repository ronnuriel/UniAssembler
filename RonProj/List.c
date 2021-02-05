#include "List.h"
#include <stdlib.h>


List* initList() {
	List* newList = (List*)malloc(sizeof(List));
	if (!newList)
		return NULL;
	newList->head = NULL;
	newList->length = 0;
	return newList;
 }
void addToList(void* data, List* list) {
	if (!list)
		return;

	if (!list->head)
	{
		list->head = initNode(data, NULL);
	}
	else
	{
		Node* t = list->head;
		while (t->next) {
			t = getNextNode(t);
		}

		setNodeNext(t, initNode(data, NULL));
	}

	list->length++;
}




void freeList(List* list) {

	if (!list) 
		return;

	Node* t = list->head;
	
	while (t)
	{
		Node* del = t;
		t = getNextNode(t);
		freeNode(del);
	}

	free(list);
}


