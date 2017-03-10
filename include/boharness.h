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
    vectord BestCurrentSimple() override;
    void ResetTimers() override;
    void InitRunLists() override;
    void UpdateRunLists(const vectord& last, const vectord& best_current) override;
    void CommitRunLists() override;

  protected:
    void OutputData(nlohmann::json* json) override;
    void OutputHeader(nlohmann::json* j) override;
    void OutputModelTime(nlohmann::json* j);
    void OutputCritTime(nlohmann::json* j);
    bopt_params CreateParameters(int seed, int iterations);

  protected:
    const std::string criteria_;
    const std::string kernel_; 
    const std::string surrogate_; 
    const int it_relearn_;
    std::unique_ptr<BOModel> current_model_;
    std::vector<std::vector<double>> all_model_times_;
    std::vector<std::vector<double>> all_crit_times_;

    //Run-specific variables
    std::vector<double> run_model_times_;
    std::vector<double> run_crit_times_;
};
