#pragma once

#include "harness.h"

class SequentialHarness : public Harness
{
  public:
    SequentialHarness(std::string name, const Function& fn, int seed, 
                      int init_samples);
    virtual ~SequentialHarness() = default;

  public:
    void Evaluate(int max_samples, int iterations) override;

  protected:
    void SingleRun(int run_seed, int iterations);
    double Regret(const vectord& point);
    std::vector<double> CalcDist(const std::vector<vectord>& points);
    void OutputData(nlohmann::json* json) override;
    void OutputRegrets(nlohmann::json* json);
    void OutputDists(nlohmann::json* json);
    void OutputRunTimes(nlohmann::json* json);
    void OutputObjTimes(nlohmann::json* json);
    void OutputPoints(nlohmann::json* j);

  protected:
    virtual void InitEvaluation(int run_seed, int max_samples) = 0;
    virtual vectord SingleStep() = 0;
    virtual vectord BestCurrent() = 0;

  protected:
    const int init_samples_;
    std::vector<std::vector<double>> all_regrets_;
    std::vector<std::vector<double>> all_simple_regrets_;
    std::vector<std::vector<double>> all_dists_;
    std::vector<std::vector<double>> all_run_times_;
    std::vector<std::vector<double>> all_obj_times_;
    std::vector<std::vector<std::vector<double>>> all_points_;
};
