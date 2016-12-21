#include "sooharness.h"

#include "common.h"

#include <iostream>
#include <chrono>
#include <ctime>

using std::vector;

//Anonymous namespace for helper functions
namespace {

void add_node_points(const std::vector<cpplogo::Node>& nodes, 
                     vector<vector<vector<double>>>* points)
{
  vector<vector<double>> pts;
  for (const auto& node : nodes) {
    pts.push_back(vectord_to_vector(node.Center()));
  }
  points->push_back(pts);
} /* add_node_points() */
}

/*********************************************************************
* SOOHarness Class
*********************************************************************/
SOOHarness::SOOHarness(const Function& fn, int seed, std::string name) :
    Harness(name, fn, seed),
    all_regrets_(),
    all_times_(),
    all_obj_times_(),
    all_points_()
{
} /* SOOHarness() */

void SOOHarness::Evaluate(int max_samples, int iterations)
{
  Harness::Evaluate(max_samples, iterations);
  for (int i = 0; i < iterations; i++) {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << std::put_time(std::localtime(&now), "%F %T") << " - ";
    try {
      RandomInt rand_seed(rng_, UniformIntDist(0, std::numeric_limits<int>::max()));
      SingleRun(rand_seed(), max_samples);

      std::cout << "Finished iteration " << (i+1) << std::endl;
      std::cout.flush();
    } catch (const std::exception& e) {
      std::cout << "EXCEPTION @ " << i << ": " << e.what() << std::endl;
    }
  }
} /* Evaluate() */

void SOOHarness::OutputData(nlohmann::json* j)
{
  OutputRegrets(j);
  OutputTimes(j);
  OutputObjTimes(j);
  OutputPoints(j);
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

std::unique_ptr<cpplogo::RandomSOO> SOOHarness::CreateOptimizer(int run_seed, int max_samples) const
{
  cpplogo::RandomSOO::Options options(fn_.fn, fn_.dim, max_samples, 3, run_seed);
  return std::unique_ptr<cpplogo::RandomSOO>(new cpplogo::RandomSOO(options));
} /* CreateOptimizer() */

int SOOHarness::GetNumSamples(const cpplogo::RandomSOO* soo) const
{
  return soo->num_observations();
} /* PrepareRunRegrets() */

void SOOHarness::SingleRun(int run_seed, int max_samples)
{
  timer_.Reset();
  objective_timer.Reset();
  vector<std::tuple<int, double>> run_regrets;
  vector<std::tuple<int, double>> run_times;
  vector<std::tuple<int, double>> run_obj_times;
  vector<vector<vector<double>>> run_points;

  auto soo = CreateOptimizer(run_seed, max_samples);

  //Record initial regret & points
  int samples = GetNumSamples(soo.get());
  double regret = Regret(*soo);
  run_regrets.push_back(std::make_pair(samples, regret));
  add_node_points(soo->step_observed_nodes(), &run_points);

  while (!soo->IsFinished()) {
    timer_.Start();
    soo->Step();
    timer_.Stop();
    int samples = GetNumSamples(soo.get());
    double regret = Regret(*soo);
    run_regrets.push_back(std::make_pair(samples, regret));
    run_times.push_back(std::make_pair(samples, timer_.ElapsedTime()));
    run_obj_times.push_back(std::make_pair(samples, objective_timer.ElapsedTime()));
    add_node_points(soo->step_observed_nodes(), &run_points);
  }

  //Put this run's regrets into the list of all regrets
  all_regrets_.push_back(DenseValues(run_regrets, max_samples));
  all_times_.push_back(DenseValues(run_times, max_samples));
  all_obj_times_.push_back(DenseValues(run_obj_times, max_samples));
  all_points_.push_back(run_points);
}

double SOOHarness::Regret(const cpplogo::RandomSOO& soo) const
{
  double regret = fn_.fn_max - soo.BestNode()->value();
  return regret;
} /* Regret() */

void SOOHarness::OutputRegrets(nlohmann::json* j) const
{
  (*j)["REGRETS"] = all_regrets_;
} /* OutputRegrets() */

void SOOHarness::OutputTimes(nlohmann::json* j) const
{
  (*j)["RUNTIMES"] = all_times_;
} /* OutputRegrets() */

void SOOHarness::OutputObjTimes(nlohmann::json* j) const
{
  (*j)["OBJ_RUNTIMES"] = all_obj_times_;
} /* OutputRegrets() */

void SOOHarness::OutputPoints(nlohmann::json* j) const
{
  (*j)["POINTS"] = all_points_;
} /* OutputPoints() */

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
