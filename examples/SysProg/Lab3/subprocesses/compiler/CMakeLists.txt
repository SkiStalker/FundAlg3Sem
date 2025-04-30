add_executable(compiler_worker
    src/main.cpp
    src/CompilerWorker.cpp
)

target_include_directories(compiler_worker PRIVATE include)
target_link_libraries(compiler_worker PRIVATE logger exceptions shared_memory semaphore pthread)
