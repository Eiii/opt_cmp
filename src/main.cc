#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>
#include <vector>
#include <deque>
#include <memory>

#include "functions.h"
#include "common.h"
#include "boharness.h"
#include "fixedboharness.h"
#include "sooharness.h"
#include "logoharness.h"
#include "bamsooharness.h"
#include "initbamsooharness.h"
#include "randomharness.h"
#include "bamlogoharness.h"

#include "cpplogo/logging.h" //TODO: This is just so we can disable logging!

constexpr int SAMPLES = 500000;
constexpr int MAIN_SEED = 1337;

std::vector<const Function*> all_functions = { 
  &f_sin_1, &f_sin_2,
  &f_peaks,
  &f_branin,
  &f_rosenbrock_2,
  &f_hartman_3,
  &f_shekel_5, 
  &f_shekel_7, 
  &f_shekel_10,
  &f_hartman_6,
  &f_rosenbrock_10
};

std::vector<const Function*> functions = { 
  &f_rosenbrock_2,
  &f_hartman_3,
  /*
  &f_shekel_7, 
  &f_rosenbrock_10
  */
};

Function lipschitz(const Function* fn)
{
  Function lp_fn = *fn;

  ObjectiveFn obj = fn->fn;
  vectord max_loc = vector_to_vectord(fn->max_loc);
  double max_val = fn->fn_max;
  ObjectiveFn lp_obj = [obj, max_loc, max_val](const vectord& x) -> double {
    double ydist = std::fabs(obj(x) - max_val);
    double sum = 0.0;
    for (size_t i = 0; i < x.size(); i++) {
      sum += std::pow(x[i] - max_loc[i], 2.0);  
    }
    double xdist = std::sqrt(sum);
    return ydist/xdist;
  };

  lp_fn.fn = lp_obj;
  lp_fn.fn_max = 0.0;
  lp_fn.max_loc.clear();
  return lp_fn;
}

void run_soo(const Function* fn)
{
  cpplogo::RandomSOO::Options opt(fn->fn, fn->dim, SAMPLES, 3, MAIN_SEED);
  auto soo = std::unique_ptr<cpplogo::RandomSOO>(new cpplogo::RandomSOO(opt));
  soo->Optimize();
  auto result = soo->BestNode();
  std::cout << result->value() << std::endl;
  std::cout << result->Center() << std::endl;
}

void comp() 
{
  for (const auto& fn : functions) {
    Function lp = lipschitz(fn);
    /*
    std::cout << fn->name << std::endl;
    run_soo(fn);
    */
    std::cout << fn->name << " - LP" << std::endl;
    run_soo(&lp);
  }
}

using ArgDeque = std::deque<const char*>;

const Function* get_function(ArgDeque* args)
{
  const char* fn_name = args->front();
  args->pop_front();
  for (const Function* fn : all_functions) {
    if (fn->name == fn_name) return fn;
  }
  //Throw?
  throw std::invalid_argument("Unknown function");
  return nullptr;
}

using HarnessPtr = std::unique_ptr<Harness>;
using ArgsResult = std::tuple<std::string, HarnessPtr, int, int>;
HarnessPtr create_bo(int seed, const Function& fn, 
                                   ArgDeque* args) 
{
  if (args->size() < 5) {
    throw std::invalid_argument("Bad number of algorithm arguments.");
  }

  //Criteria
  std::string crit = args->front();
  args->pop_front();

  //Kernel
  std::string kernel = args->front();
  args->pop_front();

  //Surrogate
  std::string surr = args->front();
  args->pop_front();

  //Relearn #
  int relearn = std::stoi(args->front());
  args->pop_front();

  //Initial samples
  int init = std::stoi(args->front());
  args->pop_front();

  //Parameters
  auto params = BOHarness::BOParams(crit, kernel, surr, relearn, init);

  std::cout << "BO" << std::endl;
  std::unique_ptr<Harness> bo;
  bo.reset(new BOHarness(fn, seed, params));
  return bo;
}

HarnessPtr create_fixed_bo(int seed, const Function& fn, 
                                   ArgDeque* args) 
{
  if (args->size() < 4) {
    throw std::invalid_argument("Bad number of algorithm arguments.");
  }

  //Criteria
  std::string crit = args->front();
  args->pop_front();

  //Kernel
  std::string kernel = args->front();
  args->pop_front();

  //Surrogate
  std::string surr = args->front();
  args->pop_front();

  //Relearn #
  int relearn = std::stoi(args->front());
  args->pop_front();

  //Parameters
  auto params = FixedBOHarness::FixedBOParams(crit, kernel, surr, relearn);

  std::cout << "FIXEDBO" << std::endl;
  std::unique_ptr<Harness> bo;
  bo.reset(new FixedBOHarness(fn, seed, params));
  return bo;
}

HarnessPtr create_soo(int seed, const Function& fn, ArgDeque* args) 
{
  if (args->size() != 0) {
    throw std::invalid_argument("Bad number of algorithm arguments.");
  }
  std::cout << "SOO" << std::endl;
  std::unique_ptr<Harness> soo;
  soo.reset(new SOOHarness(fn, seed));
  return soo;
}

HarnessPtr create_logo(int seed, const Function& fn, ArgDeque* args) 
{
  if (args->size() != 0) {
    throw std::invalid_argument("Bad number of algorithm arguments.");
  }
  std::cout << "LOGO" << std::endl;
  std::unique_ptr<Harness> logo;
  logo.reset(new LOGOHarness(fn, seed));
  return logo;
}

HarnessPtr create_bamsoo(int seed, const Function& fn, ArgDeque* args) 
{
  if (args->size() != 0) {
    throw std::invalid_argument("Bad number of algorithm arguments.");
  }
  std::cout << "BAMSOO" << std::endl;
  std::unique_ptr<Harness> bamsoo;
  bamsoo.reset(new BaMSOOHarness(fn, seed));
  return bamsoo;
}

HarnessPtr create_init_bamsoo(int seed, const Function& fn, ArgDeque* args) 
{
  if (args->size() != 1) {
    throw std::invalid_argument("Bad number of algorithm arguments.");
  }

  //Initial Samples
  int init_samples = std::stoi(args->front());
  args->pop_front();

  std::cout << "INITBAMSOO" << std::endl;
  std::unique_ptr<Harness> bamsoo;
  bamsoo.reset(new InitBaMSOOHarness(fn, seed, init_samples));
  return bamsoo;
}

HarnessPtr create_bamlogo(int seed, const Function& fn, ArgDeque* args) 
{
  if (args->size() != 0) {
    throw std::invalid_argument("Bad number of algorithm arguments.");
  }
  std::cout << "BAMLOGO" << std::endl;
  std::unique_ptr<Harness> bamlogo;
  bamlogo.reset(new BaMLOGOHarness(fn, seed));
  return bamlogo;
}

HarnessPtr create_random(int seed, const Function& fn, ArgDeque* args) 
{
  if (args->size() != 0) {
    throw std::invalid_argument("Bad number of algorithm arguments.");
  }
  std::cout << "RANDOM" << std::endl;
  std::unique_ptr<Harness> random;
  random.reset(new RandomHarness(fn, seed));
  return random;
}

HarnessPtr create_harness(int seed, const Function& fn, ArgDeque* args) 
{
  std::string alg_name = args->front();
  args->pop_front();

  std::cout << "Alg: ";
  if (alg_name == "BO1") {
    return create_bo(seed, fn, args);
  } else if (alg_name == "FIXEDBO1") {
    return create_fixed_bo(seed, fn, args);
  } else if (alg_name == "RANDOM") {
    return create_random(seed, fn, args);
  } else if (alg_name == "SOO") {
    return create_soo(seed, fn, args);
  } else if (alg_name == "LOGO") {
    return create_logo(seed, fn, args);
  } else if (alg_name == "BAMSOO") {
    return create_bamsoo(seed, fn, args);
  } else if (alg_name == "INITBAMSOO") {
    return create_init_bamsoo(seed, fn, args);
  } else if (alg_name == "BAMLOGO") {
    return create_bamlogo(seed, fn, args);
  } else {
    throw std::invalid_argument("Unknown algorithm");
  }
  return nullptr;
}

ArgsResult parse_args(int argc, const char* argv[])
{
  ArgDeque args(argv, argv+argc);

  //Get output name
  std::string out_file = args.front();
  args.pop_front();
  std::cout << "Output file: " << out_file << std::endl;

  //Get samples
  int samples = std::stoi(args.front());
  args.pop_front();
  std::cout << "Samples: " << samples << std::endl;

  //Get iterations
  int iterations = std::stoi(args.front());
  args.pop_front();
  std::cout << "Iterations: " << iterations << std::endl;

  //Get seed
  int seed = std::stoi(args.front());
  args.pop_front();
  std::cout << "Seed: " << seed << std::endl;

  //Get function
  const Function& fn = *get_function(&args);
  std::cout << "Fn: " << fn.name << std::endl;

  HarnessPtr harness = create_harness(seed, fn, &args);

  return ArgsResult(out_file, std::move(harness), samples, iterations);
}

int main(int argc, const char* argv[]) 
{
  if (argc == 1) {
    comp();
  } else {
    std::string out_filename;
    HarnessPtr harness;
    int samples, num_it;
    std::tie(out_filename, harness, samples, num_it) = parse_args(argc-1, argv+1);
    harness->Evaluate(samples, num_it); 

    std::ofstream output;
    nlohmann::json json;
    output.open(out_filename);
    harness->OutputResult(&json);
    output << json;
    output.close();
  }
}
