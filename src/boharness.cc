#include "boharness.h"
#include "common.h"
#include "boharness_timers.h"
#include "dataset.hpp"

using std::get;

CPUTimer model_timer;
CPUTimer crit_timer;

BOHarness::BOHarness(const Function& fn, int seed, BOParams params, std::string name) :
    SequentialHarness(name, fn, seed, get<4>(params)), 
    criteria_(get<0>(params)),
    kernel_(get<1>(params)),
    surrogate_(get<2>(params)),
    it_relearn_(get<3>(params)),
    current_model_(nullptr),
    all_model_times_(),
    all_crit_times_()
{
} /* BOHarness() */

void BOHarness::InitEvaluation(int run_seed, int max_samples)
{
  bopt_params params = CreateParameters(run_seed, max_samples);
  current_model_.reset(new BOModel(params, fn_, objective_));
  current_model_->initializeOptimization();
} /* InitEvaluation() */

vectord BOHarness::SingleStep()
{
    current_model_->stepOptimization();
    return current_model_->lastPoint();
} /* SingleStep() */

vectord BOHarness::BestCurrent()
{
  return current_model_->getFinalResult();
} /* BestCurrent() */

vectord BOHarness::BestCurrentSimple()
{
  auto dataset = current_model_->getData();
  size_t count = dataset->getNSamples();
  double best = std::numeric_limits<double>::infinity();
  int best_idx = -1;
  for (size_t i = 0; i < count; i++) {
    double y = dataset->getSampleY(i);
    if (y < best) {
      best = y;
      best_idx = i;
    }
  }
  assert(best_idx != -1);
  return dataset->getSampleX(best_idx);
} /* BestCurrentSimple() */

void BOHarness::ResetTimers()
{
  SequentialHarness::ResetTimers();
  model_timer.Reset();
  crit_timer.Reset();
} /* ResetTimers() */

void BOHarness::InitRunLists()
{
  SequentialHarness::InitRunLists();
  run_model_times_.clear();
  run_crit_times_.clear();
} /* InitRunLists() */

void BOHarness::UpdateRunLists(const vectord& last, const vectord& best_current)
{
  SequentialHarness::UpdateRunLists(last, best_current);
  run_model_times_.push_back(model_timer.ElapsedTime());
  run_crit_times_.push_back(crit_timer.ElapsedTime());
} /* UpdateRunLists() */

void BOHarness::CommitRunLists()
{
  SequentialHarness::CommitRunLists();
  all_model_times_.push_back(run_model_times_);
  all_crit_times_.push_back(run_crit_times_);
} /* CommitRunLists() */

void BOHarness::OutputData(nlohmann::json* json)
{
  SequentialHarness::OutputData(json);
  OutputModelTime(json);
  OutputCritTime(json);
} /* OutputData() */

void BOHarness::OutputHeader(nlohmann::json* j)
{
  (*j)["FN_NAME"] = fn_.name;
  (*j)["NAME"] = name_;

  //Output extra info
  std::stringstream ss;
  ss << "# ";
  ss << "v1,";
  ss << "seed:" << seed_ << ",";
  ss << "crit:" << criteria_ << ",";
  ss << "kernel:" << kernel_ << ",";
  ss << "surrogate:" << surrogate_ << ",";
  ss << "it_relearn:" << it_relearn_ << ",";
  ss << "init_samples:" << init_samples_ << ",";
  (*j)["VERSION"] = ss.str();
} /* OutputHeader() */

void BOHarness::OutputModelTime(nlohmann::json* j)
{
  (*j)["MODEL_TIMES"] = all_model_times_;
} /* OutputModelTime() */

void BOHarness::OutputCritTime(nlohmann::json* j)
{
  (*j)["CRIT_TIMES"] = all_crit_times_;
} /* OutputCritTime() */

bopt_params BOHarness::CreateParameters(int seed, int iterations)
{
  bopt_params params = initialize_parameters_to_default();

  const std::string beta_prefix = "cLCB-";
  if (criteria_.find(beta_prefix) == 0) {
    set_criteria(&params, "cLCB");
    std::string beta_str = criteria_.substr(beta_prefix.size());
    double beta = std::stod(beta_str);
    params.n_crit_params = 1;
    params.crit_params[0] = beta;
  } else {
    set_criteria(&params, criteria_.c_str());
    params.n_crit_params = 0;
  }

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
