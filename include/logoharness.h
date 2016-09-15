#pragma once

#include "harness.h"
#include "cpplogo/logo.h"

/***********************************************************
* Settings
* LOGO settings - COPIED FROM cpplogo/main.h
***********************************************************/
struct Settings {
  std::string name;
  std::function<int(const logo::LOGO&)> w_sched;
  int init_w;
};

class LOGOHarness : public Harness
{
  public:
    LOGOHarness(const Function& fn, int seed);
    virtual ~LOGOHarness() = default;

  public:
    void Evaluate(int max_samples, int iterations) override;
    void OutputResult(std::ofstream* of) override;

  protected:
    void SingleRun(int run_seed, int max_samples);
    double Regret(const logo::LOGO& logo) const;
    void OutputRegrets(std::ofstream* of) const;
    logo::Options BuildOptions(const Settings& opt, int max, int seed) const;
    std::vector<double> DenseRegrets(std::vector<std::tuple<int, double>> regrets, int max_samples);

  protected:
    std::vector<std::vector<double>> all_regrets_;
    Settings setting_;
};
