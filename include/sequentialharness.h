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
    void OutputData(nlohmann::json* json) override;
    void OutputRegrets(nlohmann::json* json);
    void OutputRunTimes(nlohmann::json* json);
    void OutputObjTimes(nlohmann::json* json);
    void OutputPoints(nlohmann::json* j);
    virtual void ResetTimers();
    virtual void InitRunLists();
    virtual void UpdateRunLists(const vectord& last, const vectord& best_current);
    virtual void CommitRunLists();

  protected:
    virtual void InitEvaluation(int run_seed, int max_samples) = 0;
    virtual vectord SingleStep() = 0;
    virtual vectord BestCurrent() = 0;

  protected:
    const int init_samples_;
    std::vector<std::vector<double>> all_regrets_;
    std::vector<std::vector<double>> all_simple_regrets_;
    std::vector<std::vector<double>> all_run_times_;
    std::vector<std::vector<double>> all_obj_times_;
    std::vector<std::vector<std::vector<double>>> all_points_;

    //Run-specific variables
    std::vector<double> run_regrets_;
    std::vector<double> run_simple_regrets_;
    std::vector<double> run_times_;
    std::vector<double> run_obj_times_;
    std::vector<vectord> run_points_;
};
