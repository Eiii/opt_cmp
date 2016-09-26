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
    void OutputData(std::ofstream* of) override;
    void OutputRegrets(std::ofstream* of);
    void OutputDists(std::ofstream* of);

  protected:
    virtual void InitEvaluation(int run_seed, int max_samples) = 0;
    virtual vectord SingleStep() = 0;
    virtual vectord BestCurrent() = 0;

  protected:
    const int num_data_sections_;
    const int init_samples_;
    std::vector<std::vector<double>> all_regrets_;
    std::vector<std::vector<double>> all_dists_;
    std::vector<std::vector<vectord>> all_points_;
};
