#include "harness.h"

Harness::Harness(std::string name, const Function& fn, int seed) :
    rng_(seed), seed_(seed), name_(name), fn_(fn), timer_()
{
} /* Harness() */

void Harness::Evaluate(int max_samples, int iterations)
{
  (void)max_samples; (void)iterations;
} /* Evaluate() */

void Harness::OutputResult(nlohmann::json* j)
{
  nlohmann::json entry;
  OutputHeader(&entry);
  OutputData(&entry);
  j->push_back(entry);
} /* OutputResult() */

