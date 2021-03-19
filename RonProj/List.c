#include "List.h"
#include <stdlib.h>
#include "SymbolList.h"

List* initList() {
	List* newList = (List*)malloc(sizeof(List));
	if (!newList)
		return NULL;

	newList->head = NULL;
	newList->length = 0;
	return newList;
 }
int addToList(void* data, List* list) {
	if (!list)
		return 0;

	if (!list->head)
	{
		list->head = initNode(data, NULL);
		if (!list->head)
		{
			return 0;
		}
		list->length++;
		return 1;
	}
	else
	{
		Node* t = list->head;
		
		while (t->next) {
			t = getNodeNext(t);
		}

		Node* newNode = initNode(data, NULL);
		if (!newNode)
		{
			return 0;
		}
		setNodeNext(t, newNode);
		list->length++;
		return 1;
	}

	
}




void freeList(List* list, void deleter(void *)) {

	printf("freeList\n");
	if (list)
	{
		Node* t = list->head;

		while (t)
		{
			Node* del = t;
			printCodeListRow(t->data);
			deleter(t->data);
			
			t = getNodeNext(t);
			freeNode(del);
		}

		free(list);
	}
}


