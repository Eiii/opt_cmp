#include "initbamsooharness.h"

#include "common.h"

/*********************************************************************
* InitBaMSOOHarness Class
*********************************************************************/
InitBaMSOOHarness::InitBaMSOOHarness(const Function& fn, int seed, int init_samples) :
    SOOHarness(fn, seed, "InitBaMSOO"),
    init_samples_(init_samples)
{
} /* BaMSOOHarness() */

void InitBaMSOOHarness::OutputHeader(nlohmann::json* j)
{
  (*j)["FN_NAME"] = fn_.name;
  (*j)["NAME"] = name_;

  //Output extra info
  std::stringstream ss;
  ss << "# ";
  ss << "v1,";
  ss << "seed:" << seed_ << ",";
  ss << "init_samples:" << init_samples_;
  (*j)["VERSION"] = ss.str();
} /* OutputHeader() */

int InitBaMSOOHarness::GetNumSamples(const cpplogo::RandomSOO* soo) const
{
  return SOOHarness::GetNumSamples(soo)+init_samples_;
} /* PrepareRunRegrets() */

std::unique_ptr<cpplogo::RandomSOO> InitBaMSOOHarness::CreateOptimizer(int run_seed, int max_samples) const
{
  cpplogo::InitBaMSOO::Options options(fn_.fn, fn_.dim, max_samples, 3, run_seed, init_samples_);
  return std::unique_ptr<cpplogo::InitBaMSOO>(new cpplogo::InitBaMSOO(options));
} /* CreateOptimizer() */
