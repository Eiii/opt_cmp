#pragma once

#include "harness.h"
#include "cpplogo/imgpo.h"

class dSOOHarness : public Harness
{
  public:
    dSOOHarness(const Function& fn, int seed, std::string name="dSOO");
    virtual ~dSOOHarness() = default;

  public:
    void Evaluate(int max_samples, int iterations) override;
    void OutputData(nlohmann::json* j) override;
    void OutputHeader(nlohmann::json* j) override;

  protected:
    virtual std::unique_ptr<cpplogo::SOO> CreateOptimizer(int run_seed, int max_samples) const;
    virtual int GetNumSamples(const cpplogo::SOO* soo) const;
    virtual double Regret(const cpplogo::SOO& logo) const;

  protected:
    void SingleRun(int run_seed, int max_samples);
    void OutputRegrets(nlohmann::json* j) const;
    void OutputTimes(nlohmann::json* j) const;
    void OutputObjTimes(nlohmann::json* j) const;
    void OutputPoints(nlohmann::json* j) const;
    std::vector<double> DenseValues(std::vector<std::tuple<int, double>> regrets, int max_samples);

  protected:
    std::vector<std::vector<double>> all_regrets_;
    std::vector<std::vector<double>> all_times_;
    std::vector<std::vector<double>> all_obj_times_;
    std::vector<std::vector<std::vector<std::vector<double>>>> all_points_;
};
