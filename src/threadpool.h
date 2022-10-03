#ifndef INCLUDED_THREADPOOL_H
#define INCLUDED_THREADPOOL_H

#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace MT {

class ThreadPool {
public:
  // PUBLIC TYPES
  using Task = std::function<void()>;

private:
  // DATA
  uint16_t                 d_numThreads;
  std::atomic_bool         d_running;
  std::vector<std::thread> d_threads;
  std::queue<Task>         d_tasks;
  std::mutex               d_tasksMutex;
  std::condition_variable  d_condition;

public:
  // CREATORS
  ThreadPool(uint16_t numThreads = std::thread::hardware_concurrency());
  ~ThreadPool();

  // MANIPULATORS
  void addTask(ThreadPool::Task task);
  void stop();
  bool isBusy();

private:
  void init();
  void waitForTask();
};

}  // namespace MT
#endif
