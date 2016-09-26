#include "harness.h"

Harness::Harness(std::string name, const Function& fn, int seed) :
    rng_(seed), seed_(seed), name_(name), fn_(fn)
{
} /* Harness() */

void Harness::OutputResult(std::ofstream* of)
{
  OutputHeader(of);
  OutputData(of);
} /* OutputResult() */
