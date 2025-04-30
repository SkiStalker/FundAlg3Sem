add_library(logger STATIC
    src/Logger.cpp
)
target_include_directories(logger PUBLIC include)