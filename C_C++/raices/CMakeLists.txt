cmake_minimum_required(VERSION 3.10)
project(raices)

find_package(GSL REQUIRED)

add_executable(raices main_LauraAndrade.cpp)
target_link_libraries(raices GSL::gsl GSL::gslcblas)