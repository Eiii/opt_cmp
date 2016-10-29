#pragma once

#include "sooharness.h"
#include "cpplogo2/initbamsoo.h"

class InitBaMSOOHarness : public SOOHarness
{
  public:
    InitBaMSOOHarness(const Function& fn, int seed, int init_samples);
    virtual ~InitBaMSOOHarness() = default;

  public:
    void OutputHeader(nlohmann::json* j) override;
    int GetNumSamples(const logo::RandomSOO* soo) const override;

  protected:
    std::unique_ptr<logo::RandomSOO> CreateOptimizer(int run_seed, int max_samples) const override;

  protected:
    int init_samples_;
};
