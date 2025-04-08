cmake_minimum_required(VERSION 3.10)
project(MyProject LANGUAGES CXX)

enable_testing()

# Флаги компиляции
set(COMMON_FLAGS "-Wall -Wextra -Wpedantic -Werror -fsanitize=address -fsanitize=leak")

# Флаги для покрытия кода (активны только в Debug)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COVERAGE_FLAGS "--coverage -fprofile-arcs -ftest-coverage")
endif()

# Добавляем GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Добавляем библиотеку
add_library(my_lib src/my_lib.cpp)
target_include_directories(my_lib PUBLIC include)
target_compile_options(my_lib PRIVATE ${COMMON_FLAGS} ${COVERAGE_FLAGS})
target_link_libraries(my_lib PRIVATE ${COVERAGE_FLAGS})

# Создаём отдельный исполняемый файл для тестов
add_executable(tests test/test_my_lib.cpp)
target_link_libraries(tests PRIVATE my_lib GTest::gtest_main)
target_compile_options(tests PRIVATE ${COMMON_FLAGS} ${COVERAGE_FLAGS})
target_link_libraries(tests PRIVATE ${COVERAGE_FLAGS})

# Регистрируем тесты
add_test(NAME MyTests COMMAND tests)

# Добавляем цель для покрытия кода
find_program(LCOV lcov)
find_program(GENHTML genhtml)

if(LCOV AND GENHTML)
    add_custom_target(coverage
        COMMAND ${LCOV} --capture --directory . --output-file coverage.info
        COMMAND ${LCOV} --remove coverage.info '/usr/*' '*/googletest/*' --output-file coverage_filtered.info
        COMMAND ${GENHTML} coverage_filtered.info --output-directory coverage_report
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Генерация отчёта покрытия кода..."
        VERBATIM
    )
else()
    message(WARNING "lcov или genhtml не найдены, цель 'coverage' недоступна.")
endif()

# Добавляем цель для анализа кода cppcheck
find_program(CPPCHECK cppcheck)

if(CPPCHECK)
    add_custom_target(cppcheck
        COMMAND ${CPPCHECK} --enable=all --inconclusive --quiet --suppress=missingIncludeSystem -I include src test
        COMMENT "Запуск cppcheck..."
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        VERBATIM
    )
else()
    message(WARNING "cppcheck не найден, цель 'cppcheck' недоступна.")
endif()