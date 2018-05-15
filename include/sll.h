#ifndef SLL_H_
#define SLL_H_

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "strings.h"

struct node_t {
	char   *data;
	uint8_t idx;
	struct  node_t *next;
};

struct list_t {
	struct node_t *head;
	struct node_t *tail;
	unsigned int   count;
};

unsigned int   get_count_list  (const struct list_t *list);

struct node_t  *create_node   (const char *data);
struct list_t  *create_list   (void);

void    add_head_list     (struct list_t *list, const char *data);
void    remove_head_list  (struct list_t *list);

void    add_tail_list     (struct list_t *list, const char *data);
void    rm_tail_list      (struct list_t *list);

void    del_list      (struct list_t *list);
void    empty_list       (struct list_t *list);
void    rm_list      (struct list_t *list, struct node_t *node, struct node_t *prev);

#endif /* SLL_H_ */

