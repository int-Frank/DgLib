//@group Misc/impl

#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <vector>

#include "../DgWorkerPool.h"
#include "../DgDoublyLinkedList.h"

namespace Dg
{
  struct WorkerPoolTask
  {
    WorkerPoolCallback function;
    WorkerPoolCallback postFunction; // runs on main thread
    void * pUserData;
    bool freeUserData;
  };

  struct WorkerPool::PIMPL
  {
    PIMPL()
      : threadsWorking(0)
      , shouldQuit(false)
    {}

    std::atomic<uint32_t> threadsWorking;
    std::atomic<bool> shouldQuit;
    std::mutex queuedTasksMutex;
    std::mutex queuedPostTasksMutex;
    std::condition_variable cv;
    Dg::DoublyLinkedList<WorkerPoolTask> queuedTasks;
    Dg::DoublyLinkedList<WorkerPoolTask> queuedPostTasks;
    std::vector<std::thread> workerThreads;
  };

  // Helper class (hack) to ensure the threadsWorking counter is decremented if anything in the worker thread throws.
  class CounterHelper
  {
  public:
    CounterHelper(std::atomic<uint32_t> * a_ptr)
      : m_pThreadsWorking(a_ptr)
      , m_shouldDecrement(true)
    { }

    ~CounterHelper()
    {
      if (m_shouldDecrement)
        (*m_pThreadsWorking)--;
    }

    void ShouldDecrement(bool a_val)
    {
      m_shouldDecrement = a_val;
    }

  private:
    std::atomic<uint32_t> * m_pThreadsWorking;
    bool m_shouldDecrement;
  };

  WorkerPool::WorkerPool(uint32_t a_totalThreads)
    : m_pimpl(new PIMPL())
  {
    if (a_totalThreads == 0)
      throw std::invalid_argument("Worker pool initialised with 0 threads!");

    for (uint32_t i = 0; i < a_totalThreads; i++)
      m_pimpl->workerThreads.emplace_back([this]
        {
          while (!this->m_pimpl->shouldQuit)
          {
            WorkerPoolTask temp = {};

            {
              std::unique_lock<std::mutex> lock(this->m_pimpl->queuedTasksMutex);
              this->m_pimpl->cv.wait(lock,
                [this]{ return (!this->m_pimpl->queuedTasks.empty()) || this->m_pimpl->shouldQuit; });

              if (this->m_pimpl->shouldQuit)
                break;

              this->m_pimpl->threadsWorking++;
              CounterHelper ch(&this->m_pimpl->threadsWorking);
              temp = this->m_pimpl->queuedTasks.front();
              this->m_pimpl->queuedTasks.pop_front();
              ch.ShouldDecrement(false);
            }

            CounterHelper ch(&this->m_pimpl->threadsWorking);
            temp.function(temp.pUserData);

            if (temp.postFunction != nullptr)
            {
              std::unique_lock<std::mutex> lock(this->m_pimpl->queuedPostTasksMutex);
              this->m_pimpl->queuedPostTasks.push_back(temp);
            }
            else if (temp.freeUserData)
            {
              delete temp.pUserData;
            }
          }
        });
  }

  WorkerPool::~WorkerPool()
  {
    {
      std::unique_lock<std::mutex> lock(m_pimpl->queuedTasksMutex);
      m_pimpl->shouldQuit = true;
    }

    m_pimpl->cv.notify_all();
    for (std::thread & worker: m_pimpl->workerThreads)
      worker.join();
  }

  ErrorCode WorkerPool::AddTask(WorkerPoolCallback a_func, void * a_pUserData, bool a_clearMemory, WorkerPoolCallback a_postFunction)
  {
    WorkerPoolTask temp{};
    temp.freeUserData = a_clearMemory;
    temp.function = a_func;
    temp.postFunction = a_postFunction;
    temp.pUserData = a_pUserData;

    {
      std::unique_lock<std::mutex> lock(m_pimpl->queuedTasksMutex);
      m_pimpl->queuedTasks.push_back(temp);
    }

    m_pimpl->cv.notify_one();
    return ErrorCode::None;
  }

  uint32_t WorkerPool::DoPostWork(uint32_t a_processLimit)
  {
    uint32_t doneTasks = 0;
    for (uint32_t i = 0; i < a_processLimit; i++)
    {
      bool newTask = false;
      WorkerPoolTask temp = {};
      {
        std::unique_lock<std::mutex> lock(m_pimpl->queuedPostTasksMutex);

        if (!m_pimpl->queuedPostTasks.empty())
        {
          temp = m_pimpl->queuedPostTasks.front();
          m_pimpl->queuedPostTasks.pop_front();
          newTask = true;
        }
      }

      if (!newTask)
        break;

      temp.postFunction(temp.pUserData);
      if (temp.freeUserData)
        delete temp.pUserData;

      doneTasks++;
    }
    return doneTasks;
  }

  bool WorkerPool::HasActiveWorkers()
  {
    std::unique_lock<std::mutex> lock(m_pimpl->queuedTasksMutex);
    return (!m_pimpl->queuedTasks.empty() || (m_pimpl->threadsWorking != 0));
  }
}