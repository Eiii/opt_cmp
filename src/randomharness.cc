#include "randomharness.h"
#include "common.h"

RandomHarness::RandomHarness(const Function& fn, int seed) :
    SequentialHarness("RANDOM", fn, seed, 0),
    has_best(true),
    best_point()
{
} /* RandomHarness() */

void RandomHarness::InitEvaluation(int run_seed, int max_samples)
{
  (void)run_seed; (void)max_samples;
  has_best = true;
} /* InitEvaluation() */

vectord RandomHarness::SingleStep()
{
  vectord rand_point(fn_.dim); 

  RandomReal rand(rng_, UniformRealDist(0, 1));
  for (int i = 0; i < fn_.dim; i++) {
    rand_point(i) = rand();
  }

  if (has_best || Regret(rand_point) < Regret(best_point)) {
    best_point = rand_point;
    has_best = false;
  }

  return rand_point;
} /* SingleStep() */

vectord RandomHarness::BestCurrent()
{
  return best_point;
} /* BestCurrent() */

void RandomHarness::OutputHeader(nlohmann::json* j)
{
  (*j)["FN_NAME"] = fn_.name;
  (*j)["NAME"] = name_;
  (*j)["VERSION"] = "v1";
} /* OutputData() */
