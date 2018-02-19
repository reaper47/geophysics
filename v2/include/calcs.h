/*******************************************************************
 * NAME: calcs.h
 *
 * PURPOSE: Defines general-purpose calculation functions
 *
 * GLOBAL VARIABLES: None
 *
 * Variables              Type      Description
 * ---------              ----      -----------
 * EPSILON                double    minimum error margin 
 *
 * LAT_CORR_mGAL_PER_KM   double    Maximum latitude correction
 *                                  per kilometer at a latitude 
 *                                  of 45deg (mGal). Null at poles.
 *
 *******************************************************************/
#ifndef CALCS_H_
#define CALCS_H_

#define _GNU_SOURCE
#include <stdlib.h>
#include <math.h>

#define EPSILON 1e-7
#define LAT_CORR_mGAL_PER_KM -0.81399

_Bool     ApproxEq         (double a, double b, double epsilon);
double    AvgArr           (double *arr, int n);
double    CorrectLatitude  (double lat, double lng, double pos);
double*   InterpolatePts   (double start_value, double end_value, int steps);
double    MaxArr           (double *arr, int n);
double    StdArr           (double *arr, double avg, int n);

#endif /* CALCS_H_ */

