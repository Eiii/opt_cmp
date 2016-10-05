#pragma once

#include "harness.h"
#include "logoharness.h"
#include "cpplogo/logo.h"

class SOOHarness : public Harness
{
  public:
    SOOHarness(const Function& fn, int seed);
    virtual ~SOOHarness() = default;

  public:
    void Evaluate(int max_samples, int iterations) override;
    void OutputData(nlohmann::json* j) override;
    void OutputHeader(nlohmann::json* j) override;

  protected:
    void SingleRun(int run_seed, int max_samples);
    double Regret(const logo::LOGO& logo) const;
    void OutputRegrets(nlohmann::json* j) const;
    void OutputWs(nlohmann::json* j) const;
    logo::Options BuildOptions(const Settings& opt, int max, int seed) const;
    std::vector<double> DenseValues(std::vector<std::tuple<int, double>> regrets, int max_samples);

  protected:
    std::vector<std::vector<double>> all_regrets_;
    std::vector<std::vector<double>> all_ws_;
    Settings setting_;
};
