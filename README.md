# threadpool

Simple C++ non-copyable and non-movable threadpool

# Usage

```
MT::Threadpool pool;

for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
  pool.addTask([]() { std::this_thread::sleep_for (std::chrono::seconds(3)); });
}

while(pool.isBusy()) {
  // make sure all tasks are started
  std::this_thread::sleep_for (std::chrono::milliseconds(100));
}

```

This should take only about 3 seconds to complete all tasks.


# Other implementations

1. https://github.com/vit-vit/CTPL
2. https://github.com/AtanasRusevPros/CPP11_ThreadPool
3. https://github.com/Ethan13310/Thread-Pool-Cpp
4. boost::thread_pool
5. https://docs.pocoproject.org/current/Poco.ThreadPool.html
