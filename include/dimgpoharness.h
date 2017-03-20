#pragma once

#include "dsooharness.h"
#include "cpplogo/imgpo.h"

class dIMGPOHarness : public dSOOHarness
{
  public:
    dIMGPOHarness(const Function& fn, int seed);
    virtual ~dIMGPOHarness() = default;

  protected:
    std::unique_ptr<cpplogo::SOO> CreateOptimizer(int run_seed, int max_samples) const override;
};
