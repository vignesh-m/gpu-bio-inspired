cmake_minimum_required(VERSION 2.6.0)
project (inspired)
find_package(CUDA REQUIRED)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(test_aco tests/test_aco.cpp ${SOURCES})
cuda_add_executable(test_aco_cu tests/ant_colony_tsp.cu)
target_link_libraries(test_aco ${LINK_LIBS})
