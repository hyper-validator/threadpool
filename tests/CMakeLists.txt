project(test C CXX)

add_executable(test.tsk
  test.m.cpp
)

target_include_directories(test.tsk PRIVATE ".")
target_link_libraries(test.tsk PRIVATE threadpool)
