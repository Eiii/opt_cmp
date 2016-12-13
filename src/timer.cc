#include "timer.h"

#include <cassert>
#include <sys/resource.h>

#include <iostream>

CPUTimer::CPUTimer() :
  is_running(false), has_started(false), start_ru(), total_time(0.0)
{
} /* CPUTimer() */

void CPUTimer::Start()
{
  assert(!is_running);
  is_running = true;
  has_started = true;
  getrusage(RUSAGE_SELF, &start_ru);
} /* Start() */

void CPUTimer::Stop()
{
  assert(is_running);

  struct rusage end_ru;
  getrusage(RUSAGE_SELF, &end_ru);
  auto u_sec_diff = end_ru.ru_utime.tv_sec - start_ru.ru_utime.tv_sec;
  auto u_us_diff = end_ru.ru_utime.tv_usec - start_ru.ru_utime.tv_usec;
  auto s_sec_diff = end_ru.ru_stime.tv_sec - start_ru.ru_stime.tv_sec;
  auto s_us_diff = end_ru.ru_stime.tv_usec - start_ru.ru_stime.tv_usec;

  double result = u_us_diff + s_us_diff;
  result += (u_sec_diff + s_sec_diff) * 1e6;

  total_time += result;
  is_running = false;
} /* Stop() */

void CPUTimer::Reset()
{
  assert(!is_running);
  has_started = false;
  total_time = 0.0;
} /* Stop() */

double CPUTimer::ElapsedTime() const
{
  assert(has_started);
  return total_time;
} /* ElapsedTime() */
