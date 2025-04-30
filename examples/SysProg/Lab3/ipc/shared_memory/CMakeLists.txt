add_library(shared_memory STATIC
    src/SharedMemory.cpp
)
target_include_directories(shared_memory PUBLIC include)