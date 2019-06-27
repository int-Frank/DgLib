//! @file DgTimer.h
//!
//! @author Frank Hart
//! @date
//!
//! Class declaration: DgTimer

#ifndef DGTIMER_H
#define DGTIMER_H

#include <stdint.h>

namespace Dg
{
  //! @ingroup DgUtility_types
  //!
  //! @class Timer
  //!
  //! @brief Timer
  //!
  //! @author Frank Hart
  //! @date
  class Timer
  {
  public:

    Timer();

    bool IsStarted() const;
    bool IsStopped() const;
    bool IsPaused() const;

    void Start();
    void Stop();
    void Pause();
    void Resume();

    //In seconds...
    double GetTime() const;

  private:

    int64_t m_totalTime;
    int64_t m_startTime;
    int     m_state;
  };
}

#endif