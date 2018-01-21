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



double correct_latitude(double lat, double lng, double pos)
{
	double lat_rad = lat * M_PI / 180;
	double lng_rad = lng * M_PI / 180;
	double mGal_per_m = pos * LAT_CORR_mGAL_PER_KM / 1000;

	return mGal_per_m * sin(2*lat_rad)*cos(lng_rad);
}



double* interpolate_pts(double start_value, double end_value, int steps)
{
	double *results = malloc(sizeof(double)*(size_t)steps);
		
	double step_size = (start_value - end_value) / (steps-1);
	double current_value = start_value + step_size;
	results[0] = start_value;

	for(int i = 1; i < steps; i++) {
		results[i] = current_value;
		current_value += step_size;
	}
	
	return results;
}



double std_arrf(double *arr, double avg, int n)
{
	double sum = 0.0;
	for(int i = 0; i < n; i++)
		sum += (arr[i] - avg) * (arr[i] - avg);
	return sqrt(sum/n);
}

