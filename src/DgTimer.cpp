
#include <Windows.h>
#include <stdint.h>

#include "DgTimer.h"

typedef  LONGLONG timer_int;

namespace Dg
{
  namespace
  {
    enum State
    {
      On,
      Off,
      Paused
    };
  }

  Timer::Timer()
    : m_totalTime(0)
    , m_startTime(0)
    , m_state(Off)
  {

  }

  bool Timer::IsStarted() const
  {
    return m_state == On;
  }

  bool Timer::IsStopped() const
  {
    return m_state == Off;
  }

  bool Timer::IsPaused() const
  {
    return m_state == Paused;
  }

  void Timer::Start()
  {
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    m_startTime = t.QuadPart;
    m_totalTime = 0;
    m_state = On;
  }

  void Timer::Stop()
  {
    m_startTime = 0;
    m_totalTime = 0;
    m_state = Off;
  }

  void Timer::Pause()
  {
    if (m_state == On)
    {
      LARGE_INTEGER t;
      QueryPerformanceCounter(&t);
      m_totalTime += t.QuadPart - m_startTime;
      m_state = Paused;
    }
    else if (m_state == Paused)
    {
      Resume();
    }
  }

  void Timer::Resume()
  {
    if (m_state == Paused)
    {
      LARGE_INTEGER t;
      QueryPerformanceCounter(&t);
      m_startTime = t.QuadPart;
      m_state = On;
    }
  }

  double Timer::GetTime() const
  {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    if (m_state == On)
    {
      LARGE_INTEGER currentTime;
      QueryPerformanceCounter(&currentTime);
      return static_cast<double>(m_totalTime + (currentTime.QuadPart - m_startTime))
           / static_cast<double>(frequency.QuadPart);
    }
    else if (m_state == Paused)
    {
      return static_cast<double>(m_totalTime) / static_cast<double>(frequency.QuadPart);
    }

    return 0.0;
  }
}