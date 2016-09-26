#include "boharness.h"
#include "common.h"

using std::get;

BOHarness::BOHarness(const Function& fn, int seed, BOParams params) :
    SequentialHarness("BO1", fn, seed, get<4>(params)), 
    criteria_(get<0>(params)),
    kernel_(get<1>(params)),
    surrogate_(get<2>(params)),
    it_relearn_(get<3>(params)),
    current_model_(nullptr)
{
} /* BOHarness() */

void BOHarness::InitEvaluation(int run_seed, int max_samples)
{
  bopt_params params = CreateParameters(run_seed, max_samples);
  BOModel model(params, fn_); 
  current_model_.reset(new BOModel(params, fn_));
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

void BOHarness::OutputHeader(std::ofstream* of)
{
  *of << fn_.name << "," << name_ << "," << num_data_sections_ << std::endl;

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
