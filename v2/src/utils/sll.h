#ifndef SLL_H_
#define SLL_H_

#define _GNU_SOURCE
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "./strings.h"

// inspired from http://www.martinbroadhurst.com/singly-linked-list-in-c.html

struct node_t {
	char   *data;
	uint8_t idx;
	struct  node_t *next;
};

struct list_t {
	struct node_t *head;
	struct node_t *tail;
	unsigned int cnt;
};

struct node_t *create_node(const char *data);
struct list_t *create_list(void);
unsigned int get_cnt_list(const struct list_t *list);

void add_head_list(struct list_t *list, const char *data);
void remove_head_list(struct list_t *list);

void add_tail_list(struct list_t *list, const char *data);
void remove_tail_list(struct list_t *list);

void delete_list(struct list_t *list);
void empty_list(struct list_t *list);
void remove_list(struct list_t *list, struct node_t *node, struct node_t *prev);

#endif /* SLL_H_ */