#include "logoharness.h"

#include "common.h"

/*********************************************************************
* LOGOHarness Class
*********************************************************************/
LOGOHarness::LOGOHarness(const Function& fn, int seed) :
    SOOHarness(fn, seed, "LOGO")
{
} /* LOGOHarness() */

std::unique_ptr<logo::RandomSOO> LOGOHarness::CreateOptimizer(int run_seed, int max_samples) const
{
  std::vector<int> w_sched = {3, 4, 5, 6, 8, 30};
  logo::RandomLOGO::Options options(fn_.fn, fn_.dim, max_samples, 3, run_seed, w_sched);
  return std::unique_ptr<logo::RandomLOGO>(new logo::RandomLOGO(options));
} /* CreateOptimizer() */
