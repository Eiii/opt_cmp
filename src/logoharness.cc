#include "logoharness.h"

#include "common.h"
#include "cpplogo/logo.h"

/*********************************************************************
* LOGO Settings
* Copied from cpplogo/settings.h
*********************************************************************/
Settings s_logo = {
  .name = "LOGO",
  .w_sched = [](const logo::LOGO& logo) {
    constexpr std::array<size_t, 6> w{{3, 4, 5, 6, 8, 30}};
    size_t idx = std::find(w.cbegin(), w.cend(), logo.w()) - w.cbegin();
    assert(0 <= idx && idx < w.size());
    if (logo.PrevBestIterationObservation(0) >= logo.PrevBestIterationObservation(1)) {
      if (idx < w.size()-1) idx++;
    } else {
      if (idx > 0) idx--; 
    }
    return w[idx];
  },
  .init_w = 3,
};

double h_max(const logo::LOGO& logo) 
{ 
  double n = logo.num_samples();
  //double w = logo.w();
  double w = 1.0;
  double val = w * sqrt(n);
  return std::max(val, 0.0); 
}


/*********************************************************************
* LOGOHarness Class
*********************************************************************/
constexpr int NUM_SECTIONS = 1;

LOGOHarness::LOGOHarness(const Function& fn, int seed) :
    Harness("LOGO", fn, seed), all_regrets_(), setting_(s_logo)
{
} /* LOGOHarness() */

void LOGOHarness::Evaluate(int max_samples, int iterations)
{
  for (int i = 0; i < iterations; i++) {
    SingleRun(i, max_samples);
  }
} /* Evaluate() */

void LOGOHarness::OutputResult(std::ofstream* of)
{
  *of << fn_.name << "," << name_ << "," << NUM_SECTIONS << std::endl;
  OutputRegrets(of);
} /* OutputResult() */

void LOGOHarness::SingleRun(int run_seed, int max_samples)
{
  std::vector<std::tuple<int, double>> run_regrets;

  auto options = BuildOptions(s_logo, max_samples, run_seed);
  logo::LOGO logo(options);

  while (!logo.IsDone()) {
    logo.Step();
    int samples = logo.library().NumSamples();
    double regret = Regret(logo);
    run_regrets.push_back(std::make_pair(samples, regret));
  }

  //Put this run's regrets into the list of all regrets
  all_regrets_.push_back(DenseRegrets(run_regrets, max_samples));
}

double LOGOHarness::Regret(const logo::LOGO& logo) const
{
  double regret = fn_.fn_max - logo.library().BestObservation();
  return regret;
} /* Regret() */

void LOGOHarness::OutputRegrets(std::ofstream* of) const
{
  *of << "REGRETS," << all_regrets_.size() << std::endl;
  output_csv(all_regrets_, of);
} /* OutputRegrets() */

logo::Options LOGOHarness::BuildOptions(const Settings& opt, int max, int seed) const
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

std::vector<double> LOGOHarness::DenseRegrets(std::vector<std::tuple<int, double>> regrets, int max_samples)
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
