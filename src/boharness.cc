#include "boharness.h"
#include "bopt.h"
#include "common.h"

using std::get;

BOHarness::BOHarness(const Function& fn, int seed, BOParams params) :
    Harness("BO1", fn, seed), 
    criteria_(get<0>(params)),
    kernel_(get<1>(params)),
    surrogate_(get<2>(params)),
    it_relearn_(get<3>(params)),
    init_samples_(get<4>(params)),
    all_regrets_(), 
    all_dists_()
{
} /* BOHarness() */

void BOHarness::Evaluate(int max_samples, int iterations)
{
  for (int i = 0; i < iterations; i++) {
    SingleRun(i, max_samples);
  }
} /* Evaluate() */

void BOHarness::OutputData(std::ofstream* of)
{
  OutputRegrets(of);
  OutputDists(of);
} /* OutputData() */

void BOHarness::OutputHeader(std::ofstream* of)
{
  constexpr int NUM_SECTIONS = 2;
  *of << fn_.name << "," << name_ << "," << NUM_SECTIONS << std::endl;

  //Output extra info
  *of << "# ";
  *of << "v1,";
  *of << "seed:" << seed_ << ",";
  *of << "crit:" << criteria_ << ",";
  *of << "kernel:" << kernel_ << ",";
  *of << "surrogate:" << surrogate_ << ",";
  *of << "it_relearn:" << it_relearn_ << ",";
  *of << "init_samples:" << init_samples_ << ",";
  *of << std::endl;
} /* OutputData() */

void BOHarness::SingleRun(int run_seed, int iterations)
{
  std::vector<double> run_regrets;
  std::vector<vectord> run_points;
  bopt_params params = CreateParameters(run_seed, iterations);
  BOModel model(params, fn_); 
  model.initializeOptimization();

  //No regret associated with the initial samples-- just put in NaN
  for (int i = 0; i < init_samples_; i++) {
    run_regrets.push_back(std::numeric_limits<double>::quiet_NaN());
  }

  for (size_t i = 0; i < params.n_iterations; i++) {
    model.stepOptimization();
    auto last = model.lastPoint();
    run_points.push_back(last);
    auto result = model.getFinalResult();
    auto regret = Regret(result);
    run_regrets.push_back(regret);
  }

  //Put this run's regrets into the list of all regrets
  all_regrets_.push_back(run_regrets);
  all_dists_.push_back(CalcDist(run_points));
}

bopt_params BOHarness::CreateParameters(int seed, int iterations)
{
  bopt_params params = initialize_parameters_to_default();

  set_criteria(&params, criteria_.c_str());
  params.n_crit_params = 0;

  set_kernel(&params, kernel_.c_str());
  params.kernel.hp_mean[0] = 1.0;
  params.kernel.hp_std[0] = 1.0;
  params.kernel.n_hp = 1;

  set_mean(&params,"mConst"); //Mean 0, std 0
  params.mean.coef_mean[0] = 0.0;
  params.mean.coef_std[0] = 1.0;
  params.mean.n_coef = 1;

  set_surrogate(&params, surrogate_.c_str());
  params.noise = 1e-12;

  params.sc_type = SC_MTL; //Kernel parameter learning type
  params.l_type = L_EMPIRICAL;
  params.l_all = 1;

  params.random_seed = seed; 
  params.n_init_samples = init_samples_;
  params.n_iterations = iterations-params.n_init_samples;
  params.init_method = 3; //Uniform samples
  params.n_iter_relearn = it_relearn_;
  params.force_jump = 0; //Don't jump

  params.verbose_level = -1;

  return params;
} /* CreateParameters() */

double BOHarness::Regret(const vectord& point)
{
  double diff = fn_.fn_max - fn_.fn(point);
  return diff;
} /* Regret() */

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

std::vector<double> BOHarness::CalcDist(const std::vector<vectord>& points)
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

void BOHarness::OutputRegrets(std::ofstream* of)
{
  *of << "REGRETS," << all_regrets_.size() << std::endl;
  output_csv(all_regrets_, of);
} /* OutputRegrets() */

void BOHarness::OutputDists(std::ofstream* of)
{
  *of << "DISTS," << all_dists_.size() << std::endl;
  output_csv(all_dists_, of);
} /* OutputDists() */
