#ifndef CALCS_H_
#define CALCS_H_

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define EPSILON 1e-7

bool approx_eq(double a, double b, double epsilon);
double avg_arrf(double *arr, int n);
double* interpolate_pts(double start_value, double end_value, int steps);
double std_arrf(double *arr, double avg, int n);

#endif /* CALCS_H_ */
