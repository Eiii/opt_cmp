#pragma once

#include "sooharness.h"
#include "cpplogo/randomimgpo.h"

class IMGPOHarness : public SOOHarness
{
  public:
    IMGPOHarness(const Function& fn, int seed);
    virtual ~IMGPOHarness() = default;

  protected:
    std::unique_ptr<cpplogo::RandomSOO> CreateOptimizer(int run_seed) const override;
};
