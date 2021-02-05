#include "Node.h"
#include <stdlib.h>
void* getNodeData(Node* node)
{
	if (!node)
		return NULL;

	return node->data;
}

Node* getNodeNext(Node* node)
{
	if (!node)
		return NULL;

	return node->next;
}

void setNodeData(Node* node, void* data)
{
	if (!node)
		return;

	 node->data = data;
}
void setNodeNext(Node* node, Node* next)
{
	if (!node)
		return;

	node->next = next;
}

Node* initNode(void* data, Node* next)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (!newNode)
		return NULL;

	setNodeData(newNode, data);
	setNodeNext(newNode, next);

	return newNode;
}

/* freeNode FREES data! */
void freeNode(Node* node)
{
	if (!node)
		return;

	free(node->data);
	free(node);
}
