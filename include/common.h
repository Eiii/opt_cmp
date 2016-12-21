#pragma once

#include <vector>
#include <fstream>
#include "bayesopt/bayesopt.hpp"

std::vector<double> vectord_to_vector(vectord v);
vectord vector_to_vectord(std::vector<double> v);
std::vector<std::vector<double>> vectord_to_vector(std::vector<vectord> v);
