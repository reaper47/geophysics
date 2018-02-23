#include "../../include/arrays.h"

double avg_arr(double *arr, int n)
{
	double sum = 0.0;
	for(int i = 0; i < n; i++)
		sum += arr[i];
	return sum/n;
}



double max_arr(double *arr, int n)
{
	double max = arr[0];
	for(int i = 0; i < n; i++) {
		if(max < arr[i])
			max = arr[i];
	}
	return max;
}



double std_arr(double *arr, double avg, int n)
{
	double sum = 0.0;
	for(int i = 0; i < n; i++)
		sum += (arr[i] - avg) * (arr[i] - avg);
	return sqrt(sum/n);
}

