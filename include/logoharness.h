#pragma once

#include "sooharness.h"
#include "cpplogo2/randomlogo.h"

class LOGOHarness : public SOOHarness
{
  public:
    LOGOHarness(const Function& fn, int seed);
    virtual ~LOGOHarness() = default;

  protected:
    std::unique_ptr<logo::RandomSOO> CreateOptimizer(int run_seed, int max_samples) const override;
};
