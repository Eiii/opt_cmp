#include "randomharness.h"
#include "common.h"

RandomHarness::RandomHarness(const Function& fn, int seed) :
    SequentialHarness("RANDOM", fn, seed, 0),
    has_best_(true),
    best_point_(),
    start_center_(true)
{
} /* RandomHarness() */

void RandomHarness::InitEvaluation(int run_seed, int max_samples)
{
  (void)run_seed; (void)max_samples;
  has_best_ = true;
  start_center_ = true;
} /* InitEvaluation() */

vectord RandomHarness::SingleStep()
{
  vectord rand_point(fn_.dim); 

  RandomReal rand(rng_, UniformRealDist(0, 1));
  for (int i = 0; i < fn_.dim; i++) {
    if (start_center_) {
      rand_point(i) = 0.5;
    } else {
      rand_point(i) = rand();
    }
  }

  start_center_ = false;

  if (has_best_ || Regret(rand_point) < Regret(best_point_)) {
    best_point_ = rand_point;
    has_best_ = false;
  }

  return rand_point;
} /* SingleStep() */

vectord RandomHarness::BestCurrent()
{
  return best_point_;
} /* BestCurrent() */

void RandomHarness::OutputHeader(nlohmann::json* j)
{
  (*j)["FN_NAME"] = fn_.name;
  (*j)["NAME"] = name_;
  (*j)["VERSION"] = "v1";
} /* OutputData() */
