#pragma once

#include "bayesopt.hpp"
#include "common.h"

double sample(const boost::numeric::ublas::vector<double> &query);

class BOModel : public bayesopt::ContinuousModel
{
  public:
    BOModel(bopt_params params) : 
      ContinuousModel(2, params) {}

    double evaluateSample(const boost::numeric::ublas::vector<double> &query)
    {
      return sample(query);
    }

    bool checkReachability(const boost::numeric::ublas::vector<double> &query)
    {
      (void)query;
      return true;
    }

};

void eval_bo();
