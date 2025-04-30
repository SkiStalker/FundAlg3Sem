add_library(tcp_server STATIC
    src/TCPServer.cpp
)
target_include_directories(tcp_server PUBLIC include)