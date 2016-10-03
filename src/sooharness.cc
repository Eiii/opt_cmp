#include "sooharness.h"

#include "common.h"
#include "cpplogo/logo.h"

/*********************************************************************
* LOGO Settings
* Copied from cpplogo/settings.h
*********************************************************************/
namespace {
Settings s_soo = {
  .name = "SOO",
  .w_sched = [](const logo::LOGO&) {
    return 1;
  },
  .init_w = 1,
};

double h_max(const logo::LOGO& logo) 
{ 
  double n = logo.num_samples();
  //double w = logo.w();
  double w = 1.0;
  double val = w * sqrt(n);
  return std::max(val, 0.0); 
}
}

/*********************************************************************
* SOOHarness Class
*********************************************************************/
SOOHarness::SOOHarness(const Function& fn, int seed) :
    Harness("SOO", fn, seed), all_regrets_(), setting_(s_soo)
{
} /* SOOHarness() */

void SOOHarness::Evaluate(int max_samples, int iterations)
{
  for (int i = 0; i < iterations; i++) {
    RandomInt rand_seed(rng_, UniformIntDist(0, std::numeric_limits<int>::max()));
    SingleRun(rand_seed(), max_samples);
  }
} /* Evaluate() */

void SOOHarness::OutputData(std::ofstream* of)
{
  OutputRegrets(of);
  OutputWs(of);
} /* OutputData() */

void SOOHarness::OutputHeader(std::ofstream* of)
{
  constexpr int NUM_SECTIONS = 2;
  *of << fn_.name << "," << name_ << "," << NUM_SECTIONS << std::endl;

  //Output extra info
  *of << "# ";
  *of << "v1,";
  *of << "seed:" << seed_;
  *of << std::endl;
} /* OutputHeader() */

void SOOHarness::SingleRun(int run_seed, int max_samples)
{
  std::vector<std::tuple<int, double>> run_regrets;
  std::vector<std::tuple<int, double>> run_ws;

  auto options = BuildOptions(setting_, max_samples, run_seed);
  logo::LOGO logo(options);

  while (!logo.IsDone()) {
    logo.Step();
    int samples = logo.library()->NumSamples();
    double regret = Regret(logo);
    double w = static_cast<double>(logo.w());
    run_regrets.push_back(std::make_pair(samples, regret));
    run_ws.push_back(std::make_pair(samples, w));
  }

  //Put this run's regrets into the list of all regrets
  all_regrets_.push_back(DenseValues(run_regrets, max_samples));
  all_ws_.push_back(DenseValues(run_ws, max_samples));
}

double SOOHarness::Regret(const logo::LOGO& logo) const
{
  double regret = fn_.fn_max - logo.library().BestObservation();
  return regret;
} /* Regret() */

void SOOHarness::OutputRegrets(std::ofstream* of) const
{
  *of << "REGRETS," << all_regrets_.size() << std::endl;
  output_csv(all_regrets_, of);
} /* OutputRegrets() */

void SOOHarness::OutputWs(std::ofstream* of) const
{
  *of << "WS," << all_ws_.size() << std::endl;
  output_csv(all_ws_, of);
} /* OutputWs() */

logo::Options SOOHarness::BuildOptions(const Settings& opt, int max, int seed) const
{
  logo::Options o = {
    .seed = seed,
    .max_samples = max,
    .k = 3,
    .min_error = -std::numeric_limits<double>::infinity(),
    .dim = fn_.dim,
    .fn = fn_.fn,
    .fn_max = fn_.fn_max,
    .w_sched = opt.w_sched,
    .init_w = opt.init_w,
    .h_max = h_max,
  };
  return o;
} /* BuildOptions() */

std::vector<double> SOOHarness::DenseValues(std::vector<std::tuple<int, double>> regrets, int max_samples)
{
  std::vector<double> result(max_samples);
  auto reg_entry = regrets.begin();
  double val = std::numeric_limits<double>::quiet_NaN();
  for (int i = 0; i < max_samples; i++) {
    if (reg_entry != regrets.end() && i == std::get<0>(*reg_entry)) {
      val = std::get<1>(*reg_entry);
      reg_entry++;
    }
    result[i] = val;
  }
  return result;
} /* DenseRegrets() */
