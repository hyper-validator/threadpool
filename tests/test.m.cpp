#include <threadpool.h>

#include <chrono>
#include <iostream>
#include <mutex>

std::mutex g_logMutex;

void job(int jobId) {
  {
    std::lock_guard<std::mutex> lock(g_logMutex);
    std::cout << std::this_thread::get_id() << " running " << jobId << "..." << std::endl;
  }

  std::this_thread::sleep_for (std::chrono::seconds(5));

  {
    std::lock_guard<std::mutex> lock(g_logMutex);
    std::cout << std::this_thread::get_id() << " done running " << jobId << "..." << std::endl;
  }
}

int main(int argc, char* argv[]) {
  MT::ThreadPool pool(8);

  // std::this_thread::sleep_for (std::chrono::seconds(1));
  for (int i = 1; i <= 8; ++i) {
    pool.addTask([i]() { job(i); });
  }

  while(pool.isBusy()) {
    std::this_thread::sleep_for (std::chrono::milliseconds(100));
  }
  return 0;
}
