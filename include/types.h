/*********************************************************************
* INCLUDES
*********************************************************************/
#pragma once

/*********************************************************************
* INCLUDES
*********************************************************************/
#include <map>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/random.hpp>

/*********************************************************************
* TYPES
*********************************************************************/
using matrixd = boost::numeric::ublas::matrix<double>;
using vectord = boost::numeric::ublas::vector<double>;

using RandomEngine = boost::mt19937;
using UniformIntDist = boost::uniform_int<>;
using UniformRealDist = boost::uniform_real<>;
using NormalDist = boost::normal_distribution<>;
using RandomInt = boost::variate_generator<RandomEngine&, UniformIntDist>;
using RandomReal = boost::variate_generator<RandomEngine&, UniformRealDist>;
using NormalReal = boost::variate_generator<RandomEngine&, NormalDist>;
using ObjectiveFn = std::function<double(const vectord&)>;
