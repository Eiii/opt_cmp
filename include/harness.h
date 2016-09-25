#pragma once

#include "types.h"

#include <fstream>

class Harness
{
  public:
    Harness(std::string name, const Function& fn, int seed);
    virtual ~Harness() = default;

  public:
    virtual void Evaluate(int max_samples, int iterations) = 0;
    virtual void OutputResult(std::ofstream* of) = 0;

  public:
    const std::string& name() const { return name_; } 
    const std::string out_filename() const { 
      return name_ + "_" + fn_.name + ".csv"; 
    }

  protected:
    RandomEngine rng_;
    const std::string name_;
    const Function& fn_;
};
