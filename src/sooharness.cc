#include "sooharness.h"

#include "common.h"

/*********************************************************************
* SOOHarness Class
*********************************************************************/
SOOHarness::SOOHarness(const Function& fn, int seed, std::string name) :
    Harness(name, fn, seed), all_regrets_()
{
} /* SOOHarness() */

void SOOHarness::Evaluate(int max_samples, int iterations)
{
  for (int i = 0; i < iterations; i++) {
    RandomInt rand_seed(rng_, UniformIntDist(0, std::numeric_limits<int>::max()));
    SingleRun(rand_seed(), max_samples);
  }
} /* Evaluate() */

void SOOHarness::OutputData(nlohmann::json* j)
{
  OutputRegrets(j);
} /* OutputData() */

void SOOHarness::OutputHeader(nlohmann::json* j)
{
  (*j)["FN_NAME"] = fn_.name;
  (*j)["NAME"] = name_;

  //Output extra info
  std::stringstream ss;
  ss << "# ";
  ss << "v2,";
  ss << "seed:" << seed_;
  (*j)["VERSION"] = ss.str();
} /* OutputHeader() */

std::unique_ptr<logo::RandomSOO> SOOHarness::CreateOptimizer(int run_seed, int max_samples) const
{
  logo::RandomSOO::Options options(fn_.fn, fn_.dim, max_samples, 3, run_seed);
  return std::unique_ptr<logo::RandomSOO>(new logo::RandomSOO(options));
} /* CreateOptimizer() */

void SOOHarness::SingleRun(int run_seed, int max_samples)
{
  std::vector<std::tuple<int, double>> run_regrets;
  std::vector<std::tuple<int, double>> run_ws;

  auto soo = CreateOptimizer(run_seed, max_samples);

  //Record initial regret
  int samples = soo->num_observations();
  double regret = Regret(*soo);
  run_regrets.push_back(std::make_pair(samples, regret));

  while (!soo->IsFinished()) {
    soo->Step();
    int samples = soo->num_observations();
    double regret = Regret(*soo);
    run_regrets.push_back(std::make_pair(samples, regret));
  }

  //Put this run's regrets into the list of all regrets
  all_regrets_.push_back(DenseValues(run_regrets, max_samples));
}

double SOOHarness::Regret(const logo::RandomSOO& soo) const
{
  double regret = fn_.fn_max - soo.BestNode()->value();
  return regret;
} /* Regret() */

void SOOHarness::OutputRegrets(nlohmann::json* j) const
{
  (*j)["REGRETS"] = all_regrets_;
} /* OutputRegrets() */

std::vector<double> SOOHarness::DenseValues(std::vector<std::tuple<int, double>> regrets, int max_samples)
{
  std::vector<double> result(max_samples);
  auto reg_entry = regrets.begin();
  double val = std::numeric_limits<double>::quiet_NaN();
  for (int i = 1; i <= max_samples; i++) {
    while (reg_entry != regrets.end() && i == std::get<0>(*reg_entry)) {
      val = std::get<1>(*reg_entry);
      reg_entry++;
    }
    result[i-1] = val;
  }
  return result;
} /* DenseRegrets() */
