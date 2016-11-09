#include "initbamsooharness.h"

#include "common.h"

/***********************************************************
* InitBaMSOO constructor
***********************************************************/
InitBaMSOOHarness::InitBaMSOOHarness(const Function& fn, int seed, int init_samples) :
    SOOHarness(fn, seed, "InitBaMSOO"),
    init_samples_(init_samples)
{
} /* BaMSOOHarness() */

/***********************************************************
* OutputHeader
***********************************************************/
void InitBaMSOOHarness::OutputHeader(nlohmann::json* j)
{
  (*j)["FN_NAME"] = fn_.name;
  (*j)["NAME"] = name_;

  //Output extra info
  std::stringstream ss;
  ss << "# ";
  ss << "v2,";
  ss << "seed:" << seed_ << ",";
  ss << "init_samples:" << init_samples_;
  (*j)["VERSION"] = ss.str();
} /* OutputHeader() */

/***********************************************************
* GetNumSamples
* Add the initial samples to the usual count.
***********************************************************/
int InitBaMSOOHarness::GetNumSamples(const cpplogo::RandomSOO* soo) const
{
  return SOOHarness::GetNumSamples(soo)+init_samples_;
} /* PrepareRunRegrets() */

/***********************************************************
* Regret
* Modified to take into account the initial samples observed
* by the algorithm.
***********************************************************/
double InitBaMSOOHarness::Regret(const cpplogo::RandomSOO& soo) const
{
  // Calculate the 'default' regret
  double regret = SOOHarness::Regret(soo);

  // ...and replace it with the regret of any better initial
  // point
  const auto& bamsoo = dynamic_cast<const cpplogo::InitBaMSOO&>(soo);
  for (const auto& sample : bamsoo.InitialSamples()) {
    const auto& val = sample.second;
    double pt_regret = fn_.fn_max - val;
    regret = std::min(pt_regret, regret);
  }
  return regret;
} /* Regret() */

/***********************************************************
* CreateOptimizer
* Create BaMSOO.
***********************************************************/
std::unique_ptr<cpplogo::RandomSOO> InitBaMSOOHarness::CreateOptimizer(int run_seed, int max_samples) const
{
  cpplogo::InitBaMSOO::Options options(fn_.fn, fn_.dim, max_samples, 3, run_seed, init_samples_);
  return std::unique_ptr<cpplogo::InitBaMSOO>(new cpplogo::InitBaMSOO(options));
} /* CreateOptimizer() */
