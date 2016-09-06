#pragma once

#include "functions.h"
#include "bayesopt.hpp"

#include <fstream>

class BOModel : public bayesopt::ContinuousModel
{
  public:
    BOModel(bopt_params params, const Function& fn) : 
      ContinuousModel(fn.dim, params), fn_(fn) {}

    double evaluateSample(const boost::numeric::ublas::vector<double> &query)
    {
      return -fn_.fn(query);
    }

    bool checkReachability(const boost::numeric::ublas::vector<double> &query)
    {
      (void)query;
      return true;
    }

    const Function& fn() const { return fn_; }

  protected:
    const Function& fn_;

};

void eval_bo(const Function& fn, std::ofstream& os);
void eval_good_bo(const Function& fn, std::ofstream& os);
