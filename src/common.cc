#include "common.h"

std::vector<double> vectord_to_vector(vectord v)
{
  return std::vector<double>(v.begin(), v.end());
}

std::vector<std::vector<double>> vectord_to_vector(std::vector<vectord> v)
{
  std::vector<std::vector<double>> result;
  for (const auto& entry : v) {
    result.push_back(vectord_to_vector(entry));
  }
  return result;
}
