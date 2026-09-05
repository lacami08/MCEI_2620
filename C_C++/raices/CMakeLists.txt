
cmake_minimum_required(VERSION 3.10)
project(raices)

find_package(GSL REQUIRED)

add_executable(${PROJECT_NAME} main_LauraAndrade.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE GSL::gsl GSL::gslcblas)