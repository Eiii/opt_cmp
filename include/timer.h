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
    double ElapsedTime();

  protected:
    bool is_running;
    struct rusage start_ru; 
};
