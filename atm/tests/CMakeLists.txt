cmake_minimum_required(VERSION 3.10)

add_executable(bank_test test_bank.cpp)
target_link_libraries(bank_test atm gtest gtest_main)
target_compile_options(bank_test PRIVATE -Wall -Wextra -Werror)
add_test(NAME bank_test COMMAND bank_test)

add_executable(atm_test test_atm.cpp)
target_link_libraries(atm_test atm gtest gtest_main)
target_compile_options(atm_test PRIVATE -Wall -Wextra -Werror)
add_test(NAME atm_test COMMAND atm_test)
