project(ThreadPool C CXX)

add_library(threadpool
  threadpool.cpp
)

target_include_directories(threadpool PUBLIC ".")
