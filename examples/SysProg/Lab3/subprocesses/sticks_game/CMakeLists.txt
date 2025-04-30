add_executable(game_worker
    src/main.cpp
    src/GameWorker.cpp
)

target_include_directories(game_worker PRIVATE include)
target_link_libraries(game_worker PRIVATE logger exceptions message_queue pthread)
