#include "cpplogo/main.h"

#include "cpplogo/evaluator.h"

#include "cpplogo/logo.h"
#include "cpplogo/logoset.h"

#include <cassert>
#include <fstream>
#include <iomanip>


using std::vector;
using namespace logo;

constexpr int max_iterations = 500;

/*********************************************************************
* EVALUATOR CLASS
*********************************************************************/

/***********************************************************
* Evaluator
* Constructor
***********************************************************/
Evaluator::Evaluator(const Function& fn, const vector<Settings>& sets) :
  rng_(), max_iterations_(max_iterations), inited_(false), fn_(fn), 
  settings_(sets), logoset_(nullptr), num_steps_(), regrets_()
{

} /* Evaluator() */

/***********************************************************
* RunMany
* Initialize and run several things
***********************************************************/
void Evaluator::RunMany(int num)
{
  rng_.seed(1337);
  RandomInt rand_seed(rng_, UniformIntDist(0, 10000));

  for (int i = 0; i < num; i++) {
    Init(rand_seed());
    RunOne();
  }
} /* RunMany() */

/***********************************************************
* Init
* Initialize the thing
***********************************************************/
void Evaluator::Init(int seed)
{

  logoset_.reset(new LOGOSet(fn_.fn, seed));
  for (const auto& setting : settings_) {
    Options logo_opt = build_options(setting, fn_, max_iterations_, seed);
    logoset_->AddLOGO(logo_opt);
  }
  logoset_->Finalize();

  regrets_.emplace_back();

  inited_ = true;
} /* Init() */

/***********************************************************
* RunOne
* Execute the thing
***********************************************************/
void Evaluator::RunOne()
{
  assert(inited_); 
  while (!logoset_->IsFinished()) {
    logoset_->Step();

    //Calculate regret at this step
    int num_steps = logoset_->library().NumSamples();
    double regret = fn_.fn_max - logoset_->library().BestObservation();
    regrets_.back().emplace_back(std::make_pair(num_steps, regret));
  }

  int steps = logoset_->library().NumSamples();
  num_steps_.push_back(steps);
  inited_ = false;
} /* RunOne() */

/***********************************************************
* Name
* Calculate its name
***********************************************************/
std::string Evaluator::Name() const
{
  std::stringstream ss;
  for (auto it = settings_.cbegin(); it != settings_.cend(); ++it) {
    ss << it->name;
    if (it+1 != settings_.cend()) ss << "+";
  }
  return ss.str();
} /* Name() */

/***********************************************************
* CalcResult
* Fills out the result variables after many successful runs
***********************************************************/
double Evaluator::AvgNumSteps() const
{
  assert(!inited_);
  double sum = 0.0;
  for (const auto& s : num_steps_) sum += s;
  return sum / static_cast<double>(num_steps_.size());
} /* AvgNumSteps() */

/***********************************************************
* AvgRegret
* Average regret at each timestep in the set of 
* optimizations.
***********************************************************/
vector<double> Evaluator::AvgRegret() const
{
  int min_end = max_iterations_;
  for (const auto& reg_list : regrets_) {
    int end_step = reg_list.back().first;
    min_end = std::min(end_step, min_end);
  }

  vector<double> avg_regrets(min_end+1);

  for (size_t t = 0; t < avg_regrets.size(); t++) {
    bool valid = false;
    double regret_sum = 0.0;
    int valid_regrets = 0;
    for (const auto& regret_list : regrets_) {
      double r = regret_at_time(regret_list, t);
      if (!std::isinf(r)) {
        valid = true;
        regret_sum += r;
        valid_regrets += 1;
      }
    }
    if (!valid) regret_sum = std::numeric_limits<double>::infinity();
    avg_regrets[t] = regret_sum / valid_regrets;
  }

  return avg_regrets;
} /* AvgRegret() */

/***********************************************************
* AllRegrets
* Creates an easy-to-parse list of all regrets.
***********************************************************/
vector<vector<double>> Evaluator::AllRegrets() const
{
  int min_end = max_iterations_;
  for (const auto& reg_list : regrets_) {
    int end_step = reg_list.back().first;
    min_end = std::min(end_step, min_end);
  }

  vector<vector<double>> all_regrets;

  for (const auto& regret_list : regrets_) {
    vector<double> regrets;
    for (int t = 0; t < min_end; t++) {
      regrets.push_back(regret_at_time(regret_list, t));
    }
    all_regrets.push_back(regrets);
  }
  return all_regrets;
} /* AllRegrets() */

/*********************************************************************
* regret_at_time
* Returns the regret at the given timestep in the regret entry list.
*********************************************************************/
double regret_at_time(const std::vector<std::pair<int, double>>& regret_list, 
                      int time)
{
  double prev_regret = std::numeric_limits<double>::infinity();
  for (const auto& entry : regret_list) {
    int t = entry.first;
    if (t == time) prev_regret = entry.second;  
    if (t >= time) {
      break;
    } else {
      prev_regret = entry.second;
    }

  }
  return prev_regret;
}


/*********************************************************************
* FUNCTIONS
*********************************************************************/

/***********************************************************
* RandomLOGO
* TODO
***********************************************************/
vector<Settings> RandomLogo(int count, int seed)
{
  vector<Settings> settings;
  RandomEngine rng(seed);
  for (int i = 0; i < count; i++) {
    vector<size_t> ws;
    RandomInt rand_num_w(rng, UniformIntDist(3, 10));
    int num_w = rand_num_w();
    size_t last_w = 1;
    RandomInt rand_inc(rng, UniformIntDist(0, 10));
    for (int j = 0; j < num_w; j++) {
      last_w += static_cast<size_t>(rand_inc());
      ws.push_back(last_w);
    }
    settings.emplace_back(logo_custom_w(ws, "Z"));
  }
  return settings;
}

/***********************************************************
* h_max
* LOGO function
***********************************************************/
double h_max(const logo::LOGO& logo) 
{ 
  double n = logo.num_samples();
  //double w = logo.w();
  double w = 1.0;
  double val = w * sqrt(n);
  return std::max(val, 0.0); 
}

/***********************************************************
* build_options
* Build an options structure that represents an optimization
* of the given function with the given settings.
***********************************************************/
logo::Options build_options(const Settings& opt, const Function& fn, 
                            int max, int seed)
{
  logo::Options o = {
    .seed = 1337+seed,
    .max_samples = max,
    .k = 3,
    .min_error = -std::numeric_limits<double>::infinity(),
    .dim = fn.dim,
    .fn = fn.fn,
    .fn_max = fn.fn_max,
    .w_sched = opt.w_sched,
    .init_w = opt.init_w,
    .h_max = h_max,
  };
  return o;
}

/***********************************************************
* logo_custom_w
* Make a LOGO instance with a custom w schedule.
***********************************************************/
Settings logo_custom_w(std::vector<size_t> w, const char* name)
{
  auto w_fn = [w](const LOGO& logo) {
    size_t idx = std::find(w.cbegin(), w.cend(), logo.w()) - w.cbegin();
    assert(0 <= idx && idx < w.size());
    if (logo.PrevBestIterationObservation(0) >= logo.PrevBestIterationObservation(1)) {
      if (idx < w.size()-1) idx++; 
    } else {
      if (idx > 0) idx--;
    }
    return w[idx];
  };

  Settings s = {
    .name = name ? name : "Generated",
    .w_sched = w_fn,
    .init_w = static_cast<int>(w[0]),
  };

  return s;
} /* logo_custom_w() */
