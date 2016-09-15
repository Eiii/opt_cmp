#include "common.h"

void output_csv(const std::vector<double>& data, std::ofstream* of)
{
  for (auto it = data.cbegin(); it != data.cend(); ++it) {
    *of << *it;
    if (it+1 != data.cend()) *of << ",";
  }
  *of << std::endl;
}

void output_csv(const std::vector<std::vector<double>>& data, std::ofstream* of)
{
  for (const auto& v : data) output_csv(v, of);
}
