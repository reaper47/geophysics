#ifndef CALCS_H_
#define CALCS_H_

#define _GNU_SOURCE
#include <stdlib.h>
#include <math.h>

#define EPSILON 1e-7
#define LAT_CORR_mGAL_PER_KM -0.81399

struct point_t {
	double x;
	double y;
};

_Bool     approx_eq         (double a, double b, double epsilon);
double    correct_latitude  (double lat, double lng, double pos);
double    line_intercept    (struct point_t p1, struct point_t p2, double slope);
double*   interpolate_pts   (double start_value, double end_value, int steps);

#endif /* CALCS_H_ */

