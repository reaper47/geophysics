#include "../../include/arrays.h"

int FindMaxArr(int *arr, int num_els)
{
	int max = arr[0];
	for(int i = 0; i < num_els; i++)
		if(arr[i] > max)
			max = arr[i];	
	return max;
}
