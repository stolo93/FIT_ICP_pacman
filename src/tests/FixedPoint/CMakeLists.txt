cmake_minimum_required(VERSION 3.5)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../GameState)

add_compile_definitions(OVERFLOW_CHECKS)
add_compile_options(-g)
add_library(Pos STATIC ../../GameState/Pos.h)

add_executable(from_int from_int.cpp)
target_link_libraries(from_int Pos)

add_executable(comparison comparison.cpp)
target_link_libraries(comparison Pos)

add_executable(addition addition.cpp)
target_link_libraries(addition Pos)

add_executable(subtraction subtraction.cpp)
target_link_libraries(subtraction Pos)

add_executable(multiplication multiplication.cpp)
target_link_libraries(multiplication Pos)

add_test(NAME from_int COMMAND $<TARGET_FILE:from_int>)
add_test(NAME comparison COMMAND $<TARGET_FILE:comparison>)
add_test(NAME addition COMMAND $<TARGET_FILE:addition>)
add_test(NAME subtraction COMMAND $<TARGET_FILE:subtraction>)
add_test(NAME multiplication COMMAND $<TARGET_FILE:multiplication>)

