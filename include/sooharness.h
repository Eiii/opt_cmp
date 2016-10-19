#pragma once

#include "harness.h"
#include "cpplogo2/randomsoo.h"

class SOOHarness : public Harness
{
  public:
    SOOHarness(const Function& fn, int seed, std::string name="SOO");
    virtual ~SOOHarness() = default;

  public:
    void Evaluate(int max_samples, int iterations) override;
    void OutputData(nlohmann::json* j) override;
    void OutputHeader(nlohmann::json* j) override;

  protected:
    virtual std::unique_ptr<logo::RandomSOO> CreateOptimizer(int run_seed, int max_samples) const;

  protected:
    void SingleRun(int run_seed, int max_samples);
    double Regret(const logo::RandomSOO& logo) const;
    void OutputRegrets(nlohmann::json* j) const;
    std::vector<double> DenseValues(std::vector<std::tuple<int, double>> regrets, int max_samples);

  protected:
    std::vector<std::vector<double>> all_regrets_;
};
