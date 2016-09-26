#pragma once

#include "harness.h"
#include "bayesopt.hpp"
#include <tuple>

class BOHarness : public Harness
{
  public:
    //Criteria, Kernel, Surrogate, Relearn #, Initial samples
    using BOParams = std::tuple<std::string, std::string, std::string, int, int>;
  public:
    BOHarness(const Function& fn, int seed, 
              BOParams params = BOParams("cEI", "kSEISO", "sGaussianProcessML", 15, 2)
             );
    virtual ~BOHarness() = default;

  public:
    void Evaluate(int max_samples, int iterations) override;
    void OutputData(std::ofstream* of) override;
    void OutputHeader(std::ofstream* of) override;

  protected:
    void SingleRun(int run_seed, int iterations);
    bopt_params CreateParameters(int seed, int iterations);
    double Regret(const vectord& point);
    std::vector<double> CalcDist(const std::vector<vectord>& points);
    void OutputRegrets(std::ofstream* of);
    void OutputDists(std::ofstream* of);

  protected:
    const std::string criteria_;
    const std::string kernel_; 
    const std::string surrogate_; 
    const int it_relearn_;
    const int init_samples_;
    std::vector<std::vector<double>> all_regrets_;
    std::vector<std::vector<double>> all_dists_;
};
