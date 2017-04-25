#include "bamsooharness.h"

#include "common.h"

/*********************************************************************
* BaMSOOHarness Class
*********************************************************************/
BaMSOOHarness::BaMSOOHarness(const Function& fn, int seed) :
    SOOHarness(fn, seed, "BaMSOO")
{
} /* BaMSOOHarness() */

std::unique_ptr<cpplogo::RandomSOO> BaMSOOHarness::CreateOptimizer(int run_seed) const
{
  cpplogo::RandomBaMSOO::Options options(objective_, fn_.dim, 10, 3, run_seed);
  return std::unique_ptr<cpplogo::RandomBaMSOO>(new cpplogo::RandomBaMSOO(options));
} /* CreateOptimizer() */
