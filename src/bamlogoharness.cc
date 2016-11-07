#include "bamlogoharness.h"

#include "common.h"

/*********************************************************************
* BaMLOGOHarness Class
*********************************************************************/
BaMLOGOHarness::BaMLOGOHarness(const Function& fn, int seed) :
    SOOHarness(fn, seed, "BAMLOGO")
{
} /* LOGOHarness() */

std::unique_ptr<cpplogo::RandomSOO> BaMLOGOHarness::CreateOptimizer(int run_seed, int max_samples) const
{
  std::vector<int> w_sched = {3, 4, 5, 6, 8, 30};
  cpplogo::RandomBaMLOGO::Options options(fn_.fn, fn_.dim, max_samples, 3, run_seed, w_sched);
  return std::unique_ptr<cpplogo::RandomBaMLOGO>(new cpplogo::RandomBaMLOGO(options));
} /* CreateOptimizer() */
