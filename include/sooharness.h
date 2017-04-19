#pragma once

#include "harness.h"
#include "cpplogo/randomsoo.h"

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
    virtual std::unique_ptr<cpplogo::RandomSOO> CreateOptimizer(int run_seed, int max_samples) const;
    virtual int GetNumSamples(const cpplogo::RandomSOO* soo) const;
    virtual double Regret(const cpplogo::RandomSOO& logo) const;

  protected:
    void SingleRun(int run_seed, int max_samples);
    void OutputRegrets(nlohmann::json* j) const;
    void OutputTimes(nlohmann::json* j) const;
    void OutputObjTimes(nlohmann::json* j) const;
    void OutputPoints(nlohmann::json* j) const;
    std::vector<double> DenseValues(std::vector<std::tuple<int, double>> regrets);

  protected:
    std::vector<std::vector<double>> all_regrets_;
    std::vector<std::vector<double>> all_times_;
    std::vector<std::vector<double>> all_obj_times_;
    std::vector<std::vector<std::vector<std::vector<double>>>> all_points_;
};
