#pragma once

#include "boharness.h"

class FixedBOHarness : public BOHarness
{
  public:
    FixedBOHarness(const Function& fn, int seed, BOParams params, bool center);
    virtual ~FixedBOHarness() = default;

  protected:
    void InitEvaluation(int run_seed, int max_samples) override;

  protected:
    bool center_;
};
