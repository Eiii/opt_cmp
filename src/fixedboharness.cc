#include "fixedboharness.h"
#include "cpplogo/randomsoo.h"
#include "common.h"

using std::get;
using bayesopt::vecOfvec;

FixedBOHarness::FixedBOHarness(const Function& fn, int seed, FixedBOParams params) :
  BOHarness(fn, seed, std::tuple_cat(params, std::make_tuple(3)), "FixedBO1")
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
  cpplogo::RandomSOO::Options options(fn_.fn, fn_.dim, max_samples, 3, run_seed);
  cpplogo::RandomSOO soo(options);

  //First observation
  auto last = soo.step_observed_nodes();
  assert(last.size() == 1);
  {
    const auto& node = last.front();
    xs.push_back(node.Center());
    ys.push_back(node.value());
  }

  //Second observation
  soo.Step();
  last = soo.step_observed_nodes();
  //Grabbing the first TWO nodes from the next step lets us
  //get the first three nodes that split the space. This
  //is a horrible hack, but it works as long as the order
  //of the nodes in the step_observed list stays the same.
  for (int i = 0; i < 2; i++) {
    const auto& node = last[i];
    xs.push_back(node.Center());
    ys.push_back(node.value());
  }

  //Convert to boost container, because of dumb stuff
  vectord bys(ys.size());
  for (size_t i = 0; i < bys.size(); i++) {
    bys(i) = ys[i];
  }
  current_model_->initializeOptimizationWithPoints(xs, bys);
} /* InitEvaluation() */
