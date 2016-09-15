#include "harness.h"

Harness::Harness(std::string name, const Function& fn, int seed) :
    rng_(seed), name_(name), fn_(fn)
{
} /* Harness() */
