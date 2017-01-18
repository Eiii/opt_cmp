#pragma once
#include <cmath>
#include "types.h"
#include "timer.h"

matrixd set_matrix(size_t rows, size_t cols, const std::vector<double>& vec);
vectord set_vector(const std::vector<double>& vec);

/***********************************************************
* Timer
***********************************************************/
extern CPUTimer objective_timer;
ObjectiveFn add_timer(const ObjectiveFn& fn);

/***********************************************************
* sin
***********************************************************/
extern Function f_sin_1;
extern Function f_sin_2;

/***********************************************************
* peaks
***********************************************************/
extern Function f_peaks;

/***********************************************************
* branin
***********************************************************/
extern Function f_branin;

/***********************************************************
* rosenbrock
***********************************************************/
extern Function f_rosenbrock_2;
extern Function f_rosenbrock_4;
extern Function f_rosenbrock_6;
extern Function f_rosenbrock_8;
extern Function f_rosenbrock_10;

/***********************************************************
* hartman
***********************************************************/
extern Function f_hartman_3;
extern Function f_hartman_6;

/***********************************************************
* shekel
***********************************************************/
extern Function f_shekel_5;
extern Function f_shekel_7;
extern Function f_shekel_10;

/***********************************************************
* rastrigin
***********************************************************/
extern Function f_rastrigin_2;
extern Function f_rastrigin_4;
extern Function f_rastrigin_6;
extern Function f_rastrigin_10;

/***********************************************************
* schwefel
***********************************************************/
extern Function f_schwefel_2;
extern Function f_schwefel_4;
extern Function f_schwefel_6;
extern Function f_schwefel_10;

/***********************************************************
* ackley
***********************************************************/
extern Function f_ackley_2;
extern Function f_ackley_4;
extern Function f_ackley_6;
extern Function f_ackley_10;
