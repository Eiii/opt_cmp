#include "imgpoharness.h"

#include "common.h"

/*********************************************************************
* IMGPOHarness Class
*********************************************************************/
IMGPOHarness::IMGPOHarness(const Function& fn, int seed) :
    SOOHarness(fn, seed, "IMGPO")
{
} /* IMGPOHarness() */

std::unique_ptr<cpplogo::RandomSOO> IMGPOHarness::CreateOptimizer(int run_seed) const
{
  cpplogo::RandomIMGPO::Options options(objective_, fn_.dim, 10, 3, run_seed, 4);
  return std::unique_ptr<cpplogo::RandomIMGPO>(new cpplogo::RandomIMGPO(options));
} /* CreateOptimizer() */
