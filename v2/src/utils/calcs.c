#include "calcs.h"

double avg_arrf(double *arr, int n)
{
	double sum = 0.0;
	for(int i = 0; i < n; i++)
		sum += arr[i];
	return sum/n;
}



bool approx_eq(double a, double b, double epsilon)
{
	double fabsa = fabs(a), fabsb = fabs(b);
	
	return fabs(a-b) <= ((fabsa < fabsb ? fabsb : fabsa) * epsilon);
}



double std_arrf(double *arr, double avg, int n)
{
	double sum = 0.0;
	for(int i = 0; i < n; i++)
		sum += (arr[i] - avg) * (arr[i] - avg);
	return sqrt(sum/n);
}

