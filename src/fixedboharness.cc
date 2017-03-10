#include "fixedboharness.h"
#include "common.h"

using std::get;
using bayesopt::vecOfvec;

FixedBOHarness::FixedBOHarness(const Function& fn, int seed, BOParams params) :
  BOHarness(fn, seed, params, "FixedBO1")
{
} /* FixedBOHarness() */

void FixedBOHarness::InitEvaluation(int run_seed, int max_samples)
{
  //Same as BOHarness...
  bopt_params params = CreateParameters(run_seed, max_samples);
  BOModel model(params, fn_); 
  current_model_.reset(new BOModel(params, fn_));

  //Instead of initializing the default way, get the initial observations
  //from SOO
  vecOfvec xs; std::vector<double> ys;

  //Add center point...
  vectord center_point(fn_.dim);
  for (int i = 0; i < fn_.dim; i++) {
    center_point[i] = 0.5;
  }
  double center_val = fn_.fn(center_point);
  xs.push_back(center_point);
  ys.push_back(center_val);

  //...and the rest as random.
  for (int i = 1 ; i < init_samples_; i++) {
    vectord pt(fn_.dim);
    RandomReal rand(rng_, UniformRealDist(0, 1));
    for (size_t j = 0; j < pt.size(); j++) {
      pt(j) = rand();
    }
    double val = fn_.fn(pt);
    xs.push_back(pt);
    ys.push_back(val);
  }

  //Convert to boost container, because of dumb stuff
  vectord bys(ys.size());
  for (size_t i = 0; i < bys.size(); i++) {
    bys(i) = ys[i];
  }
  current_model_->initializeOptimizationWithPoints(xs, bys);
} /* InitEvaluation() */
