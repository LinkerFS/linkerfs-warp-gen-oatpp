cmake_minimum_required(VERSION 3.21)
project(linkerfs_warp_gen_oatpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -static)

add_subdirectory(src)
add_subdirectory(linkerfs)
add_subdirectory(3rd)