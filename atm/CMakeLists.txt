cmake_minimum_required(VERSION 3.10)

project(atm_library CXX)

add_library(atm SHARED 
  atm_controller.hpp
  atm_controller.cpp
  bank.cpp
  bank.hpp
  bank_api.hpp
  card.hpp
)

target_include_directories(atm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(tests)
