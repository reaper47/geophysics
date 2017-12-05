#include "../src/utils/sll.h"
#include "test.h"

#define NODE1_DATA "HELLO"
#define NODE2_DATA "ELLO"
#define NODE3_DATA "LLO"
#define NODE4_DATA "LO"
#define NODE5_DATA "O"

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
 * tests - create_node
 *
 */
MU_TEST(test_assert_create_node)
{
	const char *data_expected = node1_data;
	struct node_t *node = create_node(data_expected);
	
	mu_assert_string_eq(data_expected, node->data);
	mu_assert(node->next == NULL, "next should be null");
}



/*
 * tests - create_list
 *
 */
MU_TEST(test_assert_create_list)
{
	struct list_t *list = create_list();
	
	unsigned int cnt_expected = 0;
	
	mu_assert(list->head == NULL, "head should be null");
	mu_assert(list->tail == NULL, "tail should be null");
	mu_assert_uint_eq(cnt_expected, list->cnt);
}



/*
 * tests - add_head_list
 *
 */
MU_TEST(test_assert_add_head_list)
{
	struct list_t *list = create_list();
	
	unsigned int cnt_expected = 1;
	const char *data_expected = node1_data;
	
	add_head_list(list, data_expected);
	
	const char *data_actual = list->head->data;
	mu_assert_string_eq(data_expected, data_actual);
	mu_assert_uint_eq(cnt_expected, list->cnt);
}



/*
 * tests - get_cnt_list
 *
 */
MU_TEST(test_assert_get_cnt_list)
{
	struct list_t *list = create_list();
	
	add_head_list(list, node1_data);
	add_head_list(list, node1_data);
	add_head_list(list, node2_data);
	
	unsigned int cnt_expected = 3;
	unsigned int data_actual = get_cnt_list(list);

	mu_assert_uint_eq(cnt_expected, data_actual);
}



/*
 * tests - remove_head_list
 *
 */
MU_TEST(test_assert_remove_head_list)
{
	struct list_t *list = create_list();
	
	add_head_list(list, node1_data);
	add_head_list(list, node1_data);
	unsigned int cnt_expected = 2;
	
	remove_head_list(list);
	cnt_expected--;
	
	const char *data_expected = node1_data;
	const char *data_actual = list->head->data;
	mu_assert_string_eq(data_expected, data_actual);
	mu_assert_uint_eq(cnt_expected, list->cnt);

	remove_head_list(list);
	cnt_expected--;
	
	mu_assert(list->head == 0, "head should be null");
	mu_assert_uint_eq(0, list->cnt);
}


 
/*
 * tests - add_tail_list
 *
 */
MU_TEST(test_assert_add_tail_list)
{
	struct list_t *list = create_list();
	unsigned int cnt_expected = 1;
	const char *data_expected = node1_data;
	
	add_tail_list(list, data_expected);
	
	const char *data_actual = list->tail->data;
	mu_assert_string_eq(data_expected, data_actual);
	mu_assert_uint_eq(cnt_expected, list->cnt);
}



/*
 * tests - insert_before_list
 *
 */
MU_TEST(test_assert_insert_before_list)
{
	struct list_t *list = create_list();
	
	struct node_t *node1 = create_node(NODE1_DATA);
	struct node_t *node2 = create_node(NODE2_DATA);
	
	add_head_list(list, node1->data);
	add_tail_list(list, node2->data);
	
	insert_before_list(list, node2, node1, NODE3_DATA);
	
	unsigned int cnt_expected = 3;
	const char *data_expected = node3_data;
	
	mu_assert_uint_eq(cnt_expected, list->cnt);
	mu_assert_string_eq(data_expected, node1->next->data);
}



/*
 * tests - insert_after_list
 *
 */
MU_TEST(test_assert_insert_after_list)
{
	struct list_t *list = create_list();
	
	struct node_t *node1 = create_node(NODE1_DATA);
	struct node_t *node2 = create_node(NODE2_DATA);
	
	add_head_list(list, node1->data);
	add_tail_list(list, node2->data);
	
	insert_after_list(list, node2, node3_data);
	insert_after_list(list, node1, node4_data);
	insert_after_list(list, NULL, node5_data);
	
	unsigned int cnt_expected = 5;
	const char *data_expected = node3_data;
	const char *data2_expected = node4_data;
	const char *data3_expected = node5_data;
	
	mu_assert_uint_eq(cnt_expected, list->cnt);
	mu_assert_string_eq(data_expected, node2->next->data);
	mu_assert_string_eq(data2_expected, node1->next->data);
	mu_assert_string_eq(data3_expected, list->tail->data);
}



/*
 * tests - remove_tail_list
 *
 */
MU_TEST(test_assert_remove_tail_list)
{
	struct list_t *list = create_list();
	
	add_tail_list(list, node1_data);
	remove_tail_list(list);
	
	unsigned int cnt_expected = 0;
	mu_assert(list->tail == 0, "tail should be null");
	mu_assert_uint_eq(cnt_expected, list->cnt);
}



/*
 * tests - remove_list
 *
 */
MU_TEST(test_assert_remove_list)
{
	struct list_t *list = create_list();
	
	struct node_t *node1 = create_node(node1_data);
	struct node_t *node2 = create_node(node2_data);
	unsigned int cnt_expected = 2;
	
	add_head_list(list, node1->data);
	add_head_list(list, node2->data);
	
	remove_list(list, node2, node1);
	
	cnt_expected--;
	mu_assert_uint_eq(cnt_expected, list->cnt);
	
	const char *data_expected = node1_data;
	const char *data_actual = list->tail->data;
	mu_assert_string_eq(data_expected, data_actual);
}



/*
 * tests - empty_list
 *
 */
MU_TEST(test_assert_empty_list)
{
	struct list_t *list = create_list();
	add_tail_list(list, node1_data);
	unsigned int cnt_expected = 0;

	empty_list(list);
	
	mu_assert_uint_eq(cnt_expected, list->cnt);
}



MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
	MU_RUN_TEST(test_check_setup);
	MU_RUN_TEST(test_assert_create_node);
	MU_RUN_TEST(test_assert_create_list);
	MU_RUN_TEST(test_assert_get_cnt_list);
	
	MU_RUN_TEST(test_assert_add_head_list);
	MU_RUN_TEST(test_assert_remove_head_list);
	
	MU_RUN_TEST(test_assert_insert_before_list);
	MU_RUN_TEST(test_assert_insert_after_list);
	
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
