#pragma once

#include "function.h"
#include "bayesopt/bayesopt.hpp"

class BOModel : public bayesopt::ContinuousModel
{
  public:
    BOModel(bopt_params params, const Function& fn, const ObjectiveFn& obj) : 
      ContinuousModel(fn.dim, params), obj_(obj) {}

    double evaluateSample(const boost::numeric::ublas::vector<double> &query)
    {
      return -obj_(query);
    }

    bool checkReachability(const boost::numeric::ublas::vector<double> &query)
    {
      (void)query;
      return true;
    }

  protected:
    const ObjectiveFn& obj_;

};
