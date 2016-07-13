#include "bopt.h"
#include "common.h"

#include <iostream>

using std::cout;
using std::endl;

bopt_params default_params(int seed)
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
  params.l_all = 0;

  //TODO this is just so that the initial observations are always the same
  params.random_seed = seed; 
  params.n_init_samples = 2;
  params.n_iterations = TOTAL_SAMPLES-params.n_init_samples;
  params.init_method = 3; //Uniform samples
  params.n_iter_relearn = 100; //TODO ???
  params.force_jump = 0; //Don't jump

  params.verbose_level = -1;

  return params;
}

double sample(const boost::numeric::ublas::vector<double> &query)
{
  double pt[2];
  pt[0] = query[0]; pt[1] = query[1];
  return -FN(pt);
}

void display_result(size_t step, const vectord& result, std::ostream& os)
{
  os << step << "\t";
  os << result[0] << "," << result[1] << "\t";
  double regret = FN_MAX - (-sample(result));
  os << regret << endl;
}

void eval_bo(std::ofstream &of)
{
  bopt_params params = default_params(1337);
  BOModel model(params); 
  model.initializeOptimization();
  for (size_t i = 0; i < params.n_iterations; i++) {
    model.stepOptimization();
    auto result = model.getFinalResult();
    display_result(i, result, of);
  }
}
