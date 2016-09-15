#pragma once

#include "harness.h"

#include "bayesopt.hpp"

class BOHarness : public Harness
{
  public:
    BOHarness(const Function& fn, int seed);
    virtual ~BOHarness() = default;

  public:
    void Evaluate(int max_samples, int iterations) override;
    void OutputResult(std::ofstream* of) override;

  protected:
    void SingleRun(int run_seed, int iterations);
    bopt_params CreateParameters(int seed, int iterations);
    double Regret(const vectord& point);
    void OutputRegrets(std::ofstream* of);

  protected:
    std::vector<std::vector<double>> all_regrets_;
};
