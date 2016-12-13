#include "timer.h"

#include <cassert>
#include <sys/resource.h>

CPUTimer::CPUTimer() :
  is_running(false), start_ru()
{
} /* CPUTimer() */

void CPUTimer::Start()
{
  assert(!is_running);
  is_running = true;
  getrusage(RUSAGE_SELF, &start_ru);
} /* Start() */

void CPUTimer::Stop()
{
  assert(is_running);
  is_running = false;
} /* Stop() */

double CPUTimer::ElapsedTime()
{
  struct rusage end_ru;
  getrusage(RUSAGE_SELF, &end_ru);
  auto u_sec_diff = end_ru.ru_utime.tv_sec - start_ru.ru_utime.tv_sec;
  auto u_us_diff = end_ru.ru_utime.tv_usec - start_ru.ru_utime.tv_usec;
  auto s_sec_diff = end_ru.ru_stime.tv_sec - start_ru.ru_stime.tv_sec;
  auto s_us_diff = end_ru.ru_stime.tv_usec - start_ru.ru_stime.tv_usec;

  double result = u_us_diff + s_us_diff;
  result += (u_sec_diff + s_sec_diff) * 1e6;
  return result;
} /* ElapsedTime() */
