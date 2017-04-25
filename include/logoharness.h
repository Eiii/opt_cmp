#pragma once

#include "sooharness.h"
#include "cpplogo/randomlogo.h"

class LOGOHarness : public SOOHarness
{
  public:
    LOGOHarness(const Function& fn, int seed);
    virtual ~LOGOHarness() = default;

  protected:
    std::unique_ptr<cpplogo::RandomSOO> CreateOptimizer(int run_seed) const override;
};
