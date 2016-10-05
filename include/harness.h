#pragma once

#include "types.h"
#include "json.hpp"

#include <fstream>

class Harness
{
  public:
    Harness(std::string name, const Function& fn, int seed);
    virtual ~Harness() = default;

  public:
    virtual void Evaluate(int max_samples, int iterations) = 0;
    void OutputResult(nlohmann::json* json);
    virtual void OutputData(nlohmann::json* json) = 0;
    virtual void OutputHeader(nlohmann::json* json) = 0;

  public:
    const std::string& name() const { return name_; } 

  protected:
    RandomEngine rng_;
    const int seed_;
    const std::string name_;
    const Function& fn_;
};
