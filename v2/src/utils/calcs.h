#ifndef CALCS_H_
#define CALCS_H_

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define EPSILON 1e-7f  

bool  approx_eq(float a, float b, float epsilon);
float avg_arrf(float *arr, int n);
float std_arrf(float *arr, float avg, int n);

#endif /* CALCS_H_ */
