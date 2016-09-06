#pragma once

#define CAT(a, ...) a ## __VA_ARGS__
#define _FN_MAX(x) CAT(MAX__, x)
#define FN_MAX _FN_MAX(FN)

#define _FN_DIM(x) CAT(DIM__, x)
#define FN_DIM _FN_DIM(FN)

#ifndef FN
#define FN rosenbrock_2
#endif

#define MAX__rosenbrock_2 (0.0)
#define DIM__rosenbrock_2 (2)

#define MAX__rosenbrock_10 (0.0)
#define DIM__rosenbrock_10 (10)

#define MAX__sin_2 (0.9517936893872353)
#define DIM__sin_2 (2)

#define MAX__peaks (8.106189094771677)
#define DIM__peaks (2)

#define MAX__branin (-0.397887)
#define DIM__branin (2)

#define TOTAL_SAMPLES 500


double rosenbrock_2(double *i);
double rosenbrock_10(double *i);
double sin_2(double *i);
double peaks(double *i);
double branin(double *i);
