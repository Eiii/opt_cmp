#include "directharness.h"

#include "common.h"

#include "nlopt.hpp"

#include <chrono>

using std::vector;

/*********************************************************************
* DIRECTHarness Class
*********************************************************************/
DIRECTHarness::DIRECTHarness(const Function& fn, int seed, std::string name) :
    Harness(name, fn, seed),
    all_regrets_(),
    all_points_()
{
} /* DIRECTHarness() */

void DIRECTHarness::Evaluate(int max_samples, int iterations)
{
  Harness::Evaluate(max_samples, iterations);
  for (int i = 0; i < iterations; i++) {
    RandomInt rand_seed(rng_, UniformIntDist(0, std::numeric_limits<int>::max()));
    SingleRun(rand_seed(), max_samples);

    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << std::put_time(std::localtime(&now), "%F %T") << " - ";
    std::cout << "Finished iteration " << (i+1) << std::endl;
    std::cout.flush();
  }
} /* Evaluate() */

void DIRECTHarness::OutputData(nlohmann::json* j)
{
  OutputRegrets(j);
  OutputPoints(j);
} /* OutputData() */

void DIRECTHarness::OutputHeader(nlohmann::json* j)
{
  (*j)["FN_NAME"] = fn_.name;
  (*j)["NAME"] = name_;

  //Output extra info
  std::stringstream ss;
  ss << "# ";
  ss << "v1,";
  ss << "seed:" << seed_;
  (*j)["VERSION"] = ss.str();
} /* OutputHeader() */

void DIRECTHarness::SingleRun(int run_seed, int max_samples)
{
  (void)run_seed;
  vector<double> run_regrets;
  vector<vector<double>> run_points;

  //Create extra nlopt data
  DIRECTData nlopt_data{ &run_points, &objective_ };
  
  //Create nlopt
  nlopt::opt opt(nlopt::algorithm::GN_DIRECT, fn_.dim);
  opt.set_max_objective(&DIRECTObjective, &nlopt_data);
  opt.set_lower_bounds(0.0);
  opt.set_upper_bounds(1.0);
  opt.set_maxeval(max_samples);

  vector<double> pt(fn_.dim, 0.5);
  double val;
  opt.optimize(pt, val);

  //Put this run's regrets into the list of all regrets
  all_regrets_.push_back(run_regrets);
  all_points_.push_back(run_points);
} /* SingleRun() */

double DIRECTHarness::Regret() const
{
  return 0.0; //TODO
} /* Regret() */

void DIRECTHarness::OutputRegrets(nlohmann::json* j) const
{
  (*j)["REGRETS"] = all_regrets_;
} /* OutputRegrets() */

void DIRECTHarness::OutputPoints(nlohmann::json* j) const
{
  (*j)["POINTS"] = all_points_;
} /* OutputPoints() */

double DIRECTObjective(const std::vector<double>& x, std::vector<double>& grad, void* f_data)
{
  DIRECTData* dd = static_cast<DIRECTData*>(f_data);
  auto objective_fn = *dd->objective;
  double val = objective_fn(vector_to_vectord(x));
  grad.clear();
  return val;
} /* DIRECTObjective() */
