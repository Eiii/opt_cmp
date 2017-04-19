#pragma once

#include <sys/resource.h>

class CPUTimer
{
  public:
    CPUTimer();
    virtual ~CPUTimer() = default;

  public:
    void Start();
    void Stop();
    void Reset();
    double ElapsedTime() const;
    bool HasStarted() const { return has_started; }

  protected:
    bool is_running;
    bool has_started;
    struct rusage start_ru; 
    double total_time;
};
