#include "bopt.h"

#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

constexpr int num_it = 25;
constexpr int init_samples = 12;

bopt_params good_params(int seed, int total_samples)
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

  //TODO this is just so that the initial observations are always the same
  params.random_seed = seed; 
  params.n_init_samples = init_samples;
  params.n_iterations = total_samples-params.n_init_samples;
  params.init_method = 3; //Uniform samples
  params.n_iter_relearn = 25; //TODO ???
  params.force_jump = 0; //Don't jump

  params.verbose_level = -1;

  return params;
}

double fn_value(const vectord& result, const Function& fn) 
{
  double diff = fn.fn_max - fn.fn(result);
  return diff;
}

void display_result(size_t step, const vectord& result, const Function& fn, std::ostream& os)
{
  os << step << "\t";
  os << result[0] << "," << result[1] << "\t";
  os << fn_value(result, fn) << endl;
}

void display_avg_result(const vector<vector<double>>& rs, const Function& fn, 
                        std::ostream& os)
{
  os << fn.name << ",BO1," << rs.size() << endl;
  for (const auto& regrets : rs) {
    for (auto it = regrets.begin(); it != regrets.end(); ++it) {
      os << *it;
      if (it+1 != regrets.end()) os << ",";
    }
    os << endl;
  }
}

void eval(std::function<bopt_params (int,int)> param_fn, int total_samples, const Function& objective, std::ofstream& of)
{
  vector<vector<double>> regrets(num_it);
  for (int x = 0; x < num_it; x++) {
    cout << x << " "; cout.flush();
    bopt_params params = param_fn(1338+x, total_samples);
    BOModel model(params, objective); 
    model.initializeOptimization();
    regrets[x].clear();
    for (size_t i = 0; i < init_samples; i++) {
      regrets[x].push_back(std::numeric_limits<double>::quiet_NaN());
    }
    for (size_t i = 0; i < params.n_iterations; i++) {
      model.stepOptimization();
      auto result = model.getFinalResult();
      regrets[x].push_back(fn_value(result, objective));
    }
  }
  display_avg_result(regrets, objective, of);
  cout << endl;
}

void eval_good_bo(const Function& fn, int total_samples, std::ofstream& of)
{
  eval(good_params, total_samples, fn, of);
}
