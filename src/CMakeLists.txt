# Qt
find_package(QT NAMES QT5 QT6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

# oat++
find_package(oatpp REQUIRED)
find_package(oatpp-swagger REQUIRED)
include_directories(${oatpp_INCLUDE_DIRS} ${oatpp-swagger_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src)
add_definitions(-DOATPP_SWAGGER_RES_PATH="${oatpp-swagger_INCLUDE_DIRS}/../bin/oatpp-swagger/res")

set(SOURCES
        App.cpp
        ErrorHandler.cpp
        common/utils/File.cpp
        service/DirService.cpp
)

add_executable(linkerfs_warp_gen_oatpp ${SOURCES})
target_link_libraries(linkerfs_warp_gen_oatpp PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        oatpp::oatpp
        oatpp::oatpp-swagger
        liblinkerfs
)
set_target_properties(linkerfs_warp_gen_oatpp PROPERTIES OUTPUT_NAME "lwg_server" RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})