#include "common.h"
#include "bopt.h"

#include <iostream>
#include <vector>

#define STR_(x) #x
#define STR(x) STR_(x)

using std::cout;
using std::endl;
using std::vector;

constexpr int num_it = 10;
constexpr int init_samples = 2;

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
  params.n_init_samples = init_samples;
  params.n_iterations = TOTAL_SAMPLES - params.n_init_samples;
  params.init_method = 3; //Uniform samples
  params.n_iter_relearn = 100; //TODO ???
  params.force_jump = 0; //Don't jump

  params.verbose_level = -1;

  return params;
}

bopt_params good_params(int seed)
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
  params.n_iterations = TOTAL_SAMPLES-params.n_init_samples;
  params.init_method = 3; //Uniform samples
  params.n_iter_relearn = 50; //TODO ???
  params.force_jump = 0; //Don't jump

  params.verbose_level = -1;

  return params;
}

double fn_value(const vectord& result) 
{
  double diff = FN_MAX - (-sample(result));
  if (FN_MAX == 0.0) {
    return diff;
  } else {
    return diff / fabs(FN_MAX);
  }
}

double sample(const boost::numeric::ublas::vector<double> &query)
{
  double pt[FN_DIM];
  for (int i = 0; i < FN_DIM; i++) {
    pt[i] = query[i];
  }
  return -FN(pt);
}

void display_result(size_t step, const vectord& result, std::ostream& os)
{
  os << step << "\t";
  os << result[0] << "," << result[1] << "\t";
  os << fn_value(result) << endl;
}

void display_avg_result(const vector<vector<double>>& rs, std::ostream& os)
{
  os << STR(FN) << ",BO1" << endl;
  for (size_t i = 0; i < rs[0].size(); i++) {
    double avg = 0.0;
    for (size_t j = 0; j < num_it; j++) {
      avg += rs[j][i];
    }
    avg /= (double)num_it;
    os << avg;
    if (i+1 < rs[0].size()) os << ",";
  }
  os << endl;
}

void eval(std::function<bopt_params (int)> fn, std::ofstream& of)
{
  vector<vector<double>> regrets(num_it);
  for (int x = 0; x < num_it; x++) {
    cout << x << " "; cout.flush();
    bopt_params params = fn(1338+x);
    BOModel model(params); 
    model.initializeOptimization();
    regrets[x].clear();
    for (size_t i = 0; i < init_samples; i++) {
      regrets[x].push_back(std::numeric_limits<double>::quiet_NaN());
    }
    for (size_t i = 0; i < params.n_iterations; i++) {
      model.stepOptimization();
      auto result = model.getFinalResult();
      regrets[x].push_back(fn_value(result));
    }
  }
  display_avg_result(regrets, of);
  cout << endl;
}

double eval_final(std::function<bopt_params (int)> fn)
{
  vector<double> regrets(num_it);
  for (int x = 0; x < num_it; x++) {
    cout << x << " "; cout.flush();
    bopt_params params = fn(1338+x);
    BOModel model(params); 
    model.initializeOptimization();
    for (size_t i = 0; i < params.n_iterations; i++) {
      model.stepOptimization();
    }
    auto result = model.getFinalResult();
    regrets.push_back(fn_value(result));
  }
  cout << endl;
  
  double f = 0.0;
  for (const auto& r : regrets) {
    f += r;
  }
  f /= (double)num_it;

  return f;
}


void eval_bo(std::ofstream& of)
{
  eval(default_params, of);
}

void eval_good_bo(std::ofstream& of)
{
  eval(good_params, of);
}

double eval_good_bo_final()
{
  return eval_final(good_params);
}
