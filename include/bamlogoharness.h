#pragma once

#include "sooharness.h"
#include "cpplogo/randombamlogo.h"

class BaMLOGOHarness : public SOOHarness
{
  public:
    BaMLOGOHarness(const Function& fn, int seed);
    virtual ~BaMLOGOHarness() = default;

  protected:
    std::unique_ptr<cpplogo::RandomSOO> CreateOptimizer(int run_seed, int max_samples) const override;
};
