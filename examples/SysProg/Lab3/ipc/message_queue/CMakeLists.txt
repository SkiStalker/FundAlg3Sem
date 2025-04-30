add_library(message_queue STATIC
    src/MessageQueue.cpp
)
target_include_directories(message_queue PUBLIC include)