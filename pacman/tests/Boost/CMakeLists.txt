cmake_minimum_required(VERSION 3.5)

add_executable(boost_builds builds.cpp)

add_test(NAME boost_builds COMMAND $<TARGET_FILE:boost_builds>)
