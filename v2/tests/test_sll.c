#include "../include/sll.h"
#include "test.h"

#define NODE1_DATA "hello"
#define NODE2_DATA "ello"
#define NODE3_DATA "llo"
#define NODE4_DATA "lo"
#define NODE5_DATA "o"

const char *node1_data;
const char *node2_data;
const char *node3_data;
const char *node4_data;
const char *node5_data;

void test_setup(void)
{
	node1_data = NODE1_DATA;
	node2_data = NODE2_DATA;
	node3_data = NODE3_DATA;
	node4_data = NODE4_DATA;
	node5_data = NODE5_DATA;
}



void test_teardown(void)
{
}



/*
 * tests - setup
 *
 */
MU_TEST(test_check_setup)
{
	mu_check(strcmp(node1_data, NODE1_DATA) == 0);
	mu_check(strcmp(node2_data, NODE2_DATA) == 0);
	mu_check(strcmp(node3_data, NODE3_DATA) == 0);
	mu_check(strcmp(node4_data, NODE4_DATA) == 0);
	mu_check(strcmp(node5_data, NODE5_DATA) == 0);
}



/*
 * tests - CreateNode
 *
 */
MU_TEST(test_assert_create_node)
{
	const char *data_expected = node1_data;
	struct node_t *node = CreateNode(data_expected);
	
	mu_assert_string_eq(data_expected, node->data);
	mu_assert(node->next == NULL, "next should be null");
	
	free(node->data);
	free(node);
}



/*
 * tests - CreateList
 *
 */
MU_TEST(test_assert_create_list)
{
	struct list_t *list = CreateList();
	
	unsigned int count_expected = 0;
	
	mu_assert(list->head == NULL, "head should be null");
	mu_assert(list->tail == NULL, "tail should be null");
	mu_assert_uint_eq(count_expected, list->count);
	
	DeleteList(list);
}



/*
 * tests - AddHeadList
 *
 */
MU_TEST(test_assert_add_head_list)
{
	struct list_t *list = CreateList();
	
	unsigned int count_expected = 1;
	const char *data_expected = node1_data;
	
	AddHeadList(list, data_expected);
	
	const char *data_actual = list->head->data;
	mu_assert_string_eq(data_expected, data_actual);
	mu_assert_uint_eq(count_expected, list->count);
	
	DeleteList(list);
}



MU_TEST(test_assert_add_head_list2)
{
	struct list_t *list = CreateList();
	
	const char *line_actual = "hello,this,is,a,test";
	const char *line_expected[] = { "hello", "this", "is", "a", "test" };	
	const char delim = ',';
	
	char *tok, *str, *tofree;
	tofree = str = strdup(line_actual);
	
	while((tok = strsep(&str, &delim)))
		AddHeadList(list, tok);
	free(tofree);	
		
	struct node_t *node = list->head;
	int i = 0;
	while(node != NULL) {
		mu_assert_string_eq(line_expected[i], node->data);
		node = node->next;
		i++;
	}

	DeleteList(list);
}



/*
 * tests - GetCountList
 *
 */
MU_TEST(test_assert_get_cnt_list)
{
	struct list_t *list = CreateList();
	
	AddHeadList(list, node1_data);
	AddHeadList(list, node1_data);
	AddHeadList(list, node2_data);
	
	unsigned int count_expected = 3;
	unsigned int data_actual = GetCountList(list);

	mu_assert_uint_eq(count_expected, data_actual);
	
	DeleteList(list);
}



/*
 * tests - RemoveHeadList
 *
 */
MU_TEST(test_assert_remove_head_list)
{
	struct list_t *list = CreateList();
	
	AddHeadList(list, node1_data);
	AddHeadList(list, node1_data);
	unsigned int count_expected = 2;
	
	RemoveHeadList(list);
	count_expected--;
	
	const char *data_expected = node1_data;
	const char *data_actual = list->head->data;
	mu_assert_string_eq(data_expected, data_actual);
	mu_assert_uint_eq(count_expected, list->count);

	RemoveHeadList(list);
	count_expected--;
	
	mu_assert(list->head == 0, "head should be null");
	mu_assert_uint_eq(0, list->count);
	
	DeleteList(list);
}


 
/*
 * tests - AddTailList
 *
 */
MU_TEST(test_assert_add_tail_list)
{
	struct list_t *list = CreateList();
	
	unsigned int count_expected = 1;
	const char *data_expected = node1_data;
	
	AddTailList(list, data_expected);
	
	const char *data_actual = list->tail->data;
	mu_assert_string_eq(data_expected, data_actual);
	mu_assert_uint_eq(count_expected, list->count);
	
	DeleteList(list);
}



/*
 * tests - RemoveTailList
 *
 */
MU_TEST(test_assert_remove_tail_list)
{
	struct list_t *list = CreateList();
	
	AddTailList(list, node1_data);
	RemoveTailList(list);
	
	unsigned int count_expected = 0;
	mu_assert(list->tail == 0, "tail should be null");
	mu_assert_uint_eq(count_expected, list->count);
	
	DeleteList(list);
}



/*
 * tests - RemoveList
 *
 */
MU_TEST(test_assert_remove_list)
{
	struct list_t *list = CreateList();
	
	struct node_t *node1 = CreateNode(node1_data);
	struct node_t *node2 = CreateNode(node2_data);
	unsigned int count_expected = 2;
	
	AddHeadList(list, node1->data);
	AddHeadList(list, node2->data);
	
	free(node2->data);
	RemoveList(list, node2, node1);
	
	count_expected--;
	mu_assert_uint_eq(count_expected, list->count);
	
	const char *data_expected = node1_data;
	const char *data_actual = list->tail->data;
	mu_assert_string_eq(data_expected, data_actual);
	
	free(node1->data);
	free(node1);
	
	DeleteList(list);
}



/*
 * tests - EmptyList
 *
 */
MU_TEST(test_assert_empty_list)
{
	struct list_t *list = CreateList();
	
	AddTailList(list, node1_data);
	unsigned int count_expected = 0;

	EmptyList(list);
	mu_assert_uint_eq(count_expected, list->count);
	
	DeleteList(list);
}



MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
	MU_RUN_TEST(test_check_setup);
	MU_RUN_TEST(test_assert_create_node);
	MU_RUN_TEST(test_assert_create_list);
	MU_RUN_TEST(test_assert_get_cnt_list);
	
	MU_RUN_TEST(test_assert_add_head_list);
	MU_RUN_TEST(test_assert_add_head_list2);
	MU_RUN_TEST(test_assert_remove_head_list);
	
	MU_RUN_TEST(test_assert_add_tail_list);
	MU_RUN_TEST(test_assert_remove_tail_list);
	
	MU_RUN_TEST(test_assert_empty_list);
	MU_RUN_TEST(test_assert_remove_list);
}



int main(void)
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return 0;
}

