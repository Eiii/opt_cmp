#pragma once

#include "sooharness.h"
#include "cpplogo/randombamsoo.h"

class BaMSOOHarness : public SOOHarness
{
  public:
    BaMSOOHarness(const Function& fn, int seed);
    virtual ~BaMSOOHarness() = default;

  protected:
    std::unique_ptr<cpplogo::RandomSOO> CreateOptimizer(int run_seed, int max_samples) const override;
};
