#include "dimgpoharness.h"

#include "common.h"

/*********************************************************************
* dIMGPOHarness Class
*********************************************************************/
dIMGPOHarness::dIMGPOHarness(const Function& fn, int seed) :
    dSOOHarness(fn, seed, "dIMGPO")
{
} /* IMGPOHarness() */

std::unique_ptr<cpplogo::SOO> dIMGPOHarness::CreateOptimizer(int run_seed, int max_samples) const
{
  (void)run_seed;
  cpplogo::IMGPO::Options options(objective_, fn_.dim, max_samples, 3, 4);
  return std::unique_ptr<cpplogo::IMGPO>(new cpplogo::IMGPO(options));
} /* CreateOptimizer() */
