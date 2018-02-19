#include "../../include/sll.h"

struct node_t *CreateNode(const char *data)
{
	struct node_t *node = malloc(sizeof(struct node_t));
	
	if(node) {	
		node->data = strdup((char*)data);
		node->idx = 128;
		node->next = NULL;
	}
	
	return node;
}



struct list_t *CreateList(void)
{
	struct list_t *list = malloc(sizeof(struct list_t));
	
	if(list) {
		list->head = NULL;
		list->tail = NULL;
		list->count  = 0;
	}
	
	return list;
}



unsigned int GetCountList(const struct list_t *list)
{
	return list->count;
}



void AddHeadList(struct list_t *list, const char *data)
{
	struct node_t *node = CreateNode(data);
	
	if(list->tail == NULL) {
		list->head = node;
		list->tail = node;
	} else {
		list->tail->next = node;
		list->tail = node;
	}
	
	list->count++;
}



void RemoveHeadList(struct list_t *list)
{
	if(list->head) {
		struct node_t *tmp = list->head;
		
		if(list->head->next)
			list->head = list->head->next;
		else {
			list->head = NULL;
			list->tail = NULL;
		}
		free(tmp->data);
		free(tmp);
		
		list->count--;
		if(list->count == 1)
			list->tail = list->head;	
	}
}



void AddTailList(struct list_t *list, const char *data)
{
	struct node_t *node = CreateNode(data);
	
	if(list->head == NULL) {
		list->head = node;
		list->tail = node;
	} else {
		list->tail->next = node;
		list->tail = node;
	}
	
	list->count++;
}



void RemoveTailList(struct list_t *list)
{
	if(list->tail) {
		struct node_t *curr, *prev = NULL;
		
		curr = list->head;
		while(curr->next) {
			prev = curr;
			curr = curr->next;
		}
		free(curr->data);
		free(curr);
		
		if(prev)
			prev->next = NULL;
		else {
			list->head = NULL;
			list->tail = NULL;
		}
		
		list->count--;
		if(list->count == 1)
			list->head = list->tail;
	}
}



void EmptyList(struct list_t *list)
{
	struct node_t *node, *tmp;
	
	node = list->head;
	while(node != NULL) {
		tmp = node->next;
		free(node->data);
		free(node);
		node = tmp;
		list->count--;
	}
}



void DeleteList(struct list_t *list)
{
	if(list->count == 0)
		free(list);
	else {
		EmptyList(list);
		free(list);
		list = NULL;
	}
}



void RemoveList(struct list_t *list, struct node_t *node, struct node_t *prev)
{
	prev->next = node->next;
	
	list->count--;
	if(list->count == 1)
		list->tail = list->head;
	else if(node == list->tail)
		list->tail = prev;

	free(node);
}

