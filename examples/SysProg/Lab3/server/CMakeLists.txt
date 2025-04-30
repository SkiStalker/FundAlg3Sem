add_executable(server
    src/main.cpp
    src/Server.cpp
)

target_include_directories(server PRIVATE include)
target_link_libraries(server PRIVATE tcp_server logger exceptions message_queue shared_memory semaphore pthread)
