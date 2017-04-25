#pragma once

#include "types.h"
#include "json.hpp"
#include "timer.h"
#include "function.h"

#include <fstream>

extern CPUTimer objective_timer;

class Harness
{
  public:
    Harness(std::string name, const Function& fn, int seed);
    virtual ~Harness() = default;

  public:
    virtual void Evaluate(int max_secs, int iterations);
    void OutputResult(nlohmann::json* json);
    void OutputEdges(nlohmann::json* json);
    virtual void OutputData(nlohmann::json* json) = 0;
    virtual void OutputHeader(nlohmann::json* json) = 0;

  public:
    const std::string& name() const { return name_; } 
    const CPUTimer& timer() const { return timer_; }

  protected:
    RandomEngine rng_;
    const int seed_;
    const std::string name_;
    Function fn_;
    ObjectiveFn objective_;
    CPUTimer timer_;
};
