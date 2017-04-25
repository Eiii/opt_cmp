#include "harness.h"

Harness::Harness(std::string name, const Function& fn, int seed) :
    rng_(seed), seed_(seed), name_(name), fn_(tweak_edges(fn, seed)), objective_(create_scaled_fn(fn_)), timer_()
{
} /* Harness() */

void Harness::Evaluate(int max_secs, int iterations)
{
  (void)max_secs; (void)iterations;
} /* Evaluate() */

void Harness::OutputResult(nlohmann::json* j)
{
  nlohmann::json entry;
  OutputHeader(&entry);
  OutputEdges(&entry);
  OutputData(&entry);
  j->push_back(entry);
} /* OutputResult() */

void Harness::OutputEdges(nlohmann::json* j)
{
  (*j)["MIN_EDGES"] = fn_.min_edge;
  (*j)["MAX_EDGES"] = fn_.max_edge;
} /* OutputEdges() */

