cmake_minimum_required(VERSION 3.21)
project(linkerfs_warp_gen_oatpp LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 99)
add_subdirectory(linkerfs)
add_subdirectory(3rd)
add_subdirectory(src)