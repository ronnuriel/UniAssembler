#ifndef NODE_H
#define NODE_H

typedef struct Node
{
	void* data;
	struct Node* next;
} Node;

void* getNodeData(Node* node);
Node* getNodeNext(Node* node);

void setNodeData(Node* node, void* data);
void setNodeNext(Node* node, Node *next);

Node* initNode(void* data, Node* next);
void freeNode(Node* node);

#endif