#include "common.h"

std::vector<double> vectord_to_vector(vectord v)
{
  return std::vector<double>(v.begin(), v.end());
}

vectord vector_to_vectord(std::vector<double> v)
{
  vectord vd(v.size());
  for (size_t i = 0; i < vd.size(); i++) {
    vd[i] = v[i]; 
  }
  return vd;
}

std::vector<std::vector<double>> vectord_to_vector(std::vector<vectord> v)
{
  std::vector<std::vector<double>> result;
  for (const auto& entry : v) {
    result.push_back(vectord_to_vector(entry));
  }
  return result;
}
