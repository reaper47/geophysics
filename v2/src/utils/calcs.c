#include "calcs.h"

float avg_arrf(float *arr, int n)
{
	float sum = 0;
	for(int i = 0; i < n; i++)
		sum += arr[i];
	return sum/(float)n;
}


#include <stdio.h>
bool approx_eq(float a, float b, float epsilon)
{
	double fabsa = fabs(a), fabsb = fabs(b);

	return fabs(a-b) <= ((fabsa < fabsb ? fabsb : fabsa) * epsilon);
}


#include <stdio.h>
float std_arrf(float *arr, float avg, int n)
{
	float sum = 0.0f;
	for(int i = 0; i < n; i++)
		sum += (arr[i] - avg) * (arr[i] - avg);
	return (float)(sqrt(sum/(float)n));
}

