#include "boharness.h"
#include "bopt.h"
#include "common.h"

constexpr int INITIAL_SAMPLES = 2;
constexpr int IT_RELEARN = 50;
constexpr int NUM_SECTIONS = 1;

BOHarness::BOHarness(const Function& fn, int seed) :
    Harness("BO1", fn, seed), all_regrets_()
{
} /* BOHarness() */

void BOHarness::Evaluate(int max_samples, int iterations)
{
  for (int i = 0; i < iterations; i++) {
    SingleRun(i, max_samples);
  }
} /* Evaluate() */

void BOHarness::OutputResult(std::ofstream* of)
{
  *of << fn_.name << "," << name_ << "," << NUM_SECTIONS << std::endl;
  OutputRegrets(of);
} /* OutputResult() */

void BOHarness::SingleRun(int run_seed, int iterations)
{
  std::vector<double> run_regrets;
  bopt_params params = CreateParameters(run_seed, iterations);
  BOModel model(params, fn_); 
  model.initializeOptimization();

  //No regret associated with the initial samples-- just put in NaN
  for (size_t i = 0; i < INITIAL_SAMPLES; i++) {
    run_regrets.push_back(std::numeric_limits<double>::quiet_NaN());
  }

  for (size_t i = 0; i < params.n_iterations; i++) {
    model.stepOptimization();
    auto result = model.getFinalResult();
    auto regret = Regret(result);
    run_regrets.push_back(regret);
  }

  //Put this run's regrets into the list of all regrets
  all_regrets_.push_back(run_regrets);
}

bopt_params BOHarness::CreateParameters(int seed, int iterations)
{
  bopt_params params = initialize_parameters_to_default();

  set_criteria(&params, "cEI");
  params.n_crit_params = 0;

  set_kernel(&params,"kSEISO");
  params.kernel.hp_mean[0] = 1.0;
  params.kernel.hp_std[0] = 1.0;
  params.kernel.n_hp = 1;

  set_mean(&params,"mConst"); //Mean 0, std 0
  params.mean.coef_mean[0] = 0.0;
  params.mean.coef_std[0] = 1.0;
  params.mean.n_coef = 1;

  set_surrogate(&params,"sGaussianProcessML");
  params.noise = 1e-12;

  params.sc_type = SC_MTL; //Kernel parameter learning type
  params.l_type = L_EMPIRICAL;
  params.l_all = 1;

  params.random_seed = seed; 
  params.n_init_samples = INITIAL_SAMPLES;
  params.n_iterations = iterations-params.n_init_samples;
  params.init_method = 3; //Uniform samples
  params.n_iter_relearn = IT_RELEARN;
  params.force_jump = 0; //Don't jump

  params.verbose_level = -1;

  return params;
} /* CreateParameters() */

double BOHarness::Regret(const vectord& point)
{
  double diff = fn_.fn_max - fn_.fn(point);
  return diff;
} /* Regret() */

void BOHarness::OutputRegrets(std::ofstream* of)
{
  *of << "REGRETS," << all_regrets_.size() << std::endl;
  output_csv(all_regrets_, of);
} /* OutputRegrets() */
