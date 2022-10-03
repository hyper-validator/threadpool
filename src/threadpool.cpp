#include <threadpool.h>

namespace MT {

ThreadPool::ThreadPool(uint16_t numThreads)
    : d_numThreads(numThreads), d_running(false) {
  init();
}

ThreadPool::~ThreadPool() { stop(); }

void ThreadPool::addTask(ThreadPool::Task task) {
  if (d_running) {
    {
      const std::lock_guard<std::mutex> lock(d_tasksMutex);
      d_tasks.push(task);
    }
    d_condition.notify_one();
  }
}

void ThreadPool::stop() {
  if (d_running) {
    d_running = false;
    d_condition.notify_all();
    for (auto& runningThread : d_threads) {
      if (runningThread.joinable()) {
        runningThread.join();  // use shutdown policy class: join or terminate?
      }
    }

    {
      std::lock_guard<std::mutex> lock(d_tasksMutex);
      if (!d_tasks.empty()) {
        std::cerr << "warning: " << d_tasks.size() << " remaining tasks"
                  << std::endl;
      }
    }
  }
}

bool ThreadPool::isBusy() {
  std::lock_guard<std::mutex> lock(d_tasksMutex);
  return !d_tasks.empty();
}

void ThreadPool::init() {
  if (!d_running) {
    d_running = true;
    for (uint16_t i = 0; i < d_numThreads; ++i) {
      d_threads.push_back(std::thread(&ThreadPool::waitForTask, this));
    }
  }
}

void ThreadPool::waitForTask() {
  while (true) {
    {
      std::unique_lock<std::mutex> lock(d_tasksMutex);  // has to be unique_lock
      d_condition.wait(
          lock, [this]() { return !this->d_running || !d_tasks.empty(); });
    }

    if (!d_running) {
      break;
    }

    if (!d_tasks.empty()) {
      ThreadPool::Task task = d_tasks.front();
      d_tasks.pop();
      task();
    }

    std::this_thread::yield();
  }
}

}  // namespace MT
