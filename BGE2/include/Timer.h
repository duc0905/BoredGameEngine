#pragma once
#include "../pch.h"
#include "Bases.h"

namespace Bored {
  /**
   * @brief Provide chrono functionalities
   */
  class Timer : public Module
  {
    std::chrono::system_clock::time_point last_frame;

  public:
    virtual void OnSetup() override
    {
      last_frame = std::chrono::system_clock::now();
    }

    double GetDt() const
    {
      return std::chrono::duration<double>(std::chrono::system_clock::now() - last_frame)
        .count();
    }

    virtual bool OnTick(double) override
    {
      last_frame = std::chrono::system_clock::now();
      return false;
    }
  };
}
