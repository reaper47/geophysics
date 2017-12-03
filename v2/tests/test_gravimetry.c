#include "../src/gravimetry.h"
#include "test.h"


void test_setup(void)
{
	
}



void test_teardown(void)
{

}



/*
 * tests - alloc_worden807
 *
 */
MU_TEST()
{

}



/*
 * tests - free_worden807
 *
 */

 

/*
 * tests - load_grav_csv
 *
 */ 

 
 
MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
	MU_RUN_TEST();
}

 
int main(void)
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return 0;
}
