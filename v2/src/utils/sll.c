#include "sll.h"

struct node_t *create_node(const char *data)
{
	struct node_t *node = malloc(sizeof(struct node_t));
	if(node) {
		node->data = data;
		node->next = NULL;
	}
	
	return node;
}



struct list_t *create_list(void)
{
	struct list_t *list = malloc(sizeof(struct list_t));
	if(list) {
		list->head = NULL;
		list->tail = NULL;
		list->cnt  = 0;
	}
	
	return list;
}



unsigned int get_cnt_list(const struct list_t *list)
{
	return list->cnt;
}



void add_head_list(struct list_t *list, const char *data)
{
	struct node_t *node = create_node(data);
	
	if(list->tail == NULL) {
		list->head = node;
		list->tail = node;
	} else {
		list->tail->next = node;
		list->tail = node;
	}
	
	list->cnt++;
}



void insert_before_list(struct list_t *list, struct node_t *node, struct node_t *prev, const char *data)
{
	if(node == list->head)
		add_head_list(list, data);
	else {
		struct node_t *new_node = create_node(data);
		new_node->next = node;
		prev->next = new_node;
		list->cnt++;
	}
}



void insert_after_list(struct list_t *list, struct node_t *node, const char *data)
{
	struct node_t *new_node;
	if(node == NULL) {
		add_head_list(list, data);
		new_node = list->head;
	} else {
		new_node = create_node(data);\
		
		if(new_node) {
			new_node->next = node->next;
			node->next = new_node;
			
			if(node == list->tail)
				list->tail = new_node;
		}
		
		list->cnt++;
	}
}



void remove_head_list(struct list_t *list)
{
	if(list->head) {
		struct node_t *tmp = list->head;
		
		if(list->head->next)
			list->head = list->head->next;
		else {
			list->head = NULL;
			list->tail = NULL;
		}
		
		free(tmp);
		
		list->cnt--;
		if(list->cnt == 1)
			list->tail = list->head;	
	}
}



void add_tail_list(struct list_t *list, const char *data)
{
	struct node_t *node = create_node(data);
	
	if(list->head == NULL) {
		list->head = node;
		list->tail = node;
	} else {
		list->tail->next = node;
		list->tail = node;
	}
	
	list->cnt++;
}



void remove_tail_list(struct list_t *list)
{
	if(list->tail) {
		struct node_t *curr, *prev = NULL;
		
		curr = list->head;
		while(curr->next) {
			prev = curr;
			curr = curr->next;
		}
		free(curr);
		
		if(prev)
			prev->next = NULL;
		else {
			list->head = NULL;
			list->tail = NULL;
		}
		
		list->cnt--;
		if(list->cnt == 1)
			list->head = list->tail;
	}
}



void empty_list(struct list_t *list)
{
	struct node_t *node, *tmp;
	
	node = list->head;
	while(node != NULL) {
		tmp = node->next;
		free(node);
		node = tmp;
		list->cnt --;
	}
}



void delete_list(struct list_t *list)
{
	if(list) {
		empty_list(list);
		free(list);
		list = NULL;
	}
}



void remove_list(struct list_t *list, struct node_t *node, struct node_t *prev)
{
	prev->next = node->next;
	
	list->cnt--;
	if(list->cnt == 1)
		list->tail = list->head;
	else if(node == list->tail)
		list->tail = prev;

	free(node);
}
