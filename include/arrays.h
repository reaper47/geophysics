#ifndef ARRAY_H_
#define ARRAY_H_

#define _GNU_SOURCE
#include <math.h>

double    avg_arr       (double *arr, int n);
double    max_arr       (double *arr, int n);
double    std_arr       (double *arr, double avg, int n);

#endif /* ARRAY_H_ */

