#pragma once

#include "harness.h"

class DIRECTHarness : public Harness
{
  public:
    DIRECTHarness(const Function& fn, int seed, std::string name="SOO");
    virtual ~DIRECTHarness() = default;

  public:
    void Evaluate(int max_samples, int iterations) override;
    void OutputData(nlohmann::json* j) override;
    void OutputHeader(nlohmann::json* j) override;

  protected:
    void SingleRun(int run_seed, int max_samples);
    double Regret() const;
    void OutputRegrets(nlohmann::json* j) const;
    void OutputPoints(nlohmann::json* j) const;

  protected:
    std::vector<std::vector<double>> all_regrets_;
    std::vector<std::vector<std::vector<double>>> all_points_;
};

struct DIRECTData {
  std::vector<std::vector<double>>* run_points;
  ObjectiveFn* objective;
};

double DIRECTObjective(const std::vector<double>& x, std::vector<double>& grad, void* f_data);
