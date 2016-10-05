#pragma once

#include "sequentialharness.h"

class RandomHarness : public SequentialHarness
{
  public:
    RandomHarness(const Function& fn, int seed);
    virtual ~RandomHarness() = default;

  protected:
    void InitEvaluation(int run_seed, int max_samples) override;
    vectord SingleStep() override;
    vectord BestCurrent() override;

  protected:
    void OutputHeader(nlohmann::json* json) override;

  protected:
    bool has_best;
    vectord best_point;
};
