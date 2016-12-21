#pragma once

#include "sequentialharness.h"
#include "bayesopt/bayesopt.hpp"
#include "bopt.h"
#include <tuple>

class BOHarness : public SequentialHarness
{
  public:
    //Criteria, Kernel, Surrogate, Relearn #, Initial samples
    using BOParams = std::tuple<std::string, std::string, std::string, int, int>;
  public:
    BOHarness(const Function& fn, int seed, 
              BOParams params = BOParams("cEI", "kSEISO", "sGaussianProcessML", 15, 2),
              std::string name = "BO1");
    virtual ~BOHarness() = default;

  protected:
    void InitEvaluation(int run_seed, int max_samples) override;
    vectord SingleStep() override;
    vectord BestCurrent() override;

  protected:
    void OutputHeader(nlohmann::json* j) override;
    bopt_params CreateParameters(int seed, int iterations);

  protected:
    const std::string criteria_;
    const std::string kernel_; 
    const std::string surrogate_; 
    const int it_relearn_;
    std::unique_ptr<BOModel> current_model_;
};
