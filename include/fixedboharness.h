#pragma once

#include "boharness.h"

class FixedBOHarness : public BOHarness
{
  public:
    //Criteria, Kernel, Surrogate, Relearn #
    using FixedBOParams = std::tuple<std::string, std::string, std::string, int>;
  public:
    FixedBOHarness(const Function& fn, int seed, 
              FixedBOParams params = FixedBOParams("cEI", "kSEISO", "sGaussianProcessML", 15)
             );
    virtual ~FixedBOHarness() = default;

  protected:
    void InitEvaluation(int run_seed, int max_samples) override;
};
