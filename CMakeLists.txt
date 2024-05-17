cmake_minimum_required(VERSION 3.21)
project(linkerfs_warp_gen_oatpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(QT NAMES QT5 QT6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

find_package(oatpp REQUIRED)
find_package(oatpp-swagger REQUIRED)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -static)
include_directories(${oatpp_INCLUDE_DIRS} ${oatpp-swagger_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src)
file(GLOB_RECURSE SOURCES src/*.?pp)
add_definitions(-DOATPP_SWAGGER_RES_PATH="${oatpp-swagger_INCLUDE_DIRS}/../bin/oatpp-swagger/res")

add_executable(linkerfs_warp_gen_oatpp ${SOURCES})
target_link_libraries(linkerfs_warp_gen_oatpp PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        oatpp::oatpp
        oatpp::oatpp-swagger
)


