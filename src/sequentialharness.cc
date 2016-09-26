#include "sequentialharness.h"
#include "common.h"

SequentialHarness::SequentialHarness(std::string name, const Function& fn, 
                                     int seed, int init_samples) :
    Harness(name, fn, seed), 
    num_data_sections_(2),
    init_samples_(init_samples),
    all_regrets_(), 
    all_dists_(),
    all_points_()
{
} /* SequentialHarness() */

void SequentialHarness::Evaluate(int max_samples, int iterations)
{
  for (int i = 0; i < iterations; i++) {
    RandomInt rand_seed(rng_, UniformIntDist(0, std::numeric_limits<int>::max()));
    SingleRun(rand_seed(), max_samples);
  }
} /* Evaluate() */

void SequentialHarness::SingleRun(int run_seed, int max_samples)
{
  std::vector<double> run_regrets;
  std::vector<vectord> run_points;

  InitEvaluation(run_seed, max_samples);

  //No regret associated with the initial samples-- just put in NaN
  for (int i = 0; i < init_samples_; i++) {
    run_regrets.push_back(std::numeric_limits<double>::quiet_NaN());
  }

  for (int i = init_samples_; i < max_samples; i++) {
    auto last = SingleStep();
    run_points.push_back(last);
    auto result = BestCurrent();
    auto regret = Regret(result);
    run_regrets.push_back(regret);
  }

  //Put this run's regrets into the list of all regrets
  all_regrets_.push_back(run_regrets);
  all_dists_.push_back(CalcDist(run_points));
}

double SequentialHarness::Regret(const vectord& point)
{
  double diff = fn_.fn_max - fn_.fn(point);
  return diff;
} /* Regret() */

void SequentialHarness::OutputData(std::ofstream* of)
{
  OutputRegrets(of);
  OutputDists(of);
} /* OutputData() */

void SequentialHarness::OutputRegrets(std::ofstream* of)
{
  *of << "REGRETS," << all_regrets_.size() << std::endl;
  output_csv(all_regrets_, of);
} /* OutputRegrets() */

void SequentialHarness::OutputDists(std::ofstream* of)
{
  *of << "DISTS," << all_dists_.size() << std::endl;
  output_csv(all_dists_, of);
} /* OutputDists() */

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
