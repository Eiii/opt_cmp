#include "bamsooharness.h"

#include "common.h"

/*********************************************************************
* BaMSOOHarness Class
*********************************************************************/
BaMSOOHarness::BaMSOOHarness(const Function& fn, int seed) :
    SOOHarness(fn, seed, "BaMSOO")
{
} /* BaMSOOHarness() */

std::unique_ptr<logo::RandomSOO> BaMSOOHarness::CreateOptimizer(int run_seed, int max_samples) const
{
  logo::RandomBaMSOO::Options options(fn_.fn, fn_.dim, max_samples, 3, run_seed);
  return std::unique_ptr<logo::RandomBaMSOO>(new logo::RandomBaMSOO(options));
} /* CreateOptimizer() */
