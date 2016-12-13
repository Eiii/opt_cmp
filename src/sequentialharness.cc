#include "sequentialharness.h"
#include "common.h"

#include <iostream>

SequentialHarness::SequentialHarness(std::string name, const Function& fn, 
                                     int seed, int init_samples) :
    Harness(name, fn, seed), 
    init_samples_(init_samples),
    all_regrets_(), 
    all_dists_(),
    all_points_()
{
} /* SequentialHarness() */

void SequentialHarness::Evaluate(int max_samples, int iterations)
{
  Harness::Evaluate(max_samples, iterations);
  for (int i = 0; i < iterations; i++) {
    try {
      RandomInt rand_seed(rng_, UniformIntDist(0, std::numeric_limits<int>::max()));
      SingleRun(rand_seed(), max_samples);
      if ((i+1) % 5 == 0) {
        std::cout << "Finished iteration " << (i+1) << std::endl;
        std::cout.flush();
      }
    } catch (const std::exception& e) {
      std::cout << "EXCEPTION @ " << i << ": " << e.what() << std::endl;
    }
  }
} /* Evaluate() */

void SequentialHarness::SingleRun(int run_seed, int max_samples)
{
  std::vector<double> run_regrets;
  std::vector<double> run_simple_regrets;
  std::vector<vectord> run_points;

  InitEvaluation(run_seed, max_samples);

  //No regret associated with the initial samples-- just put in NaN
  for (int i = 0; i < init_samples_-1; i++) {
    run_regrets.push_back(std::numeric_limits<double>::quiet_NaN());
    run_simple_regrets.push_back(std::numeric_limits<double>::quiet_NaN());
  }

  //Record the 'initial' regret before the optimization starts.
  if (init_samples_ > 0) {
    auto regret = Regret(BestCurrent());
    run_regrets.push_back(regret);
  }

  timer_.Start();
  for (int i = init_samples_; i < max_samples; i++) {
    auto last = SingleStep();
    run_points.push_back(last);
    auto result = BestCurrent();

    // Calculate normal regret
    auto regret = Regret(result);
    run_regrets.push_back(regret);

    // Calculate simple regret-- only based on best past observation
    double best_simple = std::numeric_limits<double>::infinity();
    for (const auto& pt : run_points) {
      double regret = Regret(pt);
      best_simple = std::min(best_simple, regret);
    }
    run_simple_regrets.push_back(best_simple);
  }
  timer_.Stop();

  //Put this run's regrets into the list of all regrets
  all_regrets_.push_back(run_regrets);
  all_simple_regrets_.push_back(run_simple_regrets);
  all_dists_.push_back(CalcDist(run_points));
  all_points_.push_back(vectord_to_vector(run_points));
}

double SequentialHarness::Regret(const vectord& point)
{
  double diff = fn_.fn_max - fn_.fn(point);
  return diff;
} /* Regret() */

void SequentialHarness::OutputData(nlohmann::json* j)
{
  OutputRegrets(j);
  OutputDists(j);
  OutputPoints(j);
} /* OutputData() */

void SequentialHarness::OutputRegrets(nlohmann::json* j)
{
  (*j)["REGRETS"] = all_regrets_;
  (*j)["SIMPLE_REGRETS"] = all_simple_regrets_;
} /* OutputRegrets() */

void SequentialHarness::OutputDists(nlohmann::json* j)
{
  (*j)["DISTS"] = all_dists_;
} /* OutputDists() */

void SequentialHarness::OutputPoints(nlohmann::json* j)
{
  (*j)["POINTS"] = all_points_;
} /* OutputPoints() */

//Helper functions for CalcDist
namespace {
double sqr_distance(const vectord& a, const vectord& b) 
{
  assert(a.size() == b.size());
  double total = 0.0;
  for (size_t i = 0; i < a.size(); i++) {
    double diff = a(i)-b(i);
    total += diff*diff;
  }
  return total;
}

double distance(const vectord& a, const vectord& b) 
{
  return sqrt(sqr_distance(a, b));
}

using vec_it = std::vector<vectord>::const_iterator;
vectord closest_point(vec_it begin, vec_it end)
{
  assert(begin != end);
  vectord closest_point;
  double closest_sqr = std::numeric_limits<double>::infinity();
  for (auto it = begin; it != end; ++it) {
    double sdist = sqr_distance(*end, *it);
    if (sdist < closest_sqr) {
      closest_point = *it;
      closest_sqr = sdist;
    }
  }
  return closest_point;
}

double closest_point_dist(vec_it begin, vec_it end)
{
  if (begin == end) return std::numeric_limits<double>::quiet_NaN();
  vectord point = closest_point(begin, end);
  return distance(point, *end);
}

}

std::vector<double> SequentialHarness::CalcDist(const std::vector<vectord>& points)
{
  std::vector<double> dists;

  //No distance associated with the initial samples-- just put in NaN
  for (int i = 0; i < init_samples_; i++) {
    dists.push_back(std::numeric_limits<double>::quiet_NaN());
  }

  for (auto it = points.begin(); it != points.end(); ++it) {
    double dist = closest_point_dist(points.begin(), it);
    dists.push_back(dist);
  }

  return dists;
} /* CalcDist() */
