//@group Misc

#ifndef DGWORKERPOOL_H
#define DGWORKERPOOL_H

#include <stdint.h>

#include "DgError.h"

namespace Dg
{
  // We use a plain old function pointer instead of a std::function<void(void*)>. 
  // This allows the implementation of the Worker Pool to use minimal memory allocations.
  // A consequence is the capture clause of a lambda must be empty.
  typedef void (*WorkerPoolCallback)(void *);

  class WorkerPool
  {
    WorkerPool(WorkerPool const &) = delete;
    WorkerPool operator=(WorkerPool const &) = delete;

  public:

    // TODO We use a combination of ErrorCode and throw in this library. Maybe choose one error system.
    WorkerPool(uint32_t a_totalThreads);
    ~WorkerPool();

    // Adds a function to run on a background thread, optionally with userdata. If clearMemory is true, it will call delete on pUserData after running
    ErrorCode AddTask(WorkerPoolCallback func, void * pUserData = nullptr, bool clearMemory = true, WorkerPoolCallback postFunction = nullptr);

    // This must be run on the main thread, handles marshalling work back from worker threads if required
    // The parameter can be used to limit how much work is done each time this is called
    // Returns number of tasks processed
    uint32_t DoPostWork(uint32_t processLimit = 0xFFFFFFFF);

    // Returns true if there are workers currently processing tasks or if workers should be processing tasks
    bool HasActiveWorkers();

  private:

    struct PIMPL;
    PIMPL * m_pimpl;
  };
}

#endif