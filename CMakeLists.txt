cmake_minimum_required(VERSION 3.10)
project(atm_sample VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
set(BUILD_SHARED_LIBS ON)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(atm)

set(CLANG_TIDY_COMMAND clang-tidy)
file(GLOB_RECURSE ALL_SOURCE_FILES 
     ${CMAKE_CURRENT_SOURCE_DIR}/atm/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/atm/*.hpp)

add_custom_target(tidy
    COMMAND ${CLANG_TIDY_COMMAND} -p ${CMAKE_BINARY_DIR} ${ALL_SOURCE_FILES}
)

add_custom_target(format
  COMMAND find .. -name '*.cpp' -o -name '*.hpp' | xargs clang-format-15 -i)

add_custom_target(tests
    COMMAND ${CMAKE_COMMAND} ..
    COMMAND ${CMAKE_COMMAND} --build . --target all
    COMMAND ${CMAKE_COMMAND} --build . --target test
    COMMENT "Running all tests after build"
)

if(CMAKE_BUILD_TYPE STREQUAL "Asan")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()
