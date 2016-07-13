#pragma once

#define CAT(a, ...) a ## __VA_ARGS__
#define _FN_MAX(x) CAT(MAX__, x)
#define FN_MAX _FN_MAX(FN)

#ifndef FN
#define FN rosenbrock_2
#endif

#define MAX__rosenbrock_2 (0.0)
#define MAX__sin_2 (0.9517936893872353)


#define TOTAL_SAMPLES 100

double rosenbrock_2(double *i);
double sin_2(double *i);
