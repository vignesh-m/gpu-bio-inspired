cmake_minimum_required(VERSION 2.8.9)
project (inspired)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(test_aco tests/test_aco.cpp ${SOURCES})
target_link_libraries(test_aco ${LINK_LIBS})