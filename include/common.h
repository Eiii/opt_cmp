#pragma once

#include <vector>
#include <fstream>

void output_csv(const std::vector<double>& data, std::ofstream* of);
void output_csv(const std::vector<std::vector<double>>& data, std::ofstream* of);
