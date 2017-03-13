#include "sequentialharness.h"
#include "common.h"

#include <iostream>
#include <chrono>
#include <ctime>

SequentialHarness::SequentialHarness(std::string name, const Function& fn, 
                                     int seed, int init_samples) :
    Harness(name, fn, seed), 
    init_samples_(init_samples),
    all_regrets_(), 
    all_run_times_(),
    all_points_()
{
} /* SequentialHarness() */

void SequentialHarness::Evaluate(int max_samples, int iterations)
{
  Harness::Evaluate(max_samples, iterations);
  for (int i = 0; i < iterations; i++) {
    try {
      RandomInt rand_seed(rng_, UniformIntDist(0, std::numeric_limits<int>::max()));
      SingleRun(rand_seed(), max_samples);

      auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      std::cout << std::put_time(std::localtime(&now), "%F %T") << " - ";
      std::cout << "Finished iteration " << (i+1) << std::endl;
      std::cout.flush();
    } catch (const std::exception& e) {
      auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      std::cout << std::put_time(std::localtime(&now), "%F %T") << " - ";
      std::cout << "EXCEPTION @ " << i << ": " << e.what() << std::endl;
    }
  }
} /* Evaluate() */

void SequentialHarness::SingleRun(int run_seed, int max_samples)
{
  ResetTimers();
  InitEvaluation(run_seed, max_samples);
  InitRunLists();

  for (int i = init_samples_; i < max_samples; i++) {
    timer_.Start();
    const auto last = SingleStep();
    timer_.Stop();
    const auto best_current = BestCurrent();

    UpdateRunLists(last, best_current);
  }

  //Put this run's regrets into the list of all regrets
  CommitRunLists();
}

double SequentialHarness::Regret(const vectord& point)
{
  double diff = fn_.fn_max - objective_(point);
  return diff;
} /* Regret() */

void SequentialHarness::OutputData(nlohmann::json* j)
{
  OutputRegrets(j);
  OutputRunTimes(j);
  OutputObjTimes(j);
  OutputPoints(j);
} /* OutputData() */

void SequentialHarness::OutputRegrets(nlohmann::json* j)
{
  (*j)["REGRETS"] = all_regrets_;
  (*j)["SIMPLE_REGRETS"] = all_simple_regrets_;
} /* OutputRegrets() */

void SequentialHarness::OutputRunTimes(nlohmann::json* j)
{
  (*j)["RUNTIMES"] = all_run_times_;
} /* OutputRunTimes() */

void SequentialHarness::OutputObjTimes(nlohmann::json* j)
{
  (*j)["OBJ_RUNTIMES"] = all_obj_times_;
} /* OutputObjTimes() */

void SequentialHarness::OutputPoints(nlohmann::json* j)
{
  (*j)["POINTS"] = all_points_;
} /* OutputPoints() */

void SequentialHarness::ResetTimers()
{
  timer_.Reset();
  objective_timer.Reset();
} /* ResetTimers() */

void SequentialHarness::InitRunLists()
{
  run_regrets_.clear();
  run_simple_regrets_.clear();
  run_times_.clear();
  run_obj_times_.clear();
  run_points_.clear();

  //No regret associated with the initial samples-- just put in NaN
  for (int i = 0; i < init_samples_-1; i++) {
    run_regrets_.push_back(std::numeric_limits<double>::quiet_NaN());
    run_simple_regrets_.push_back(std::numeric_limits<double>::quiet_NaN());
  }

  //Record the 'initial' regret before the optimization starts.
  if (init_samples_ > 0) {
    auto regret = Regret(BestCurrent());
    run_regrets_.push_back(regret);

    auto simple_regret = Regret(BestCurrentSimple());
    run_simple_regrets_.push_back(simple_regret);
  }

} /* InitRunLists() */

void SequentialHarness::UpdateRunLists(const vectord& last, const vectord& best_current)
{
  // Calculate normal regret
  const auto regret = Regret(best_current);
  run_regrets_.push_back(regret);

  // Calculate simple regret-- only based on best past observation
  double best_simple = std::numeric_limits<double>::infinity();
  if (!run_simple_regrets_.empty() && !isnan(run_simple_regrets_.back())) best_simple = run_simple_regrets_.back();
  best_simple = std::min(best_simple, Regret(last));
  run_simple_regrets_.push_back(best_simple);

  run_times_.push_back(timer_.ElapsedTime());
  run_obj_times_.push_back(objective_timer.ElapsedTime());
  run_points_.push_back(last);
} /* UpdateRunLists() */

void SequentialHarness::CommitRunLists()
{
  all_regrets_.push_back(run_regrets_);
  all_simple_regrets_.push_back(run_simple_regrets_);
  all_run_times_.push_back(run_times_);
  all_obj_times_.push_back(run_obj_times_);
  all_points_.push_back(vectord_to_vector(run_points_));
} /* CommitRunLists() */
