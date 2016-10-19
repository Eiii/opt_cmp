#pragma once

#include "sooharness.h"
#include "cpplogo2/randombamsoo.h"

class BaMSOOHarness : public SOOHarness
{
  public:
    BaMSOOHarness(const Function& fn, int seed);
    virtual ~BaMSOOHarness() = default;

  protected:
    std::unique_ptr<logo::RandomSOO> CreateOptimizer(int run_seed, int max_samples) const override;
};
