#include "calcs.h"

#include <stdio.h>

float avg_arr_f(float *arr, int n)
{
	float sum = 0;
	for(int i = 0; i < n; i++)
		sum += arr[i];
	return sum/(float)n;
}
