#include "harness.h"

Harness::Harness(std::string name, const Function& fn, int seed) :
    rng_(seed), seed_(seed), name_(name), fn_(fn)
{
} /* Harness() */

void Harness::OutputResult(nlohmann::json* j)
{
  nlohmann::json entry;
  OutputHeader(&entry);
  OutputData(&entry);
  j->push_back(entry);
} /* OutputResult() */

